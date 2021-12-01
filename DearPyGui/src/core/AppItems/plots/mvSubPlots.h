#pragma once

#include "mvItemRegistry.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"

namespace Marvel {

    class mvSubPlots : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSubPlots, add_subplots)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvPlot),
            MV_ADD_CHILD(mvAppItemType::mvPlotLegend)
        MV_END_CHILDREN

    public:

        explicit mvSubPlots(mvUUID uuid);

        void draw (ImDrawList* drawlist, float x, float y) override;

        void onChildRemoved(mvRef<mvAppItem> item) override;
        void onChildAdd(mvRef<mvAppItem> item) override;

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

}
