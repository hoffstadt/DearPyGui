#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvAboutWindow : public mvToolWindow
    {

    public:

        mvAboutWindow();

        const char* getName() const override { return "mvAboutWindow"; }
        const char* getTitle() const override { return "About Dear PyGui"; }

    protected:

        void drawWidgets() override;

    };

}
