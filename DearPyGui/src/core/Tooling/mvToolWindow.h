#pragma once

#include <string>
#include <imgui.h>

namespace Marvel {

    class mvToolWindow
    {

        friend class mvToolManager;

    public:

        virtual const char* getName() const = 0;
        virtual const char* getTitle() const = 0;

        void draw();

        void setXPos  (int pos);
        void setYPos  (int pos);
        void setWidth (int width);
        void setHeight(int height);

    protected:

        virtual void drawWidgets() = 0;

    protected:

        ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;
        bool             m_show = false;
        bool             m_dirtySize = true;
        bool             m_dirtyPos = true;
        int              m_xpos = 200;
        int              m_ypos = 200;
        int              m_width = 500;
        int              m_height = 500;

    };

}
