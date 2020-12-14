#pragma once
#include <Windows.h>


HBRUSH BrushSelect = CreateSolidBrush(0xAAAAAA);
HPEN PenSelect = CreatePen(0, 11, 0xAAAAAA);

class Static {
    HWND hWnd = {};
    HDC hDC = {};
    HDC hDCM = {};

public:
    RECT RT = {};
    INT Width = 1;
    INT ColorPen = 0x000000;
    INT ColorBrush = 0x00FF00;

    Static() {}
    Static(HWND hWnd) : hWnd(hWnd) {
        GetClientRect(hWnd, &RT);
        hDC = GetDC(hWnd);
        hDCM = CreateCompatibleDC(hDC);
        SelectObject(hDCM, CreateCompatibleBitmap(hDC, RT.right, RT.bottom));

        FillRect(hDCM, &RT, BrushSelect);

    }
    ~Static() {}

    void Paint() {
        BitBlt(hDC, RT.left, RT.top, RT.right, RT.bottom, hDCM, 0, 0, SRCCOPY);
    }
    operator HWND() {
        return hWnd;
    }

    operator HDC() {
        return hDCM;
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