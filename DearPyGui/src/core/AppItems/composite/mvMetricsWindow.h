#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include <string>
#include "mvMarvel.h"

namespace Marvel {

    class mvMetricsWindow : public mvBaseWindowAppitem
    {

        MV_APPITEM_TYPE(mvAppItemType::MetricsWindow, mvMetricsWindow, "add_metrics_window")

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS
        
    public:
        
        mvMetricsWindow(const std::string& name);
        
        void draw() override;
    };

}
