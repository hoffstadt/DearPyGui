#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvStyleWindow : public mvToolWindow
    {

    public:

        mvStyleWindow();

        const char* getName() const override { return "mvStyleWindow"; }
        const char* getTitle() const override { return "Style Editor"; }

    protected:

        void drawWidgets() override;

    };

}
