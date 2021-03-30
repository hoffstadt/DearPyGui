#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include <string>
#include "mvModule_Core.h"

namespace Marvel {

#ifdef MV_CPP
#else
    PyObject* add_metrics_window(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

    MV_REGISTER_WIDGET(mvMetricsWindow);
    class mvMetricsWindow : public mvBaseWindowAppitem
    {

        MV_APPITEM_TYPE(mvAppItemType::mvMetricsWindow, "add_metrics_window")

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
        
    public:
        
        mvMetricsWindow(const std::string& name);
        
        void draw() override;
    };

}
