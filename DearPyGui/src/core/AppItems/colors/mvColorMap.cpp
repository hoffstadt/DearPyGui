#include "mvColorMap.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvItemRegistry.h"
#include <implot.h>
#include "mvPythonExceptions.h"
#include "mvLog.h"
#include "mvAppItemCommons.h"

namespace Marvel {

    void mvColorMap::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::ListListInt, "colors", mvArgType::REQUIRED_ARG, "", "colors that will be mapped to the normalized value 0.0->1.0"});
        args.push_back({ mvPyDataType::Bool, "qualitative", mvArgType::REQUIRED_ARG, "", "Qualitative will create hard transitions for color boundries across the value range when enabled."});
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_4", "Parent to add this item to. (runtime adding)" });

        mvPythonParserSetup setup;
        setup.about = "Adds a legend that pairs colors with normalized value 0.0->1.0. Each color will be  This is typically used with a heat series. (ex. [[0, 0, 0, 255], [255, 255, 255, 255]] will be mapped to a soft transition from 0.0-1.0)";
        setup.category = { "Widgets", "Colors" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });

    }

    mvColorMap::mvColorMap(mvUUID uuid)
        : mvAppItem(uuid)
    { 
    }

    void mvColorMap::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvColorMap*>(item);
        _colorMap = titem->_colorMap;
        _qualitative = titem->_qualitative;
        _colors = titem->_colors;
    }

    void mvColorMap::handleSpecificRequiredArgs(PyObject* dict)
    {
        if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
            return;

        for (int i = 0; i < PyTuple_Size(dict); i++)
        {
            PyObject* item = PyTuple_GetItem(dict, i);
            switch (i)
            {
            case 0:
            {
                auto rawColors = ToVectVectInt(item);
                for (const auto& color : rawColors)
                {

                    std::vector<float> c;
                    
                    for (int j = 0; j < color.size(); j++)
                        c.push_back((float)color[j] / 255.0f);

                    while (c.size() < 4)
                        c.push_back(1.0f);

                    _colors.push_back(ImVec4(c[0], c[1], c[2], c[3]));
                    
                }
                break;
            }

            case 1:
                _qualitative = ToBool(item);
                break;

            default:
                break;
            }
        }

        _colorMap = ImPlot::AddColormap(_internalLabel.c_str(), _colors.data(), (int)_colors.size(), _qualitative);
    }

    void mvColorMap::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(_uuid);

        ImPlot::ColormapButton(_internalLabel.c_str(), ImVec2(-1.0f, 0.0f), _colorMap);
    }



}
