#pragma once
#include "Static.h"

#define IDC_INSERT                      1001

#define IDC_COMBINE                     1002
#define IDC_UPDATE                      1003
#define IDC_SIZEUP                      1004
#define IDC_SIZEDOWN                    1005
#define IDC_COPY                        1006

#define IDC_COLORPEN                    1007
#define IDC_COLORBRUSH                  1008
#define IDC_COMBOBOXPEN                 1009
#define IDC_COMBOBOXBRUSH               1010

#define IDC_RADIOF                      1011
#define IDC_RADIOSELF                   1021

#define IDC_SELECTALL                  1101
#define IDC_UNSELECTALL                1102
#define IDC_CLEARHDC                   1103

#define IDC_DELETE                     1203


class Main {
    HWND hWnd = {};


public:

    Static StaticBox = {};

    HWND SliderPen = {};
    Area ColorPen = {};
    HWND ComboBoxPen = {};

    Area ColorBrush = {};
    HWND ComboBoxBrush = {};

    HMENU ContextMenu = CreateContextMenu();

    Main() {}
    Main(HWND hWnd) {
        HWND child = 0;
        int style = WS_CHILD | WS_VISIBLE;
        StaticBox = Static(CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, "static", "", style | WS_BORDER | BS_OWNERDRAW, 10, 10, 580, 580, hWnd, (HMENU)0, GetModuleHandle(0), 0));


        CreateWindowEx(0, "button", "Настройка контура", style | BS_GROUPBOX, 600, 10, 370, 120, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Толщина линии", style | WS_BORDER | ACS_CENTER, 610, 40, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        SliderPen = CreateWindowEx(0, "msctls_trackbar32", "", style | TBS_BOTH | TBS_NOTICKS | TBS_TOOLTIPS, 770, 40, 190, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        SendMessage(SliderPen, TBM_SETRANGE, TRUE, MAKELPARAM(1, 10));
        CreateWindowEx(0, "static", "Цвет линии", style | WS_BORDER | ACS_CENTER, 610, 70, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        ColorPen = Area(CreateWindowEx(0, "button", "", style | WS_BORDER | BS_OWNERDRAW, 770, 70, 190, 20, hWnd, (HMENU)IDC_COLORPEN, GetModuleHandle(0), 0));
        CreateToolTip(ColorPen, "Нажмите для выбора цвета");
        CreateWindowEx(0, "static", "Тип линии", style | WS_BORDER | ACS_CENTER, 610, 100, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        ComboBoxPen = CreateWindow(WC_COMBOBOX, "", style | WS_BORDER | CBS_DROPDOWNLIST, 770, 100, 190, 170, hWnd, (HMENU)IDC_COMBOBOXPEN, GetModuleHandle(0), 0);
        SendMessage(ComboBoxPen, CB_ADDSTRING, 0, (LPARAM)"Простой");
        SendMessage(ComboBoxPen, CB_ADDSTRING, 0, (LPARAM)"-------");
        SendMessage(ComboBoxPen, CB_ADDSTRING, 0, (LPARAM)".......");
        SendMessage(ComboBoxPen, CB_ADDSTRING, 0, (LPARAM)"_._._._");
        SendMessage(ComboBoxPen, CB_ADDSTRING, 0, (LPARAM)"_.._.._");
        SendMessage(ComboBoxPen, CB_SETCURSEL, 0, 0);


        CreateWindowEx(0, "button", "Настройка кисти", style | BS_GROUPBOX, 600, 150, 370, 90, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Цвет кисти", style | WS_BORDER | ACS_CENTER, 610, 180, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        ColorBrush = Area(CreateWindowEx(0, "button", "", style | WS_BORDER | BS_OWNERDRAW, 770, 180, 190, 20, hWnd, (HMENU)IDC_COLORBRUSH, GetModuleHandle(0), 0));
        CreateToolTip(ColorBrush, "Нажмите для выбора цвета");
        CreateWindowEx(0, "static", "Стиль кисти", style | WS_BORDER | ACS_CENTER, 610, 210, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        ComboBoxBrush = CreateWindow(WC_COMBOBOX, "", style | WS_BORDER | CBS_DROPDOWNLIST, 770, 210, 190, 200, hWnd, (HMENU)IDC_COMBOBOXBRUSH, GetModuleHandle(0), 0);
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"Простой");
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"-----");
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"|||||");
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"\\\\\\\\\\");
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"/////");
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"+++++");
        SendMessage(ComboBoxBrush, CB_ADDSTRING, 0, (LPARAM)"xxxxx");
        SendMessage(ComboBoxBrush, CB_SETCURSEL, 0, 0);


        CreateWindowEx(0, "button", "Вставка фигур", style | BS_GROUPBOX, 600, 250, 370, 120, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "button", "Прямоугольник", style | BS_RADIOBUTTON, 610, 280, 190, 20, hWnd, (HMENU)IDC_RADIOF, GetModuleHandle(0), 0);
        CreateWindowEx(0, "button", "Эллипс", style | BS_RADIOBUTTON, 610, 300, 190, 20, hWnd, (HMENU)(IDC_RADIOF + 1), GetModuleHandle(0), 0);
        child = CreateWindowEx(0, "button", "Произвольный многоугольник", style | BS_RADIOBUTTON, 610, 320, 190, 20, hWnd, (HMENU)(IDC_RADIOF + 2), GetModuleHandle(0), 0);
        CreateToolTip(child, "Позволяет создать произвольный многоугольник. Ввод производится левой кнопкой мыши, для завершения фигуры используется правая кнопка мыши");
        CheckRadioButton(hWnd, IDC_RADIOF, IDC_RADIOF + 2, IDC_RADIOF);
        child = CreateWindowEx(0, "button", "Вставить", style, 860, 340, 100, 20, hWnd, (HMENU)IDC_INSERT, GetModuleHandle(0), 0);
        CreateToolTip(child, "Вставляет выбраню фигуру в точку последнего нажатия");

        CreateWindowEx(0, "button", "Выбор инструментов", style | BS_GROUPBOX, 600, 380, 370, 70, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 610, 410, 32, 32, hWnd, (HMENU)IDC_COMBINE, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "1.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        CreateToolTip(child, "Объединить фигуры");
        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 650, 410, 32, 32, hWnd, (HMENU)IDC_UPDATE, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "2.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        CreateToolTip(child, "Обновить фигуры");
        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 690, 410, 32, 32, hWnd, (HMENU)IDC_SIZEUP, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "3.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        CreateToolTip(child, "Увеличить фигуры");
        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 732, 410, 32, 32, hWnd, (HMENU)IDC_SIZEDOWN, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "4.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        CreateToolTip(child, "Уменьшить фигуры");
        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 772, 410, 32, 32, hWnd, (HMENU)IDC_COPY, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "5.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        CreateToolTip(child, "Копировать");

        CreateWindowEx(0, "button", "Отображение фигур", style | BS_GROUPBOX, 600, 460, 370, 80, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "button", "Все на листе", style | BS_RADIOBUTTON, 610, 490, 190, 20, hWnd, (HMENU)IDC_RADIOSELF, GetModuleHandle(0), 0);
        CreateWindowEx(0, "button", "Только выделенные", style | BS_RADIOBUTTON, 610, 510, 190, 20, hWnd, (HMENU)(IDC_RADIOSELF + 1), GetModuleHandle(0), 0);
        CheckRadioButton(hWnd, IDC_RADIOSELF, IDC_RADIOSELF + 1, IDC_RADIOSELF);
    }

    static void Open(WNDPROC wndproc) {
        WNDCLASS wc{};
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = wndproc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetModuleHandle(0);
        wc.hIcon = 0;
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = CreateSolidBrush(0xEEEEEE);
        wc.lpszMenuName = 0;
        wc.lpszClassName = "Window";
        RegisterClass(&wc);

        auto hWnd = CreateWindow("Window", "Regions Manager", WS_OVERLAPPED | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 660, 0, CreateMainMenu(), GetModuleHandle(0), 0);
        ShowWindow(hWnd, 10);
        UpdateWindow(hWnd);
    }

