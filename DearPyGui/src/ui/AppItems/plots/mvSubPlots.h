#pragma once

#include "mvItemRegistry.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"

class mvSubPlots : public mvAppItem
{

public:

    explicit mvSubPlots(mvUUID uuid);

    void draw (ImDrawList* drawlist, float x, float y) override;

    void onChildRemoved(mvRef<mvAppItem> item);
    void onChildAdd(mvRef<mvAppItem> item);

    void addFlag(ImPlotSubplotFlags flag);
    void removeFlag(ImPlotSubplotFlags flag);

    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

private:

    int _rows = 1;
    int _cols = 1;
    std::vector<float> _row_ratios;
    std::vector<float> _col_ratios;
    ImPlotSubplotFlags _flags = ImPlotSubplotFlags_None;

    
};