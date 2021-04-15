#include "mvKnob.h"
#include "mvKnobCustom.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvKnobFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("enabled");

        parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvKnobFloat::mvKnobFloat(const std::string& name)
        : mvFloatPtrBase(name)
    {
    }

    void mvKnobFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (KnobFloat(m_specificedlabel.c_str(), m_value.get(), m_min, m_max, m_step))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
    }

    void mvKnobFloat::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);
    }

    void mvKnobFloat::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

    }

}