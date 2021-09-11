#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvStyleWindow final : public mvToolWindow
    {

    public:

        mvStyleWindow();

        [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_STYLE_UUID; }
        [[nodiscard]] const char* getTitle() const override { return "Style Editor (Default Theme)"; }

    protected:

        void drawWidgets() override;

    };

}
