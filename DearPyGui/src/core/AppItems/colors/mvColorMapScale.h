#pragma once

#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>

namespace Marvel {

    MV_REGISTER_WIDGET(mvColorMapScale, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
    class mvColorMapScale : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapScale, add_colormap_scale)

        MV_START_COMMANDS
        MV_END_COMMANDS

        MV_START_CONSTANTS
        MV_END_CONSTANTS

    public:

        mvColorMapScale(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void setColorMap(ImPlotColormap colormap);

    private:

        double          _scale_min = 0;
        double          _scale_max = 0;
        ImPlotColormap  _colormap = 0;

    };

}