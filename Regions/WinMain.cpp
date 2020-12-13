#include <Windows.h>
#include <windowsx.h>
#include <Commctrl.h>

#include <vector>
#include <set>

#include <memory>

#include "resource.h"
#include "Region.h"
#include "Gui.h"

Main Window;
INT Mode;

POINT LastMousePos;
std::shared_ptr<int> s;
std::vector<std::shared_ptr<Region>> Regions;
std::vector<std::shared_ptr<Region>> SelectedRegions;
std::vector<POINT> points;


BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
    Window = Main(hWnd);
    return 1;
}

void Cls_OnDestroy(HWND hWnd) {
    PostQuitMessage(0);
}

void Cls_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    SetFocus(hWnd);
    switch(Mode) {
        case 0:
        case 1:
            if(PtInRect(&Window.StaticBox.RT, {x - 10, y - 10})) {
                LastMousePos = {x - 10, y - 10};
                if(auto region = Region::Search(Regions, LastMousePos)) {
                    SelectedRegions.push_back(region);
                    Regions.erase(std::find(Regions.begin(), Regions.end(), region));
                    region->Select(Window.StaticBox);
                } else if(auto region = Region::Search(SelectedRegions, LastMousePos)) {
                    Regions.push_back(region);
                    SelectedRegions.erase(std::find(SelectedRegions.begin(), SelectedRegions.end(), region));
                    region->Fill(Window.StaticBox);
                }
            }
            break;
        case 2:
            if(PtInRect(&Window.StaticBox.RT, {x - 10, y - 10})) {
                LastMousePos = {x - 10, y - 10};
                if(auto region = Region::Search(Regions, LastMousePos)) {
                    region->Pen(Window.StaticBox.Width, Window.StaticBox.ColorPen);
                    region->Brush(Window.StaticBox.ColorBrush);
                    region->Fill(Window.StaticBox);
                }
            }
            break;
        case 3:
            points.push_back({x, y});
            SelectObject(Window.StaticBox, GetStockObject(BLACK_PEN));
            MoveToEx(Window.StaticBox, points.front().x, points.front().y, 0);
            for(auto it = 1; it < points.size(); ++it)
                LineTo(Window.StaticBox, points[it].x, points[it].y);
            break;
    }
}

void Cls_OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    switch(Mode) {
        case 1:
            Regions.push_back(Region::Combine(SelectedRegions));
            Regions.back()->Pen(Window.StaticBox.Width, Window.StaticBox.ColorPen);
            Regions.back()->Brush(Window.StaticBox.ColorBrush);
            Regions.back()->Fill(Window.StaticBox);
            SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 0);
            break;
        case 3:
            Regions.push_back(std::shared_ptr<Region>(new Region(points)));
            Regions.back()->Pen(Window.StaticBox.Width, Window.StaticBox.ColorPen);
            Regions.back()->Brush(Window.StaticBox.ColorBrush);
            Regions.back()->Fill(Window.StaticBox);
            points.clear();
            InvalidateRect(hWnd, 0, 0);
            break;
    }
}

void Cls_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
    switch(id) {
        case IDC_COLORPEN:
            Window.StaticBox.ColorPen = ChooseColorDialog(hWnd).Show();
            InvalidateRect(Window.ColorPen, 0, 0);
            break;
        case IDC_COLORBRUSH:
            Window.StaticBox.ColorBrush = ChooseColorDialog(hWnd).Show();
            InvalidateRect(Window.ColorBrush, 0, 0);
            break;
        case IDC_INSERT:
            for(auto it = 0; it < 3; ++it)
                if(IsDlgButtonChecked(hWnd, IDC_RADIO + it)) {
                    switch(it) {
                        case 0:
                            Regions.push_back(std::shared_ptr<Region>(new Region(CreateRectRgn(
                                LastMousePos.x, LastMousePos.y, LastMousePos.x + 70, LastMousePos.y + 60))));
                            Regions.back()->Pen(Window.StaticBox.Width, Window.StaticBox.ColorPen);
                            Regions.back()->Brush(Window.StaticBox.ColorBrush);
                            Regions.back()->Fill(Window.StaticBox);
                            break;
                        case 1:
                            Regions.push_back(std::shared_ptr<Region>(new Region(CreateEllipticRgn(
                                LastMousePos.x, LastMousePos.y, LastMousePos.x + 180, LastMousePos.y + 60))));
                            Regions.back()->Pen(Window.StaticBox.Width, Window.StaticBox.ColorPen);
                            Regions.back()->Brush(Window.StaticBox.ColorBrush);
                            Regions.back()->Fill(Window.StaticBox);
                            break;
                        case 2:
                            Mode = 3;
                            points.clear();

                            break;
                    }
                }
            break;
        case IDC_SELECT:
            Mode = 0;
            break;
        case IDC_COMBINE:
            Mode = 1;
            break;
        case IDC_UPDATE:
            Mode = 2;
            break;
    }
}

