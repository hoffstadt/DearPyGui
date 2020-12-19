#pragma once

#include "mvTypeBases.h"
#include <string>
#include "mvMarvel.h"

namespace Marvel {

    class mvMetricsWindow : public mvBaseWindowAppitem
    {
        
        MV_APPITEM_TYPE(mvAppItemType::MetricsWindow, "add_metrics_window")
        
    public:
        
        mvMetricsWindow(const std::string& name)
            : mvBaseWindowAppitem(name);
        
        void draw() override;
    };

}
