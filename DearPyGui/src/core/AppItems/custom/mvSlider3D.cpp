#include "mvSlider3D.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include <dear_widgets.h>

namespace Marvel {

    void mvSlider3D::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("enabled");

        parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)");

        parser.addArg<mvPyDataType::Float>("max_x", mvArgType::KEYWORD_ARG, "100.0");
        parser.addArg<mvPyDataType::Float>("max_y", mvArgType::KEYWORD_ARG, "100.0");
        parser.addArg<mvPyDataType::Float>("max_z", mvArgType::KEYWORD_ARG, "100.0");
        parser.addArg<mvPyDataType::Float>("min_x", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("min_y", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("min_z", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("scale", mvArgType::KEYWORD_ARG, "1.0");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvSlider3D::mvSlider3D(const std::string& name)
        : mvFloat4PtrBase(name)
    {
    }

    void mvSlider3D::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if(ImWidgets::SliderScalar3D(m_specificedlabel.c_str(), &(*m_value)[0], &(*m_value)[1], &(*m_value)[2], m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ, m_scale))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
    }

    void mvSlider3D::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "max_x")) m_maxX = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_y")) m_maxY = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_z")) m_maxZ = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_x")) m_minX = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_y")) m_minY = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_z")) m_minZ = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "scale")) m_scale = ToFloat(item);
    }

    void mvSlider3D::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

    }

}
