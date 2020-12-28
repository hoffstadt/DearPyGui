#pragma once
#include "mvTypeBases.h"
#include "mvMarvel.h"

namespace Marvel {

    class mvAboutWindow : public mvBaseWindowAppitem
    {
        
        MV_APPITEM_TYPE(mvAppItemType::AboutWindow, "add_about_window")
        
    public:
        
        mvAboutWindow(const std::string& name);
        
        void draw() override;
    };

}
