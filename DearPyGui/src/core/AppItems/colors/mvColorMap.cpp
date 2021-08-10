#include "mvColorMap.h"
#include <utility>
#include "mvApp.h"
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
        {
            mvPythonParser parser(mvPyDataType::UUID, "Adds a legend that pairs values with colors. This is typically used with a heat series. ", { "Widgets" });
            mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
                MV_PARSER_ARG_ID |
                MV_PARSER_ARG_SHOW)
            );

            parser.addArg<mvPyDataType::ListListInt>("colors");
            parser.addArg<mvPyDataType::Bool>("qualitative");
            parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_4", "Parent to add this item to. (runtime adding)");

            parser.finalize();

            parsers->insert({ s_command, parser });
        }

        {
            mvPythonParser parser(mvPyDataType::None, "Sets the color map for widgets that accept it.", { "Widgets" });

            parser.addArg<mvPyDataType::UUID>("item");
            parser.addArg<mvPyDataType::UUID>("source");

            parser.finalize();

            parsers->insert({ "set_colormap", parser });
        }

        {
            mvPythonParser parser(mvPyDataType::IntList, "Returns a color from a colormap given t between 0 and 1. This command can only be ran once the app is started.", { "Widgets" });

            parser.addArg<mvPyDataType::UUID>("colormap");
            parser.addArg<mvPyDataType::Float>("t");

            parser.finalize();

            parsers->insert({ "sample_colormap", parser });
        }

        {
            mvPythonParser parser(mvPyDataType::IntList, "Returns a color from a colormap given an index >= 0 (modulo will be performed). This command can only be ran once the app is started.", { "Widgets" });

            parser.addArg<mvPyDataType::UUID>("colormap");
            parser.addArg<mvPyDataType::Integer>("index");

            parser.finalize();

            parsers->insert({ "get_colormap_color", parser });
        }
    }

    mvColorMap::mvColorMap(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _triggerAlternativeAction = true;
        
        // since the implot context may not have been created
        static ImPlotColormap startmap = 16;
        _colorMap = startmap++;
    }

    void mvColorMap::handleSpecificRequiredArgs(PyObject* dict)
    {
        if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
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
    }

    void mvColorMap::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(_uuid);

        ImPlot::ColormapButton(_internalLabel.c_str(), ImVec2(-1.0f, 0.0f), _colorMap);
    }

    void mvColorMap::alternativeCustomAction()
    {
        if (_created)
            return;

        _colorMap = ImPlot::AddColormap(_internalLabel.c_str(), _colors.data(), _colors.size(), _qualitative);
        _created = true;

        _triggerAlternativeAction = false;
    }

    PyObject* mvColorMap::set_colormap(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mvUUID item;
        mvUUID source;

        if (!(mvApp::GetApp()->getParsers())["set_colormap"].parse(args, kwargs, __FUNCTION__, &item, &source))
            return GetPyNone();

        if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
        auto aitem = mvApp::GetApp()->getItemRegistry().getItem(item);
        if (aitem == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_colormap",
                "Item not found: " + std::to_string(item), nullptr);
            return GetPyNone();
        }

        if (source > 15)
        {
            auto asource = mvApp::GetApp()->getItemRegistry().getItem(source);
            if (asource == nullptr)
            {
                mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_colormap",
                    "Source Item not found: " + std::to_string(source), nullptr);
                return GetPyNone();
            }

            if (asource->getType() == mvAppItemType::mvColorMap)
            {
                mvColorMap* colormap = static_cast<mvColorMap*>(asource);
                source = colormap->_colorMap;
            }
        }

        if (aitem->getType() == mvAppItemType::mvPlot)
        {
            mvPlot* graph = static_cast<mvPlot*>(aitem);
            graph->SetColorMap(source);
        }

        else if (aitem->getType() == mvAppItemType::mvColorMapScale)
        {
            mvColorMapScale* graph = static_cast<mvColorMapScale*>(aitem);
            graph->setColorMap(source);
        }

        else if (aitem->getType() == mvAppItemType::mvColorMapButton)
        {
            mvColorMapButton* graph = static_cast<mvColorMapButton*>(aitem);
            graph->setColorMap(source);
        }

        else if (aitem->getType() == mvAppItemType::mvColorMapSlider)
        {
            mvColorMapSlider* graph = static_cast<mvColorMapSlider*>(aitem);
            graph->setColorMap(source);
        }

        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_colormap",
                "Incompatible type. Expected types include: mvPlot, mvColorMapScale, mvColorMapButton", aitem);
            return GetPyNone();
        }

        
        return GetPyNone();
    }

    PyObject* mvColorMap::sample_colormap(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mvUUID item;
        float t;

        if (!(mvApp::GetApp()->getParsers())["sample_colormap"].parse(args, kwargs, __FUNCTION__, &item, &t))
            return GetPyNone();

        if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

        if (item > 15)
        {
            auto asource = mvApp::GetApp()->getItemRegistry().getItem(item);
            if (asource == nullptr)
            {
                mvThrowPythonError(mvErrorCode::mvItemNotFound, "sample_colormap",
                    "Source Item not found: " + std::to_string(item), nullptr);
                return GetPyNone();
            }

            if (asource->getType() == mvAppItemType::mvColorMap)
            {
                mvColorMap* colormap = static_cast<mvColorMap*>(asource);
                item = colormap->_colorMap;
            }
        }


        if (!mvApp::IsAppStarted())
        {
            mvThrowPythonError(mvErrorCode::mvNone, "sample_colormap", "This command can only be ran once the app is started.", nullptr);
            return GetPyNone();
        }
        
        ImVec4 result = ImPlot::SampleColormap(t, item);
        mvColor resultColor = mvColor(result.x, result.y, result.z, result.w);
        return ToPyColor(resultColor);
    }

    PyObject* mvColorMap::get_colormap_color(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mvUUID item;
        int index;

        if (!(mvApp::GetApp()->getParsers())["get_colormap_color"].parse(args, kwargs, __FUNCTION__, &item, &index))
            return GetPyNone();

        if(!mvApp::IsAppStarted())
        {
            mvThrowPythonError(mvErrorCode::mvNone, "get_colormap_color", "This command can only be ran once the app is started.", nullptr);
            return GetPyNone();
        }

        if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

        if (item > 15)
        {
            auto asource = mvApp::GetApp()->getItemRegistry().getItem(item);
            if (asource == nullptr)
            {
                mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_colormap_color",
                    "Source Item not found: " + std::to_string(item), nullptr);
                return GetPyNone();
            }

            if (asource->getType() == mvAppItemType::mvColorMap)
            {
                mvColorMap* colormap = static_cast<mvColorMap*>(asource);
                item = colormap->_colorMap;
            }
        }


        ImVec4 result = ImPlot::GetColormapColor(index, item);
        mvColor resultColor = mvColor(result.x, result.y, result.z, result.w);
        return ToPyColor(resultColor);
    }

}
