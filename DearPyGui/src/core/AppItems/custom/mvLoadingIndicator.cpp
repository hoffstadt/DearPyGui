#include "mvLoadingIndicator.h"
#include "mvLoadingIndicatorCustom.h"

namespace Marvel {

    void mvLoadingIndicator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds a rotating anamated loding symbol.", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Integer>("style", mvArgType::KEYWORD_ARG, "0", "0 is rotating dots style, 1 is rotating bar style.");
        parser.addArg<mvPyDataType::Integer>("circle_count", mvArgType::KEYWORD_ARG, "8", "Number of dots show if dots or size of circle if circle.");
        parser.addArg<mvPyDataType::Float>("speed", mvArgType::KEYWORD_ARG, "1.0", "Speed the anamation will rotate.");
        parser.addArg<mvPyDataType::Float>("radius", mvArgType::KEYWORD_ARG, "3.0", "Radius size of the loading indicator.");
        parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0", "Thickness of the circles or line.");
        parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(51, 51, 55, 255)", "Color of the growing center circle.");
        parser.addArg<mvPyDataType::IntList>("secondary_color", mvArgType::KEYWORD_ARG, "(29, 151, 236, 103)", "Background of the dots in dot mode.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvLoadingIndicator::mvLoadingIndicator(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvLoadingIndicator::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(m_uuid);

        if (m_style == 0)
            LoadingIndicatorCircle(m_specificedlabel.c_str(), m_radius, m_mainColor, m_optionalColor, m_circleCount, m_speed);
        else
            LoadingIndicatorCircle2(m_specificedlabel.c_str(), m_radius, m_thickness, m_mainColor);
    }

    void mvLoadingIndicator::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "style")) m_style = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "circle_count")) m_circleCount = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "color")) m_mainColor = ToColor(item);
        if (PyObject* item = PyDict_GetItemString(dict, "secondary_color")) m_optionalColor = ToColor(item);
    }

    void mvLoadingIndicator::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "style", ToPyInt(m_style));
        PyDict_SetItemString(dict, "circle_count", ToPyInt(m_circleCount));
        PyDict_SetItemString(dict, "radius", ToPyFloat(m_radius));
        PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "color", ToPyColor(m_mainColor));
        PyDict_SetItemString(dict, "secondary_color", ToPyColor(m_optionalColor));

    }

}