#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvAboutWindow final : public mvToolWindow
    {

    public:

        mvAboutWindow();

        [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_ABOUT_UUID; }
        [[nodiscard]] const char* getTitle() const override { return "About Dear PyGui"; }

    protected:

        void drawWidgets() override;

    };

}
