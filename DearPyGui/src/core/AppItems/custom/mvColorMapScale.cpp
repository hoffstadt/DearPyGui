#include "mvColorMapScale.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include <implot.h>

namespace Marvel {

    void mvColorMapScale::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("callback");
        parser.removeArg("callback_data");
        parser.removeArg("enabled");

        parser.addArg<mvPyDataType::Integer>("default_value", mvArgType::KEYWORD, "0");

        parser.addArg<mvPyDataType::Float>("min_scale", mvArgType::KEYWORD, "0.0");
        parser.addArg<mvPyDataType::Float>("max_scale", mvArgType::KEYWORD, "1.0");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvColorMapScale::mvColorMapScale(const std::string& name)
        : mvIntPtrBase(name)
    {
    }

    void mvColorMapScale::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        ImPlot::ColormapScale(m_label.c_str(), m_scale_min, m_scale_max, ImVec2((float)m_width, (float)m_height), *m_value);
    }

    void mvColorMapScale::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_scale")) m_scale_min = (double)ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_scale")) m_scale_max = (double)ToFloat(item);
    }

    void mvColorMapScale::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

    }

}