private:
    HWND CreateToolTip(HWND hWndTool, LPCSTR pszText) {
        HWND tooltip = CreateWindowEx(0, TOOLTIPS_CLASS, 0, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON | TTS_CLOSE | TTS_USEVISUALSTYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, 0, GetModuleHandle(0), 0);

        TOOLINFO ti{};
        ti.cbSize = sizeof(ti);
        ti.hwnd = hWnd;
        ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_PARSELINKS;
        ti.uId = (UINT_PTR)hWndTool;
        ti.lpszText = (LPSTR)pszText;

        SendMessage(tooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
        SendMessage(tooltip, TTM_SETMAXTIPWIDTH, 0, 150);
        return tooltip;
    }

   static HMENU CreateMainMenu() {
        auto hMainMenu = CreateMenu();
        {
            auto hMenu = CreatePopupMenu();
            AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (INT)hMenu, "Выделение");
            {
                AppendMenu(hMenu, MF_STRING, IDC_SELECTALL, "Выделить всё");
                AppendMenu(hMenu, MF_STRING, IDC_UNSELECTALL, "Снять выделения");
            }
        }
        AppendMenu(hMainMenu, MF_STRING, IDC_CLEARHDC, "Отчистить лист");
        return hMainMenu;
    }

   HMENU CreateContextMenu() {
        auto hMainMenu = CreatePopupMenu();
        AppendMenu(hMainMenu, MF_STRING, IDC_COMBINE, "Объединить");
        AppendMenu(hMainMenu, MF_STRING, IDC_COPY, "Клонировать");
        AppendMenu(hMainMenu, MF_STRING, IDC_DELETE, "Удалить");
        return hMainMenu;
    }
};