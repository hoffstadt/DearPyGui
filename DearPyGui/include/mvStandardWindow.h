#pragma once

#include <imgui.h>
#include <map>
#include <string>
#include "mvEventHandler.h"
#include "mvCore.h"

namespace Marvel {

    class mvStandardWindow : public mvEventHandler
    {

        struct StandardWindowEntry
        {
            mvStandardWindow* window;
            bool              show;
        };

    public:

        mvStandardWindow(const char* name) : mvEventHandler(), m_name(name){}

        virtual ~mvStandardWindow() = default;

        virtual bool prerender(bool& show);
        virtual void render(bool& show) = 0;
        virtual void postrender() {}

        void         setConfigDict(PyObject* dict);
        void         getConfigDict(PyObject* dict);

        void                   showStandardWindow(const std::string& name);
        void                   addStandardWindow (const std::string& name, mvStandardWindow* window);
        mvStandardWindow*      getStandardWindow (const std::string& name);
        void                   setToMainMode     ();
        void                   setSize           (unsigned width, unsigned height);
        [[nodiscard]] unsigned getWindowWidth    () const;
        [[nodiscard]] unsigned getWindowHeight   () const;

        void setWindowPos(float x, float y)
        {
            m_xpos = (int)x;
            m_ypos = (int)y;
            m_dirty = true;
        }

        [[nodiscard]] mvVec2 getWindowPos() const
        {
            return { (float)m_xpos, (float)m_ypos };
        }

        void setWidth(int width) { m_width = width; m_dirty = true; }
        void setHeight(int height) { m_height = height; m_dirty = true; }

    protected:

        bool             m_mainMode = false;
        unsigned         m_width = 500;
        unsigned         m_height = 500;
        int              m_xpos = 200;
        int              m_ypos = 200;
        bool             m_dirty = true;
        ImGuiWindowFlags m_flags = ImGuiWindowFlags_NoSavedSettings;
        std::string      m_name;

        std::map<std::string, StandardWindowEntry> m_standardWindows;

    };

}