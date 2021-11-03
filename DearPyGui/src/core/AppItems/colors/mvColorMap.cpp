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

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "item that the color map will be applied to"});
            args.push_back({ mvPyDataType::UUID, "source", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry.  Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });

            mvPythonParserSetup setup;
            setup.about = "Sets the color map for widgets that accept it.";
            setup.category = { "Widget Operations" };

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "bind_colormap", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
  
            args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry. Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });
            args.push_back({ mvPyDataType::Float, "t", mvArgType::REQUIRED_ARG, "", "Value of the colormap to sample between 0.0-1.0" });

            mvPythonParserSetup setup;
            setup.about = "Returns a color from a colormap given t between 0.0-1.0.";
            setup.category = { "Widget Operations" };
            setup.returnType = mvPyDataType::IntList;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "sample_colormap", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry. Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });
            args.push_back({ mvPyDataType::Integer, "index", mvArgType::REQUIRED_ARG, "", "Desired position of the color in the colors list value of the colormap being quiered " });

            mvPythonParserSetup setup;
            setup.about = "Returns a color from a colormap given an index >= 0. (ex. 0 will be the first color in the color list of the color map) Modulo will be performed against the number of items in the color list.";
            setup.category = { "Widget Operations" };
            setup.returnType = mvPyDataType::IntList;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "get_colormap_color", parser });
        }
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

    PyObject* mvColorMap::bind_colormap(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        PyObject* sourceraw;

        if (!Parse((GetParsers())["bind_colormap"], args, kwargs, __FUNCTION__, &itemraw, &sourceraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvUUID source = GetIDFromPyObject(sourceraw);

        auto aitem = GetItem((*GContext->itemRegistry), item);
        if (aitem == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_colormap",
                "Item not found: " + std::to_string(item), nullptr);
            return GetPyNone();
        }

        if (source > 15)
        {
            auto asource = GetItem(*GContext->itemRegistry, source);
            if (asource == nullptr)
            {
                mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_colormap",
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
            graph->SetColorMap((ImPlotColormap)source);
        }

        else if (aitem->getType() == mvAppItemType::mvColorMapScale)
        {
            mvColorMapScale* graph = static_cast<mvColorMapScale*>(aitem);
            graph->setColorMap((ImPlotColormap)source);
        }

        else if (aitem->getType() == mvAppItemType::mvColorMapButton)
        {
            mvColorMapButton* graph = static_cast<mvColorMapButton*>(aitem);
            graph->setColorMap((ImPlotColormap)source);
        }

        else if (aitem->getType() == mvAppItemType::mvColorMapSlider)
        {
            mvColorMapSlider* graph = static_cast<mvColorMapSlider*>(aitem);
            graph->setColorMap((ImPlotColormap)source);
        }

        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_colormap",
                "Incompatible type. Expected types include: mvPlot, mvColorMapScale, mvColorMapButton", aitem);
            return GetPyNone();
        }

        
        return GetPyNone();
    }

    PyObject* mvColorMap::sample_colormap(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        float t;

        if (!Parse((GetParsers())["sample_colormap"], args, kwargs, __FUNCTION__, &itemraw, &t))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        if (item > 15)
        {
            auto asource = GetItem((*GContext->itemRegistry), item);
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


        if (!GContext->started)
        {
            mvThrowPythonError(mvErrorCode::mvNone, "sample_colormap", "This command can only be ran once the app is started.", nullptr);
            return GetPyNone();
        }
        
        ImVec4 result = ImPlot::SampleColormap(t, (ImPlotColormap)item);
        mvColor resultColor = mvColor(result.x, result.y, result.z, result.w);
        return ToPyColor(resultColor);
    }

    PyObject* mvColorMap::get_colormap_color(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        int index;

        if (!Parse((GetParsers())["get_colormap_color"], args, kwargs, __FUNCTION__, &itemraw, &index))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        if (item > 15)
        {
            auto asource = GetItem((*GContext->itemRegistry), item);
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


        ImVec4 result = ImPlot::GetColormapColor(index, (ImPlotColormap)item);
        mvColor resultColor = mvColor(result.x, result.y, result.z, result.w);
        return ToPyColor(resultColor);
    }

}
