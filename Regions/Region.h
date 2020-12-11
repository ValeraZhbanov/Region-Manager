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
        transform(xf);
    }

    void FillRegion(HDC hDC) {
        SelectObject(hDC, brushFill);
        PaintRgn(hDC, hRegion);
        FrameRgn(hDC, hRegion, brushFrame, FrameWidth, FrameWidth);
    }

    void SelectRegion(HDC hDC) {
        static auto brushSelect = CreateHatchBrush(HS_DIAGCROSS, 0);
        SelectObject(hDC, brushSelect);
        PaintRgn(hDC, hRegion);
        FrameRgn(hDC, hRegion, brushFrame, FrameWidth, FrameWidth);
    }

    static Region * CombineRegion(const std::set<Region *> & regions) {
        auto combined = CreateRectRgn(0, 0, 0, 0);
        for(auto & region : regions) {
            CombineRgn(combined, combined, *region, RGN_OR);
        }
        return new Region(combined);
    }

    static Region * SearchRegionIndex(std::vector<Region *> & regions, POINT point) {
        Region * result = 0;
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

private:
    void transform(XFORM xf) {
        auto it = GetRegionData(hRegion, 0, NULL);
        auto lpRgnData = (LPRGNDATA)GlobalAlloc(GMEM_FIXED, sizeof(RGNDATA) * it);
        GetRegionData(hRegion, it, lpRgnData);
        if(auto region = ExtCreateRegion(&xf, it, lpRgnData)) {
            DeleteRgn(hRegion);
            hRegion = region;
        }
    }
};