#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvPlotAxis : public mvAppItem
    {

    public:

        explicit mvPlotAxis(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void onChildRemoved(mvRef<mvAppItem> item);
        void onChildAdd(mvRef<mvAppItem> item);
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
