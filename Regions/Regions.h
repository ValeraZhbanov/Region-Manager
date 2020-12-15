#pragma once

#include <vector>
#include <set>

#include <memory>

#include "Region.h"
#include "Gui.h"


class MRegions {
    Main * window;

public:

    bool DrawAll = true;
    std::vector<std::shared_ptr<Region>> Regions;
    std::vector<std::shared_ptr<Region>> SelectedRegions;

    MRegions(Main * window = 0) : window(window) {}

    void Add(Region * region, bool set_default = true) {
        Regions.push_back(std::shared_ptr<Region>(region));
        if(set_default) {
            Regions.back()->SetPen(window->StaticBox.Pen);
            Regions.back()->SetBrush(window->StaticBox.Brush);
        }
        if(DrawAll) Regions.back()->Fill(window->StaticBox);
        Paint();
    }

    void SetPen(Pen pen) {
        for(auto & region : SelectedRegions) {
            region->SetPen(pen);
            region->Select(window->StaticBox);
        }
    }

    void SetBrush(Brush brush) {
        for(auto & region : SelectedRegions) {
            region->SetBrush(brush);
            region->Select(window->StaticBox);
        }
    }

    void Select(POINT point) {
        if(DrawAll)
            if(auto region = Region::Search(Regions, point)) {
                SelectedRegions.push_back(region);
                Regions.erase(std::find(Regions.begin(), Regions.end(), region));
                region->Select(window->StaticBox);
                Paint();
                return;
            }
        if(auto region = Region::Search(SelectedRegions, point)) {
            Regions.push_back(region);
            SelectedRegions.erase(std::find(SelectedRegions.begin(), SelectedRegions.end(), region));
            region->Clear(window->StaticBox);
            if(DrawAll) region->Fill(window->StaticBox);
            Paint();
        }
    }

    void Redraw(POINT point) {
        if(auto region = Region::Search(Regions, point)) {
            region->SetPen(window->StaticBox.Pen);
            region->SetBrush(window->StaticBox.Brush);
            if(DrawAll) region->Fill(window->StaticBox);
            Paint();
        }
    }

    void Offset(INT dx, INT dy) {
        for(auto & region : Regions)
            if(DrawAll) region->Fill(window->StaticBox);
        for(auto & region : SelectedRegions) {
            region->Clear(window->StaticBox);
            region->Offset(dx, dy);
            region->Select(window->StaticBox);
        }
        Paint();
    }

    void Paint() {
        window->StaticBox.Paint();
    }

    void Delete() {
        for(auto & region : SelectedRegions)
            region->Clear(window->StaticBox);
        for(auto & region : Regions)
            if(DrawAll) region->Fill(window->StaticBox);
        SelectedRegions.clear();
        Paint();
    }

    void Resize(DOUBLE x, DOUBLE y) {
        for(auto & region : SelectedRegions) {
            region->Clear(window->StaticBox);
            region->Resize(x, y);
            for(auto & region : Regions)
                if(DrawAll) region->Fill(window->StaticBox);
            for(auto & region : SelectedRegions)
                region->Select(window->StaticBox);
        }
        Paint();
    }

    void SelectAll() {
        SelectedRegions.insert(SelectedRegions.end(), Regions.begin(), Regions.end());
        Regions.clear();
        for(auto & region : SelectedRegions) {
            region->Clear(window->StaticBox);
            region->Select(window->StaticBox);
        }
        Paint();
    }

    void UnselectAll() {
        Regions.insert(Regions.end(), SelectedRegions.begin(), SelectedRegions.end());
        SelectedRegions.clear();
        for(auto & region : Regions) {
            region->Clear(window->StaticBox);
            if(DrawAll) region->Fill(window->StaticBox);
        }
        Paint();
    }

    void SelectOrUnselectAll() {
        if(Regions.size()) {
            SelectAll();
        } else {
            UnselectAll();
        }
    }

    void UpdateAll() {
        for(auto & region : Regions) {
            region->Clear(window->StaticBox);
            if(DrawAll) region->Fill(window->StaticBox);
        }
        for(auto & region : SelectedRegions) {
            region->Select(window->StaticBox);
        }
        Paint();
    }

    void CopyPaste() {
        for(auto it = 0; it < SelectedRegions.size(); ++it) {
            auto region = std::shared_ptr<Region>(SelectedRegions[it]->Copy());
            Regions.push_back(region);
            region->Offset(30, 30);
            if(DrawAll) region->Fill(window->StaticBox);
        }
        Paint();
    }

    void Clear() {
        for(auto & region : SelectedRegions) {
            region->Clear(window->StaticBox);
        }
        for(auto & region : Regions) {
            region->Clear(window->StaticBox);
        }
        SelectedRegions.clear();
        Regions.clear();
        Paint();
    }
};
