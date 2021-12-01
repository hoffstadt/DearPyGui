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
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvColorMapRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry)
        MV_END_PARENTS

        MV_CREATE_COMMAND(bind_colormap);
        MV_CREATE_COMMAND(sample_colormap);
        MV_CREATE_COMMAND(get_colormap_color);

        MV_START_COMMANDS
            MV_ADD_COMMAND(bind_colormap);
            MV_ADD_COMMAND(sample_colormap);
            MV_ADD_COMMAND(get_colormap_color);
        MV_END_COMMANDS

    public:

        explicit mvColorMap(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void applySpecificTemplate(mvAppItem* item) override;
        ImPlotColormap getColorMap() const { return _colorMap; }

    private:

        ImPlotColormap      _colorMap = -1;
        bool                _qualitative = true;
        std::vector<ImVec4> _colors;
    };

}
