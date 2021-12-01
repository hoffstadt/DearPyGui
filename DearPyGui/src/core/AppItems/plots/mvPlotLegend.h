#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvPlotLegend : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotLegend, add_plot_legend)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvSubPlots)
        MV_END_PARENTS

    public:

        explicit mvPlotLegend(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        void getSpecificConfiguration(PyObject* dict) override;

    public:

        ImPlotLocation _legendLocation = ImPlotLocation_NorthWest;
        bool           _horizontal = false;
        bool           _outside = false;
        bool           _dirty = true;
    };

}
