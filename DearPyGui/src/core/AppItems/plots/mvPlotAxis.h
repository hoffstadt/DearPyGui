#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvPlotAxis : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotAxis, add_plot_axis)
        MV_DEFAULT_CHILDREN

        MV_CREATE_COMMAND(reset_axis_ticks);
        MV_CREATE_COMMAND(set_axis_ticks);
        MV_CREATE_COMMAND(set_axis_limits);
        MV_CREATE_COMMAND(set_axis_limits_auto);
        MV_CREATE_COMMAND(get_axis_limits);
        MV_CREATE_COMMAND(fit_axis_data);

        

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvPlot)
        MV_END_PARENTS

        MV_START_COMMANDS
            MV_ADD_COMMAND(reset_axis_ticks);
            MV_ADD_COMMAND(set_axis_ticks);
            MV_ADD_COMMAND(set_axis_limits);
            MV_ADD_COMMAND(set_axis_limits_auto);
            MV_ADD_COMMAND(get_axis_limits);
            MV_ADD_COMMAND(fit_axis_data);
        MV_END_COMMANDS

    public:

        explicit mvPlotAxis(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void onChildRemoved(mvRef<mvAppItem> item) override;
        void onChildAdd(mvRef<mvAppItem> item) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        void setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations);
        void resetYTicks();
        void fitAxisData();

        void setLimits(float y_min, float y_max);
        void setLimitsAuto();

        ImPlotAxisFlags getFlags() const { return _flags; }
        const ImVec2& getYLimits() const { return _limits_actual; }

    private:

        ImPlotAxisFlags          _flags = 0;
        int                      _axis = 0;
        bool                     _setLimits = false;
        ImVec2                   _limits;
        ImVec2                   _limits_actual;
        std::vector<std::string> _labels;
        std::vector<double>      _labelLocations;
        std::vector<const char*> _clabels; // to prevent conversion from string to char* every frame
        bool                     _dirty = false;

    };

}
