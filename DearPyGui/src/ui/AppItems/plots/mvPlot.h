#pragma once

#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvItemRegistry.h"
#include "mvCore.h"

namespace Marvel {

    class mvPlot : public mvAppItem
    {

    public:

        explicit mvPlot(mvUUID uuid);

        void updateFlags();
        void updateAxesNames();

        // settings
        void SetColorMap    (ImPlotColormap colormap);
        void draw           (ImDrawList* drawlist, float x, float y) override;

        void addFlag         (ImPlotFlags flag);
        void removeFlag      (ImPlotFlags flag);

        [[nodiscard]] bool isPlotQueried() const;
        double* getPlotQueryArea();
        
        ImPlotFlags        getFlags         () const { return _flags; }

        void onChildRemoved(mvRef<mvAppItem> item);
        void onChildAdd(mvRef<mvAppItem> item);
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        std::string     _xaxisName;
        std::string     _y1axisName;
        std::string     _y2axisName;
        std::string     _y3axisName;
        ImPlotFlags     _flags    = ImPlotFlags_NoLegend;
        ImPlotAxisFlags _xflags  = 0;
        ImPlotAxisFlags _yflags  = 0;
        ImPlotAxisFlags _y1flags  = 0;
        ImPlotAxisFlags _y2flags  = 0;              
        bool            _newColorMap = false; // to bust color cache
        bool            _useColorMap = false;
        ImPlotColormap  _colormap = ImPlotColormap_Deep;                
        bool            _equalAspectRatios = false;             
        bool            _queried = false;
        double          _queryArea[4] = {0.0, 0.0, 0.0, 0.0};
        bool            _fitDirty = false;
        bool            _axisfitDirty[4] = { false, false, false, false };

        friend class mvPlotAxis;

        // custom input mapping
        ImPlotInputMap _originalMap = ImPlotInputMap();

        int _pan_button = 0;
        int _pan_mod = 0;
        int _fit_button = 0;
        int _context_menu_button = 0;
        int _box_select_button = 0;
        int _box_select_mod = 0;
        int _box_select_cancel_button = 0;
        int _query_button = 0;
        int _query_mod = 0;
        int _query_toggle_mod = 0;
        int _horizontal_mod = 0;
        int _vertical_mod = 0;
    
    };

}
