#pragma once
#include <Windows.h>



struct Pen {
    DWORD Style = 0;;
    DWORD Width = 1;
    DWORD Color = 0x00000000;
};
struct Brush {
    DWORD Color = 0x0000FF00;
    DWORD Style = 0;
};


class Static {
    HWND hWnd = {};
    HDC hDC = {};
    HDC hDCM = {};

    HBRUSH BrushSelect = CreateSolidBrush(0xAAAAAA);
    HPEN PenSelect = CreatePen(0, 11, 0xAAAAAA);

public:
    RECT RT = {};
    Pen Pen;
    Brush Brush;

    Static() {}
    Static(HWND hWnd) : hWnd(hWnd) {
        GetClientRect(hWnd, &RT);
        hDC = GetDC(hWnd);
        SetBkMode(hDC, TRANSPARENT);
        hDCM = CreateCompatibleDC(hDC);
        SetBkMode(hDCM, TRANSPARENT);
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