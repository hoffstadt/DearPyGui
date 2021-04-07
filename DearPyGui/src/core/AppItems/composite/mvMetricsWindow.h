#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include <string>
#include "mvModule_Core.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvMetricsWindow, MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
    class mvMetricsWindow : public mvBaseWindowAppitem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMetricsWindow, add_metrics_window)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS
     
    public:
        
        mvMetricsWindow(const std::string& name);
        
        void draw(ImDrawList* drawlist, float x, float y) override;
    };

}
