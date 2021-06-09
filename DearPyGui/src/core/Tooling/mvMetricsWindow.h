#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvMetricsWindow : public mvToolWindow
    {

    public:

        mvMetricsWindow();

        mvUUID getUUID() const override { return MV_TOOL_METRICS_UUID; }
        const char* getTitle() const override { return "Metrics"; }

    protected:

        void drawWidgets() override;

    };

}
