#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvMetricsWindow final : public mvToolWindow
    {

    public:

        mvMetricsWindow();

        [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_METRICS_UUID; }
        [[nodiscard]] const char* getTitle() const override { return "Metrics"; }

    protected:

        void drawWidgets() override;

    };

}
