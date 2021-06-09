#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvStyleWindow : public mvToolWindow
    {

    public:

        mvStyleWindow();

        mvUUID getUUID() const override { return MV_TOOL_STYLE_UUID; }
        const char* getTitle() const override { return "Style Editor"; }

    protected:

        void drawWidgets() override;

    };

}
