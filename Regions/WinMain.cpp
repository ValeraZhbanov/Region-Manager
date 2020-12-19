#include <Windows.h>
#include <windowsx.h>
#include <Commctrl.h>

#include <fstream>
#include <string>
#include <regex>
#include <locale>
#include <codecvt>

#include "Regions.h"

MRegions Regions;

Main Window;
INT Mode = -1;

POINT MousePosLClick = {};
POINT LastMousePos = {};

std::vector<POINT> points;


BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
    Window = Main(hWnd);
    Regions = MRegions(&Window);
    return 1;
}

void Cls_OnDestroy(HWND hWnd) {
    PostQuitMessage(0);
}

void Cls_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    SetFocus(hWnd);
    if(PtInRect(&Window.StaticBox.RT, {x - 10, y - 10})) {
        LastMousePos = {x - 10, y - 10};
        switch(Mode) {
            case 0:
                Regions.Add(new FRectangle({LastMousePos.x, LastMousePos.y, LastMousePos.x + 70, LastMousePos.y + 60}));
                Mode = -1;
                return;
            case 1:
                Regions.Add(new FEllipse({LastMousePos.x, LastMousePos.y, LastMousePos.x + 180, LastMousePos.y + 60}));
                Mode = -1;
                return;
            case 2:
                points.push_back({x, y});
                SelectObject(Window.StaticBox, GetStockObject(BLACK_PEN));
                MoveToEx(Window.StaticBox, points.front().x, points.front().y, 0);
                for(auto it = 1; it < points.size(); ++it)
                    LineTo(Window.StaticBox, points[it].x, points[it].y);
                Window.StaticBox.Paint();
                return;
        }
        MousePosLClick = LastMousePos;
        Regions.Select(LastMousePos);
    }
}

void Cls_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
    MousePosLClick = {};
}

void Cls_OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    if(Mode == 2) {
        Regions.Add(new FPolygon(points));
        points.clear();
    }
}

void Cls_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
    x -= 10, y -= 10;
    if(MousePosLClick.x && MousePosLClick.y) {
        auto dx = x - MousePosLClick.x;
        auto dy = y - MousePosLClick.y;
        Regions.Offset(dx, dy);
        MousePosLClick = {x, y};
    }
}

BOOL WINAPI AboutProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    static std::vector<std::wstring> texts;
    switch(uMessage) {
        case WM_INITDIALOG:
            {
                std::wifstream fin(L"about.txt", std::wifstream::binary);
                fin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
                std::wstring text;
                std::getline(fin, text, L'\0');
                {
                    std::wregex rx(L"\n");
                    text = std::regex_replace(text, rx, L"\r\n");
                }
                {
                    std::wregex rgx(L"<br>");
                    std::wsregex_token_iterator iter(text.begin(), text.end(), rgx, -1);
                    std::wsregex_token_iterator end;
                    while(iter != end) {
                        texts.push_back(*iter);
                        ++iter;
                    }
                }
                EnableWindow(Window.hWnd, 0);
                for(auto it = 0; it < 3; ++it)
                    CheckDlgButton(hWnd, IDC_CHECK + it, BM_SETCHECK);
                SendMessage(hWnd, WM_COMMAND, IDC_CHECK, 0);
            }
            return 1;
        case WM_CLOSE:
            EnableWindow(Window.hWnd, 1);
            DestroyWindow(hWnd);
            return 0;
        case WM_COMMAND:
            switch(wParam) {
                case IDCANCEL:
                    SendMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0;
                case IDC_CHECK:
                case IDC_CHECK + 1:
                case IDC_CHECK + 2:
                    {
                        std::wstring text;
                        for(auto it = 0; it < 3; ++it)
                            if(IsDlgButtonChecked(hWnd, IDC_CHECK + it))
                                text += texts[it];
                        SetWindowTextW(GetDlgItem(hWnd, IDC_TEXT), text.c_str());
                    }
                    return 0;
            }
        default:
            return 0;
    }

}

