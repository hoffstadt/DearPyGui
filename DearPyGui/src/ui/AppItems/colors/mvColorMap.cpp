#include "mvColorMap.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvItemRegistry.h"
#include <implot.h>
#include "mvPythonExceptions.h"
#include "mvLog.h"
#include "mvAppItemCommons.h"

namespace Marvel {

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
        if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
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

        _colorMap = ImPlot::AddColormap(info.internalLabel.c_str(), _colors.data(), (int)_colors.size(), _qualitative);
    }

    void mvColorMap::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(uuid);

        ImPlot::ColormapButton(info.internalLabel.c_str(), ImVec2(-1.0f, 0.0f), _colorMap);
    }



}
