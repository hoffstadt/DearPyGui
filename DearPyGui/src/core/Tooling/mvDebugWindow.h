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

        void renderItem(mvAppItem* item);

        std::string m_selectedItem;
        ImGuiID     m_selectedID = 0u;
        std::vector<std::pair<std::string, std::string>> m_commands;

    };

}
