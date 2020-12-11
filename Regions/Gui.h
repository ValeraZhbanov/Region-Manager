#pragma once
#include "Static.h"


class Main {
    HWND hWnd = {};


public:

    Static StaticBox = {};
    Area ColorPen = {};
    Area ColorBrush = {};
    HWND Slider = {};


    Main() {}
    Main(HWND hWnd) {
        HWND child = 0;
        int style = WS_CHILD | WS_VISIBLE;
        StaticBox = Static(CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, "static", "", style | WS_BORDER, 10, 10, 580, 580, hWnd, (HMENU)IDC_PSTATIC, GetModuleHandle(0), 0));
        CreateWindowEx(0, "button", "Настройка карандаша", style | BS_GROUPBOX, 600, 10, 370, 90, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Толщина линии", style | WS_BORDER | ACS_CENTER, 610, 40, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);

        Slider = CreateWindowEx(0, "msctls_trackbar32", "", style | TBS_BOTH | TBS_NOTICKS | TBS_TOOLTIPS, 770, 40, 190, 20, hWnd, (HMENU)IDC_SLIDER, GetModuleHandle(0), 0);
        SendMessage(Slider, TBM_SETRANGE, TRUE, MAKELPARAM(1, 10));

        CreateWindowEx(0, "static", "Цвет линии", style | WS_BORDER | ACS_CENTER, 610, 70, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        ColorPen = Area(CreateWindowEx(0, "button", "", style | WS_BORDER | BS_OWNERDRAW, 770, 70, 190, 20, hWnd, (HMENU)IDC_COLORPEN, GetModuleHandle(0), 0));
        CreateToolTip(ColorPen, "Нажмите для выбора цвета");
        CreateWindowEx(0, "button", "Настройка кисти", style | BS_GROUPBOX, 600, 120, 370, 60, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Цвет кисти", style | WS_BORDER | ACS_CENTER, 610, 150, 150, 20, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        ColorBrush = Area(CreateWindowEx(0, "button", "", style | WS_BORDER | BS_OWNERDRAW, 770, 150, 190, 20, hWnd, (HMENU)IDC_COLORBRUSH, GetModuleHandle(0), 0));
        CreateToolTip(ColorBrush, "Нажмите для выбора цвета");


        CreateWindowEx(0, "button", "Вставка фигур", style | BS_GROUPBOX, 600, 190, 370, 120, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "button", "Прямоугольник", style | BS_AUTORADIOBUTTON, 610, 220, 190, 20, hWnd, (HMENU)IDC_RADIO, GetModuleHandle(0), 0);
        CreateWindowEx(0, "button", "Эллипс", style | BS_AUTORADIOBUTTON, 610, 240, 190, 20, hWnd, (HMENU)(IDC_RADIO + 1), GetModuleHandle(0), 0);
        child = CreateWindowEx(0, "button", "Произвольный многоугольник", style | BS_AUTORADIOBUTTON, 610, 260, 190, 20, hWnd, (HMENU)(IDC_RADIO + 2), GetModuleHandle(0), 0);
        CreateToolTip(child, "Позволяет создать произвольный многоугольник. Ввод производится левой кнопкой мыши, для завершения фигуры используется правая кнопка мыши");
        CreateWindowEx(0, "button", "Вставить", style, 860, 280, 100, 20, hWnd, (HMENU)IDC_INSERT, GetModuleHandle(0), 0);
        child = CreateWindowEx(0, "button", "Выбор инструментов", style | BS_GROUPBOX, 600, 320, 370, 70, hWnd, (HMENU)0, GetModuleHandle(0), 0);
        CreateToolTip(child, "Втравляет выбраню фигуру в точку последнего нажатия");

        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 610, 350, 32, 32, hWnd, (HMENU)IDC_SELECT, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "select.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE));
        CreateToolTip(child, "Позволяет выделять фигуры левой кнопкой мыши");

        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 650, 350, 32, 32, hWnd, (HMENU)IDC_COMBINE, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "combine.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE));
        CreateToolTip(child, "Позволяет выделять фигуры левой кнопкой мыши и объединить их при помощи правой кнопки мыши");

        child = CreateWindowEx(0, "button", "", style | BS_BITMAP, 690, 350, 32, 32, hWnd, (HMENU)IDC_UPDATE, GetModuleHandle(0), 0);
        SendMessage(child, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "update.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE));
        CreateToolTip(child, "Позволяет применить текущий стиль к фигуре");



    }

    static void Open(WNDPROC wndproc) {
        WNDCLASS wc{};
        wc.style = 0;
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

        auto hWnd = CreateWindow("Window", "Regions Manager", WS_OVERLAPPED | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 640, 0, 0, GetModuleHandle(0), 0);
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
};