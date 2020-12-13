#pragma once
#include <Windows.h>
#include <vector>
#include <random>

#include "Static.h"


class Region {

    HRGN hRegion;
    INT FrameWidth, FrameColor;
    INT FillColor;

    HBRUSH brushFill;
    HBRUSH brushFrame;

public:
    Region(HRGN hRegion) {
        Pen(1, 0x00FF00);
        Brush(0x00FF00);
        this->hRegion = hRegion;
    }

    Region(std::vector<POINT> points) {
        Pen(1, 0x00FF00);
        Brush(0x00FF00);
        hRegion = CreatePolygonRgn(points.data(), points.size(), WINDING);
    }

    ~Region() {
        DeleteRgn(hRegion);
    }

    void Pen(INT width, INT color) {
        FrameWidth = width, FrameColor = color;
        DeleteBrush(brushFrame);
        brushFrame = CreateSolidBrush(FrameColor);
    }

    void Brush(INT color) {
        FillColor = color;
        DeleteBrush(brushFill);
        brushFill = CreateSolidBrush(FillColor);
    }

    void Offset(INT x, INT y) {
        OffsetRgn(hRegion, x, y);
    }

    void Resize(DOUBLE x, DOUBLE y) {
        XFORM xf{};
        xf.eM11 = x;
        xf.eM22 = y;
        auto it = GetRegionData(hRegion, 0, 0);
        auto lpRgnData = (LPRGNDATA)GlobalAlloc(GMEM_FIXED, sizeof(RGNDATA) * it);
        GetRegionData(hRegion, it, lpRgnData);
        if(auto region = ExtCreateRegion(&xf, it, lpRgnData)) {
            DeleteRgn(hRegion);
            hRegion = region;
            auto w = lpRgnData->rdh.rcBound.left;
            auto h = lpRgnData->rdh.rcBound.top;
            Offset((w - w * x), (h - h * y));
        }
        GlobalFree(lpRgnData);
    }

    void Clear(HDC hDC) {
        SelectObject(hDC, GetSysColorBrush(WHITE_BRUSH));
        PaintRgn(hDC, hRegion);
    }

    void Fill(HDC hDC) {
        SelectObject(hDC, brushFill);
        PaintRgn(hDC, hRegion);
        FrameRgn(hDC, hRegion, brushFrame, FrameWidth, FrameWidth);
    }

    void Select(HDC hDC) {
        SelectObject(hDC, GetSysColorBrush(DKGRAY_BRUSH));
        PaintRgn(hDC, hRegion);
        FrameRgn(hDC, hRegion, brushFrame, FrameWidth, FrameWidth);
    }

    static std::shared_ptr<Region> Combine(const std::vector<std::shared_ptr<Region>> & regions) {
        auto combined = CreateRectRgn(0, 0, 0, 0);
        for(auto & region : regions) {
            CombineRgn(combined, combined, *region, RGN_OR);
        }
        return std::shared_ptr<Region>(new Region(combined));
    }

    static std::shared_ptr<Region> Search(std::vector<std::shared_ptr<Region>> & regions, POINT point) {
        std::shared_ptr<Region> result = 0;
        for(auto & region : regions)
            if(PtInRegion(*region, point.x, point.y))
                result = region;
        return result;
    }

    operator bool() const {
        return hRegion != 0;
    }

    operator HRGN() const {
        return hRegion;
    }

};