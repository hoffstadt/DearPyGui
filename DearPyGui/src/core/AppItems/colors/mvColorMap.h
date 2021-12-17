#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>

namespace Marvel {

    class mvColorMap : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMap, add_colormap)
 
    public:

        explicit mvColorMap(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void applySpecificTemplate(mvAppItem* item) override;
        ImPlotColormap getColorMap() const { return _colorMap; }

    public:

        ImPlotColormap      _colorMap = -1;
        bool                _qualitative = true;
        std::vector<ImVec4> _colors;
    };

}
