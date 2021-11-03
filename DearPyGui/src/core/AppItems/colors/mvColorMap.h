#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>

namespace Marvel {

    MV_REGISTER_WIDGET(mvColorMap, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
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

        //-----------------------------------------------------------------------------
        // Built-in ImPlot Color maps
        //-----------------------------------------------------------------------------
        MV_CREATE_CONSTANT(mvPlotColormap_Default, 0L); // ImPlot default colormap         (n=10)
        MV_CREATE_CONSTANT(mvPlotColormap_Deep, 0L); // ImPlot default colormap         (n=10)
        MV_CREATE_CONSTANT(mvPlotColormap_Dark, 1L); // a.k.a. matplotlib "Set1"        (n=9)
        MV_CREATE_CONSTANT(mvPlotColormap_Pastel, 2L); // a.k.a. matplotlib "Pastel1"     (n=9)
        MV_CREATE_CONSTANT(mvPlotColormap_Paired, 3L); // a.k.a. matplotlib "Paired"      (n=12)
        MV_CREATE_CONSTANT(mvPlotColormap_Viridis, 4L); // a.k.a. matplotlib "viridis"     (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Plasma, 5L); // a.k.a. matplotlib "plasma"      (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Hot, 6L); // a.k.a. matplotlib/MATLAB "hot"  (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Cool, 7L); // a.k.a. matplotlib/MATLAB "cool" (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Pink, 8L); // a.k.a. matplotlib/MATLAB "pink" (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Jet, 9L); // a.k.a. MATLAB "jet"             (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Twilight, 10L); // a.k.a. MATLAB "jet"             (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_RdBu, 11L); // a.k.a. MATLAB "jet"             (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_BrBG, 12L); // a.k.a. MATLAB "jet"             (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_PiYG, 13L); // a.k.a. MATLAB "jet"             (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Spectral, 14L); // a.k.a. MATLAB "jet"             (n=11)
        MV_CREATE_CONSTANT(mvPlotColormap_Greys, 15L); // a.k.a. MATLAB "jet"             (n=11)

        MV_CREATE_COMMAND(bind_colormap);
        MV_CREATE_COMMAND(sample_colormap);
        MV_CREATE_COMMAND(get_colormap_color);

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_COMMANDS
            MV_ADD_COMMAND(bind_colormap);
            MV_ADD_COMMAND(sample_colormap);
            MV_ADD_COMMAND(get_colormap_color);
        MV_END_COMMANDS

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvPlotColormap_Default),
            MV_ADD_CONSTANT(mvPlotColormap_Deep),
            MV_ADD_CONSTANT(mvPlotColormap_Dark),
            MV_ADD_CONSTANT(mvPlotColormap_Pastel),
            MV_ADD_CONSTANT(mvPlotColormap_Paired),
            MV_ADD_CONSTANT(mvPlotColormap_Viridis),
            MV_ADD_CONSTANT(mvPlotColormap_Plasma),
            MV_ADD_CONSTANT(mvPlotColormap_Hot),
            MV_ADD_CONSTANT(mvPlotColormap_Cool),
            MV_ADD_CONSTANT(mvPlotColormap_Pink),
            MV_ADD_CONSTANT(mvPlotColormap_Jet),
            MV_ADD_CONSTANT(mvPlotColormap_Twilight),
            MV_ADD_CONSTANT(mvPlotColormap_RdBu),
            MV_ADD_CONSTANT(mvPlotColormap_BrBG),
            MV_ADD_CONSTANT(mvPlotColormap_PiYG),
            MV_ADD_CONSTANT(mvPlotColormap_Spectral),
            MV_ADD_CONSTANT(mvPlotColormap_Greys)
        MV_END_CONSTANTS

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
