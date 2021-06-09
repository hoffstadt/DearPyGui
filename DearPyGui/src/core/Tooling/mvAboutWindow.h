#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvAboutWindow : public mvToolWindow
    {

    public:

        mvAboutWindow();

        mvUUID getUUID() const override { return MV_TOOL_ABOUT_UUID; }
        const char* getTitle() const override { return "About Dear PyGui"; }

    protected:

        void drawWidgets() override;

    };

}