void Cls_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
    switch(id) {
        case IDC_COLORPEN:
            Window.StaticBox.Pen.Color = ChooseColorDialog(hWnd).Show();
            InvalidateRect(Window.ColorPen, 0, 0);
            return;
        case IDC_COLORBRUSH:
            Window.StaticBox.Brush.Color = ChooseColorDialog(hWnd).Show();
            InvalidateRect(Window.ColorBrush, 0, 0);
            return;
        case IDC_COMBOBOXPEN:
            if(codeNotify == CBN_SELCHANGE) {
                Window.StaticBox.Pen.Style = SendMessage(Window.ComboBoxPen, CB_GETCURSEL, 0, 0);
            }
            return;
        case IDC_COMBOBOXBRUSH:
            if(codeNotify == CBN_SELCHANGE) {
                Window.StaticBox.Brush.Style = SendMessage(Window.ComboBoxBrush, CB_GETCURSEL, 0, 0);
            }
            return;
        case IDC_INSERT:
            for(auto it = 0; it < 3; ++it)
                if(IsDlgButtonChecked(hWnd, IDC_RADIOF + it)) {
                    Mode = it;
                }
            return;
        case IDC_COMBINE:
            if(MessageBox(hWnd, "Объединить выделенные фигуры?", "Предупреждение", MB_ICONINFORMATION | MB_OKCANCEL) == IDOK) {
                Regions.Add(new FMulti(Regions.SelectedRegions), false);
                Regions.Delete();
            }
            return;
        case IDC_UPDATE:
            Regions.SetPen(Window.StaticBox.Pen);
            Regions.SetBrush(Window.StaticBox.Brush);
            return;
        case IDC_SIZEUP:
            Regions.Resize(1.1, 1.1);
            return;
        case IDC_SIZEDOWN:
            Regions.Resize(.9, .9);
            return;
        case IDC_COPY:
            Regions.CopyPaste();
            return;
        case IDC_RADIOF:
        case IDC_RADIOF + 1:
        case IDC_RADIOF + 2:
            CheckRadioButton(hWnd, IDC_RADIOF, IDC_RADIOF + 2, id);
            return;
        case IDC_RADIOSELF:
        case IDC_RADIOSELF + 1:
            CheckRadioButton(hWnd, IDC_RADIOSELF, IDC_RADIOSELF + 1, id);
            Regions.DrawAll = id - IDC_RADIOSELF - 1;
            Regions.UpdateAll();
            return;
        case IDC_SELECTALL:
            Regions.SelectAll();
            return;
        case IDC_UNSELECTALL:
            Regions.UnselectAll();
            return;
        case IDC_CLEARHDC:
            if(MessageBox(hWnd, "Отчистить лист?", "Предупреждение", MB_ICONINFORMATION | MB_OKCANCEL) == IDOK)
                Regions.Clear();
            return;
        case IDC_DELETE:
            if(MessageBox(hWnd, "Удалить выделенные фигуры?", "Предупреждение", MB_ICONINFORMATION | MB_OKCANCEL) == IDOK)
                Regions.Delete();
            return;
        case IDC_ABOUT:
            CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_HELP), hWnd, AboutProc);
            return;
    }
}

void Cls_OnDrawItem(HWND hWnd, const DRAWITEMSTRUCT * lpDrawItem) {
    if(lpDrawItem) {
        if(lpDrawItem->hwndItem == Window.ColorPen)
            Window.ColorPen.Paint(Window.StaticBox.Pen.Color);
        if(lpDrawItem->hwndItem == Window.ColorBrush)
            Window.ColorBrush.Paint(Window.StaticBox.Brush.Color);
    }
}

void Cls_OnPaint(HWND hWnd) {
    PAINTSTRUCT PS;
    RECT        RT;
    auto hDC = BeginPaint(hWnd, &PS);

    Window.StaticBox.Paint();

    EndPaint(hWnd, &PS);
}

void Cls_OnKey(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) {
    switch(vk) {
        case VK_UP:
            Regions.Offset(0, -10);
            return;
        case VK_DOWN:
            Regions.Offset(0, 10);
            return;
        case VK_LEFT:
            Regions.Offset(-10, 0);
            return;
        case VK_RIGHT:
            Regions.Offset(10, 0);
            return;
        case VK_DELETE:
            if(MessageBox(hWnd, "Удалить выделенные фигуры?", "Предупреждение", MB_ICONINFORMATION | MB_OKCANCEL) == IDOK)
                Regions.Delete();
            return;
        case 187:
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                Regions.Resize(1.1, 1.1);
            }
            return;
        case 189:
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                Regions.Resize(.9, .9);
            }
            return;
        case 65:
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                Regions.SelectOrUnselectAll();
            }
            return;
    }
}

void Cls_OnScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos) {
    switch(code) {
        case TB_THUMBPOSITION:
        case TB_THUMBTRACK:
            if(hWndCtl == Window.SliderPen && pos) {
                Window.StaticBox.Pen.Width = pos;
            }
            break;
    }
}

void Cls_OnContextMenu(HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos) {
    if(Mode != 2) {
        TrackPopupMenu(Window.ContextMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, xPos, yPos, 0, hWnd, 0);
    } else {
        Mode = -1;
    }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    switch(uMessage) {
        HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, Cls_OnLButtonDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, Cls_OnLButtonUp);
        HANDLE_MSG(hWnd, WM_RBUTTONDOWN, Cls_OnRButtonDown);
        HANDLE_MSG(hWnd, WM_MOUSEMOVE, Cls_OnMouseMove);
        HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
        HANDLE_MSG(hWnd, WM_DRAWITEM, Cls_OnDrawItem);
        HANDLE_MSG(hWnd, WM_KEYDOWN, Cls_OnKey);
        HANDLE_MSG(hWnd, WM_HSCROLL, Cls_OnScroll);
        HANDLE_MSG(hWnd, WM_CONTEXTMENU, Cls_OnContextMenu);
        default: return DefWindowProc(hWnd, uMessage, wParam, lParam);
    }
    return 0;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {
    MSG msg;
    Main::Open(WndProc);
    while(GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}