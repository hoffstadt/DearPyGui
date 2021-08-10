#include "mvKnob.h"
#include "mvKnobCustom.h"
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvKnobFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds a knob that rotates based of change in x mouse position.", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies lower limit to value.");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies upper limit to value.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvKnobFloat::mvKnobFloat(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvKnobFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(_uuid);

        if (KnobFloat(_specificedlabel.c_str(), _value.get(), _min, _max, _step))
        {
            auto value = *_value;
            mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloat(value), _user_data);
                });
        }
    }

    PyObject* mvKnobFloat::getPyValue()
    {
        return ToPyFloat(*_value);
    }

    void mvKnobFloat::setPyValue(PyObject* value)
    {
        *_value = ToFloat(value);
    }

    void mvKnobFloat::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (item->getValueType() != getValueType())
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<float>*>(item->getValue());
    }

    void mvKnobFloat::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) _min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) _max = ToFloat(item);
    }

    void mvKnobFloat::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "min_scale", ToPyFloat(_min));
        PyDict_SetItemString(dict, "max_scale", ToPyFloat(_max));

    }

}