#include "mvKnob.h"
#include "mvKnobCustom.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvKnobFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID);
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_LABEL |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_USER_DATA |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvKnobFloat::mvKnobFloat(mvUUID uuid)
        : mvFloatPtrBase(uuid)
    {
    }

    void mvKnobFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(m_uuid);
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (KnobFloat(m_specificedlabel.c_str(), m_value.get(), m_min, m_max, m_step))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
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

        PyDict_SetItemString(dict, "min_scale", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_scale", ToPyFloat(m_max));

    }

}