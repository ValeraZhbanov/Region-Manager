#pragma once
#include <Windows.h>
#include <vector>
#include <random>

#include "Static.h"


class Region {

protected:
    Pen _pen;
    Brush _brush;

    HBRUSH BrushSelect = CreateSolidBrush(0xAAAAAA);
    HPEN PenSelect = CreatePen(0, 11, 0xAAAAAA);

    HBRUSH Brush;
    HPEN Pen;

public:

    virtual void SetPen(const ::Pen & pen) {
        _pen = pen;
        DeletePen(Pen);
        DeletePen(PenSelect);
        Pen = CreatePen(pen.Style, pen.Width, pen.Color);
        PenSelect = CreatePen(0, pen.Width + 1, 0xAAAAAA);
    }

    virtual void SetBrush(const ::Brush & brush) {
        _brush = brush;
        DeleteBrush(Brush);
        if(brush.Style == 0) {
            Brush = CreateSolidBrush(brush.Color);
        } else {
            Brush = CreateHatchBrush(brush.Style - 1, brush.Color);
        }
    }

    virtual Region * Copy() = 0;
    virtual void Offset(INT x, INT y) = 0;
    virtual void Resize(DOUBLE x, DOUBLE y) = 0;
    virtual void Clear(HDC hDC) = 0;
    virtual void Fill(HDC hDC) = 0;
    virtual void Select(HDC hDC) = 0;
    virtual bool InRegion(POINT point) = 0;

    static std::shared_ptr<Region> Search(std::vector<std::shared_ptr<Region>> & Regions, POINT point) {
        std::shared_ptr<Region> result = 0;
        for(auto & region : Regions)
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

    virtual Region * Copy() {
        FRectangle * result = new FRectangle(RT);
        result->SetPen(_pen);
        result->SetBrush(_brush);
        return result;
    }

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
        SelectObject(hDC, Pen);
        SelectObject(hDC, Brush);
        Rectangle(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Select(HDC hDC) {
        SelectObject(hDC, Pen);
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

    virtual Region * Copy() {
        FEllipse * result = new FEllipse(RT);
        result->SetPen(_pen);
        result->SetBrush(_brush);
        return result;
    }

    virtual void Clear(HDC hDC) {
        SelectObject(hDC, PenSelect);
        SelectObject(hDC, BrushSelect);
        Ellipse(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Fill(HDC hDC) {
        SelectObject(hDC, Pen);
        SelectObject(hDC, Brush);
        Ellipse(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

    virtual void Select(HDC hDC) {
        SelectObject(hDC, Pen);
        SelectObject(hDC, GetSysColorBrush(DKGRAY_BRUSH));
        Ellipse(hDC, RT.left, RT.top, RT.right, RT.bottom);
    }

};

class FPolygon : public Region {
    std::vector<POINT> points;

public:
    FPolygon(const std::vector<POINT> & points = {}) : points(points) {}

    virtual Region * Copy() {
        FPolygon * result = new FPolygon(points);
        result->SetPen(_pen);
        result->SetBrush(_brush);
        return result;
    }

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
        SelectObject(hDC, Pen);
        SelectObject(hDC, Brush);
        Polygon(hDC, points.data(), points.size());
    }

    virtual void Select(HDC hDC) {
        SelectObject(hDC, Pen);
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
    FMulti(std::vector<std::shared_ptr<Region>> & Regions) : Region(), Regions(Regions) {}

    virtual void SetPen(const ::Pen & pen) {
        for(auto & region : Regions)
            region->SetPen(pen);
    }

    virtual void SetBrush(const ::Brush & brush) {
        for(auto & region : Regions)
            region->SetBrush(brush);
    }

    virtual Region * Copy() {
        auto regions = Regions;
        for(auto it = 0; it < Regions.size(); ++it) {
            regions[it] = std::shared_ptr<Region>(Regions[it]->Copy());
        }
        return new FMulti(regions);
    }

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