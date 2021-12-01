#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvChildWindow : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvChildWindow, add_child_window)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvChildWindow(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        void addFlag(ImGuiWindowFlags flag);
        void removeFlag(ImGuiWindowFlags flag);

        void setScrollX(float value) { _scrollX = value; _scrollXSet = true; }
        void setScrollY(float value) { _scrollY = value; _scrollYSet = true; }
        float getScrollX() const { return _scrollX; }
        float getScrollY() const { return _scrollY; }
        float getScrollXMax() const { return _scrollMaxX; }
        float getScrollYMax() const { return _scrollMaxY; }

    private:

        bool             _border     = true;
        bool             _autosize_x = false;
        bool             _autosize_y = false;
        ImGuiWindowFlags _windowflags = ImGuiWindowFlags_NoSavedSettings;

        // scroll info
        float _scrollX = 0.0f;
        float _scrollY = 0.0f;
        float _scrollMaxX = 0.0f;
        float _scrollMaxY = 0.0f;
        bool  _scrollXSet = false;
        bool  _scrollYSet = false;

    };

}
