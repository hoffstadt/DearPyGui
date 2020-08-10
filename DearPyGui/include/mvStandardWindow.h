#pragma once

#include <imgui.h>
#include <map>
#include <string>
#include "mvEventHandler.h"

namespace Marvel {

    class mvStandardWindow : public mvEventHandler
    {

        struct StandardWindowEntry
        {
            mvStandardWindow* window;
            bool              show;
        };

    public:

        mvStandardWindow() : mvEventHandler() {}

        virtual ~mvStandardWindow() = default;

        virtual void prerender();
        virtual void render(bool& show) = 0;
        virtual void postrender() {}

        void                   showStandardWindow(const std::string& name);
        void                   addStandardWindow (const std::string& name, mvStandardWindow* window);
        mvStandardWindow*      getStandardWindow (const std::string& name);
        void                   setToMainMode     ();
        void                   setSize           (unsigned width, unsigned height);
        [[nodiscard]] unsigned getWindowWidth    () const;
        [[nodiscard]] unsigned getWindowHeight   () const;

    protected:

        bool             m_mainMode = false;
        unsigned         m_width = 1280;
        unsigned         m_height = 800;
        ImGuiWindowFlags m_flags = ImGuiWindowFlags_NoSavedSettings;

        std::map<std::string, StandardWindowEntry> m_standardWindows;

    };

}