#include "mvColorMapScale.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvItemRegistry.h"
#include <implot.h>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvColorMapScale::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds a legend that pairs values with colors. This is typically used with a heat series. ", {"Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Integer>("default_value", mvArgType::KEYWORD_ARG, "0");

        parser.addArg<mvPyDataType::Float>("min_scale", mvArgType::KEYWORD_ARG, "0.0", "Sets the min number of the color scale. Typically is the same as the min scale from the heat series.");
        parser.addArg<mvPyDataType::Float>("max_scale", mvArgType::KEYWORD_ARG, "1.0", "Sets the max number of the color scale. Typically is the same as the max scale from the heat series.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvColorMapScale::mvColorMapScale(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvColorMapScale::setDataSource(mvUUID dataSource)
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
        _value = std::get<std::shared_ptr<int>>(item->getValue());
    }

    PyObject* mvColorMapScale::getPyValue()
    {
        return ToPyInt(*_value);
    }

    void mvColorMapScale::setPyValue(PyObject* value)
    {
        *_value = ToInt(value);
    }

    void mvColorMapScale::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(_uuid);

        ImPlot::ColormapScale(_label.c_str(), _scale_min, _scale_max, ImVec2((float)_width, (float)_height), *_value);
    }

    void mvColorMapScale::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_scale")) _scale_min = (double)ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_scale")) _scale_max = (double)ToFloat(item);
    }

    void mvColorMapScale::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "min_scale", ToPyFloat(_scale_min));
        PyDict_SetItemString(dict, "max_scale", ToPyFloat(_scale_max));

    }

}
