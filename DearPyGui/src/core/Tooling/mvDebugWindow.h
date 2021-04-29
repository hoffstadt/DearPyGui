#pragma once

#include "mvToolWindow.h"
#include "mvAppItem.h"

namespace Marvel {

    class mvDebugWindow : public mvToolWindow
    {

    public:

        mvDebugWindow();

        const char* getName() const override { return "mvDebugWindow"; }
        const char* getTitle() const override { return "Dear PyGui Debug"; }

    protected:

        void drawWidgets() override;

    private:

        std::vector<std::pair<std::string, std::string>> m_commands;

    };

}
