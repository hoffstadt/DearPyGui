#pragma once

#include "mvToolWindow.h"
#include "mvAppItem.h"

namespace Marvel {

    class mvDebugWindow : public mvToolWindow
    {

    public:

        mvDebugWindow();

        mvUUID getUUID() const override { return MV_TOOL_DEBUG_UUID; }
        const char* getTitle() const override { return "Dear PyGui Debug"; }

    protected:

        void drawWidgets() override;

    private:

        std::vector<std::pair<std::string, std::string>> m_commands;

    };

}
