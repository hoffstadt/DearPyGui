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
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Integer, "default_value", "", "0"},
            {mvPythonDataType::Float, "min_scale", "", "0.0"},
            {mvPythonDataType::Float, "max_scale", "", "1.0"},
            {mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Integer, "height","", "0"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Adds drag for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
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

        ImPlot::ColormapScale(m_label.c_str(), m_scale_min, m_scale_max, ImVec2(m_width, m_height), *m_value);
    }

    void mvColorMapScale::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_scale")) m_scale_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_scale")) m_scale_max = ToFloat(item);
    }

    void mvColorMapScale::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

    }

}