void Cls_OnDrawItem(HWND hWnd, const DRAWITEMSTRUCT * lpDrawItem) {
    if(lpDrawItem) {
        if(lpDrawItem->hwndItem == Window.ColorPen)
            Window.ColorPen.Paint(Window.StaticBox.ColorPen);
        if(lpDrawItem->hwndItem == Window.ColorBrush)
            Window.ColorBrush.Paint(Window.StaticBox.ColorBrush);
    } else {

        Window.StaticBox.Paint();

        for(auto & region : Regions)
            region->Fill(Window.StaticBox);
        for(auto & region : SelectedRegions)
            region->Select(Window.StaticBox);

        if(points.size()) {
            SelectObject(Window.StaticBox, GetStockObject(BLACK_PEN));
            MoveToEx(Window.StaticBox, points.front().x, points.front().y, 0);
            for(auto it = 1; it < points.size(); ++it)
                LineTo(Window.StaticBox, points[it].x, points[it].y);
        }

    }
}

void Cls_OnPaint(HWND hWnd) {
    PAINTSTRUCT PS;
    RECT        RT;
    auto hDC = BeginPaint(hWnd, &PS);

    Cls_OnDrawItem(hWnd, 0);

    EndPaint(hWnd, &PS);
}

void Cls_OnKey(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) {
    switch(vk) {
        case VK_UP:
            for(auto & region : SelectedRegions) {
                region->Clear(Window.StaticBox);
                region->Offset(0, -10);
                for(auto & region : Regions)
                    region->Fill(Window.StaticBox);
                for(auto & region : SelectedRegions)
                    region->Select(Window.StaticBox);
            }
            break;
        case VK_DOWN:
            for(auto & region : SelectedRegions) {
                region->Clear(Window.StaticBox);
                region->Offset(0, 10);
                for(auto & region : Regions)
                    region->Fill(Window.StaticBox);
                for(auto & region : SelectedRegions)
                    region->Select(Window.StaticBox);
            }
            break;
        case VK_LEFT:
            for(auto & region : SelectedRegions) {
                region->Clear(Window.StaticBox);
                region->Offset(-10, 0);
                for(auto & region : Regions)
                    region->Fill(Window.StaticBox);
                for(auto & region : SelectedRegions)
                    region->Select(Window.StaticBox);
            }
            break;
        case VK_RIGHT:
            for(auto & region : SelectedRegions) {
                region->Clear(Window.StaticBox);
                region->Offset(10, 0);
                for(auto & region : Regions)
                    region->Fill(Window.StaticBox);
                for(auto & region : SelectedRegions)
                    region->Select(Window.StaticBox);
            }
            break;
        case VK_DELETE:
            for(auto & region : SelectedRegions)
                region->Clear(Window.StaticBox);
            for(auto & region : Regions)
                region->Fill(Window.StaticBox);
            SelectedRegions.clear();
            break;
        case 187:
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                for(auto & region : SelectedRegions) {
                    region->Clear(Window.StaticBox);
                    region->Resize(1.1, 1.1);
                    for(auto & region : Regions)
                        region->Fill(Window.StaticBox);
                    for(auto & region : SelectedRegions)
                        region->Select(Window.StaticBox);
                }
            }
            break;
        case 189:
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                for(auto & region : SelectedRegions) {
                    region->Clear(Window.StaticBox);
                    region->Resize(.9, .9);
                    for(auto & region : Regions)
                        region->Fill(Window.StaticBox);
                    for(auto & region : SelectedRegions)
                        region->Select(Window.StaticBox);
                }
            }
            break;
        case 65:
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                SelectedRegions.insert(SelectedRegions.end(), Regions.begin(), Regions.end());
                Regions.clear();
                for(auto & region : SelectedRegions)
                    region->Select(Window.StaticBox);
            }
            break;
    }
}

void Cls_OnScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos) {
    switch(code) {
        case TB_THUMBPOSITION:
        case TB_THUMBTRACK:
            if(hWndCtl == Window.Slider && pos) {
                Window.StaticBox.Width = pos;
            }
            break;
    }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    switch(uMessage) {
        HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, Cls_OnLButtonDown);
        HANDLE_MSG(hWnd, WM_RBUTTONDOWN, Cls_OnRButtonDown);
        HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
        HANDLE_MSG(hWnd, WM_DRAWITEM, Cls_OnDrawItem);
        HANDLE_MSG(hWnd, WM_KEYDOWN, Cls_OnKey);
        HANDLE_MSG(hWnd, WM_HSCROLL, Cls_OnScroll);
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