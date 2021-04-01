#pragma once
#include "mvTypeBases.h"
#include "mvModule_Core.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvAboutWindow);
    class mvAboutWindow : public mvBaseWindowAppitem
    {

        MV_APPITEM_TYPE(mvAppItemType::mvAboutWindow, add_about_window)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
        
    public:
        
        mvAboutWindow(const std::string& name);
        
        void draw(ImDrawList* drawlist, float x, float y) override;
    };

}
