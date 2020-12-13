#pragma once
#include <Windows.h>



class Static {
    HWND hWnd = {};
    HDC hDC = {};

public:
    RECT RT = {};
    INT Width = 1;
    INT ColorPen = 0x000000;
    INT ColorBrush = 0x00FF00;

    Static() {}
    Static(HWND hWnd) : hWnd(hWnd) {
        GetClientRect(hWnd, &RT);
        hDC = GetDC(hWnd);
        FillRect(hDC, &RT, GetSysColorBrush(WHITE_BRUSH));
    }
    ~Static() {}

    void Paint() {
        FillRect(hDC, &RT, GetSysColorBrush(WHITE_BRUSH));
    }
    operator HWND() {
        return hWnd;
    }

    operator HDC() {
        return hDC;
    }
};


class Area {
    HWND hWnd = {};

public:
    Area() {}
    Area(HWND hWnd) : hWnd(hWnd) {}
    void Paint(INT color) {
        RECT RT;
        auto hDC = GetDC(hWnd);
        auto hBrush = CreateSolidBrush(color);
        GetClientRect(hWnd, &RT);
        FillRect(hDC, &RT, hBrush);
    }

    operator HWND() {
        return hWnd;
    }
};


class ChooseColorDialog {
    CHOOSECOLOR cc{};
    COLORREF acrCustClr[16]{};

public:
    ChooseColorDialog(HWND hWnd) {
        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hWnd;
        cc.lpCustColors = acrCustClr;
    }

    INT Show() {
        ChooseColor(&cc);
        return cc.rgbResult;
    }
};