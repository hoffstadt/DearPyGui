#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvMetricsWindow : public mvToolWindow
    {

    public:

        mvMetricsWindow();

        const char* getName() const override { return "mvMetricsWindow"; }
        const char* getTitle() const override { return "Metrics"; }

    protected:

        void drawWidgets() override;

    };

}
