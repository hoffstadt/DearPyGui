#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvAppItem;

    class mvLayoutWindow : public mvToolWindow
    {

    public:

        mvLayoutWindow();

        const char* getName() const override { return "mvLayoutWindow"; }
        const char* getTitle() const override { return "App Layout"; }

    protected:

        void drawWidgets() override;

    private:

        void renderNode(mvAppItem* item);

        std::string m_selectedItem;
        bool m_showLabels = false;

    };

}
