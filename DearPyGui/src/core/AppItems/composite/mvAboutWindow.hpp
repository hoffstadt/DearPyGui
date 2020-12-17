// to be sure to have the best speed from compiler
// without having to check the behavior of each compiler
// is better to use both pragma once and guards
// in that way can get the best of both (cross-platform and help compilation speed)

#ifndef mvAboutWindow_hpp           // guards
#define mvAboutWindow_hpp           // guards
#pragma once
#include "mvTypeBases.h"
#include "mvMarvel.h"

namespace Marvel {

    class mvAboutWindow : public mvBaseWindowAppitem
    {
        
        MV_APPITEM_TYPE(mvAppItemType::AboutWindow, "add_about_window")
        
    public:
        
        mvAboutWindow(const std::string& name)
            : mvBaseWindowAppitem(name);
        
        void draw() override;
    };

}

#endif /* mvAboutWindow_hpp */      // guards
