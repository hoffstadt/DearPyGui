#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

namespace Marvel {

    MV_REGISTER_WIDGET(mvColorMapScale, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 0);
    class mvColorMapScale : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapScale, add_colormap_scale)

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS

    public:

        mvColorMapScale(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    private:

        double m_scale_min = 0;
        double m_scale_max = 0;

    };

}