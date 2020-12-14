#pragma once
#include <Windows.h>
#include <vector>
#include <random>

#include "Static.h"


class Region {

protected:
    HBRUSH brush;
    HPEN pen;

public:
    Region() {
        Pen(1, 0x00FF00);
        Brush(0x00FF00);
    }

    void Pen(INT width, INT color) {
        DeleteBrush(pen);
        pen = CreatePen(0, width, color);
    }

    void Brush(INT color) {
        DeleteBrush(brush);
        brush = CreateSolidBrush(color);
    }

    virtual void Offset(INT x, INT y) = 0;
    virtual void Resize(DOUBLE x, DOUBLE y) = 0;
    virtual void Clear(HDC hDC) = 0;
    virtual void Fill(HDC hDC) = 0;
    virtual void Select(HDC hDC) = 0;
    virtual bool InRegion(POINT point) = 0;

    static std::shared_ptr<Region> Search(std::vector<std::shared_ptr<Region>> & regions, POINT point) {
        std::shared_ptr<Region> result = 0;
        for(auto & region : regions)
            if(region->InRegion(point))
                result = region;
        return result;
    }
};


class FRectangle : public Region {
protected:
    RECT RT;
public:
    FRectangle(const RECT & RT) : RT(RT) {}

    virtual void Offset(INT x, INT y) {
        OffsetRect(&RT, x, y);
    }

    virtual void Resize(DOUBLE x, DOUBLE y) {
        auto w = RT.left;
        auto h = RT.top;
        RT.left *= x, RT.right *= x;
        RT.top *= y, RT.bottom *= y;
        Offset(w - w * x, h - h * y);
    }

    virtual void Clear(HDC hDC) {
        SelectObject(hDC, PenSelect);
        SelectObject(hDC, BrushSelect);
        Rectangle(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Fill(HDC hDC) {
        SelectObject(hDC, pen);
        SelectObject(hDC, brush);
        Rectangle(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Select(HDC hDC) {
        SelectObject(hDC, pen);
        SelectObject(hDC, GetSysColorBrush(DKGRAY_BRUSH));
        Rectangle(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual bool InRegion(POINT point) {
        return PtInRect(&RT, point);
    }

};


class FEllipse : public FRectangle {
public:
    FEllipse(const RECT & RT) : FRectangle(RT) {}

    virtual void Clear(HDC hDC) {
        SelectObject(hDC, PenSelect);
        SelectObject(hDC, BrushSelect);
        Ellipse(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Fill(HDC hDC) {
        SelectObject(hDC, pen);
        SelectObject(hDC, brush);
        Ellipse(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Select(HDC hDC) {
        SelectObject(hDC, pen);
        SelectObject(hDC, GetSysColorBrush(DKGRAY_BRUSH));
        Ellipse(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

};

class FPolygon : public Region {
    std::vector<POINT> points;

public:
    FPolygon(const std::vector<POINT> & points = {}) : points(points) {}

    virtual void Offset(INT x, INT y) {
        for(auto & point : points) {
            point.x += x;
            point.y += y;
        }
    }
    virtual void Resize(DOUBLE x, DOUBLE y) {
        auto point = points.front();
        for(auto & point : points) {
            point.x *= x;
            point.y *= y;
        }
        Offset((point.x - point.x * x), (point.y - point.y * y));
    }

    virtual void Clear(HDC hDC) {
        SelectObject(hDC, PenSelect);
        SelectObject(hDC, BrushSelect);
        Polygon(hDC, points.data(), points.size());
    }

    virtual void Fill(HDC hDC) {
        SelectObject(hDC, pen);
        SelectObject(hDC, brush);
        Polygon(hDC, points.data(), points.size());
    }

    virtual void Select(HDC hDC) {
        SelectObject(hDC, pen);
        SelectObject(hDC, GetSysColorBrush(DKGRAY_BRUSH));
        Polygon(hDC, points.data(), points.size());
    }

    virtual bool InRegion(POINT point) {
        bool result = false;
        for(int i = 0; i < points.size() - 1; i++)
            if(point.x < points[i].x && point.y<points[i].y && point.y>points[i + 1].y
               || point.x<points[i].x && point.y>points[i].y && point.y < points[i + 1].y) result = !result;
        if(point.x < points.front().x && point.y<points.front().y && point.y>points.back().y
           || point.x<points.front().x && point.y>points.front().y && point.y < points.back().y) result = !result;
        return result;

    }
};

class FMulti : public Region {

    std::vector<std::shared_ptr<Region>> Regions;

public:
    FMulti(std::vector<std::shared_ptr<Region>> & regions) : Region(), Regions(regions) {}

    virtual void Offset(INT x, INT y) {
        for(auto & region : Regions)
            region->Offset(x, y);
    }

    virtual void Resize(DOUBLE x, DOUBLE y) {
        for(auto & region : Regions)
            region->Resize(x, y);
    }

    virtual void Clear(HDC hDC) {
        for(auto & region : Regions)
            region->Clear(hDC);
    }

    virtual void Fill(HDC hDC) {
        for(auto & region : Regions)
            region->Fill(hDC);
    }

    virtual void Select(HDC hDC) {
        for(auto & region : Regions)
            region->Select(hDC);
    }

    virtual bool InRegion(POINT point) {
        for(auto & region : Regions)
            if(region->InRegion(point))
                return true;
        return false;
    }
};