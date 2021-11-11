#include "mvDrawLayer.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvMat4Type.h"

namespace Marvel {

	void mvDrawLayer::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
        {
            std::vector<mvPythonDataElement> args;

            AddCommonArgs(args,(CommonParserArgs)(
                              MV_PARSER_ARG_ID |
                              MV_PARSER_ARG_PARENT |
                              MV_PARSER_ARG_BEFORE |
                              MV_PARSER_ARG_SHOW)
                          );

            args.push_back({ mvPyDataType::Bool, "perspective_divide", mvArgType::KEYWORD_ARG, "False", "New in 1.1. apply perspective divide" });
            args.push_back({ mvPyDataType::Bool, "depth_clipping", mvArgType::KEYWORD_ARG, "False", "New in 1.1. apply depth clipping" });
            args.push_back({ mvPyDataType::Integer, "cull_mode", mvArgType::KEYWORD_ARG, "0", "New in 1.1. culling mode, mvCullMode_* constants. Only works with triangles currently."});

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Creates a layer useful for grouping drawlist items.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::UUID;
            setup.createContextManager = true;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ s_command, parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "draw layer to set clip space" });
            args.push_back({ mvPyDataType::Float, "top_left_x", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "top_left_y", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "width", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "height", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "min_depth", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "max_depth", mvArgType::REQUIRED_ARG, "", "angle to rotate" });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Set the clip space for depth clipping and 'viewport' transformation.";
            setup.category = { "Drawlist", "Widgets" };

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "set_clip_space", parser });
        }

	}

	mvDrawLayer::mvDrawLayer(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

    mvDrawLayer::~mvDrawLayer()
    {
    }

	void mvDrawLayer::draw(ImDrawList* drawlist, float x, float y)
	{

        for (auto& item : _children[2])
        {
            // skip item if it's not shown
            if (!item->_show)
                continue;

            item->_perspectiveDivide = _perspectiveDivide;
            item->_depthClipping = _depthClipping;

            item->_transform = _transform;

            item->_cullMode = _cullMode;
            item->_clipViewport[0] = _clipViewport[0];
            item->_clipViewport[1] = _clipViewport[1];
            item->_clipViewport[2] = _clipViewport[2];
            item->_clipViewport[3] = _clipViewport[3];
            item->_clipViewport[4] = _clipViewport[4];
            item->_clipViewport[5] = _clipViewport[5];
            item->draw(drawlist, x, y);

            UpdateAppItemState(item->_state);
        }
	}

    void mvDrawLayer::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "perspective_divide")) _perspectiveDivide = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "depth_clipping")) _depthClipping = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "cull_mode")) _cullMode = ToInt(item);

    }

    void mvDrawLayer::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        PyDict_SetItemString(dict, "perspective_divide", mvPyObject(ToPyBool(_perspectiveDivide)));
        PyDict_SetItemString(dict, "depth_clipping", mvPyObject(ToPyBool(_depthClipping)));
        PyDict_SetItemString(dict, "cull_mode", mvPyObject(ToPyInt(_cullMode)));
    }
    
    PyObject* mvDrawLayer::set_clip_space(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        float topleftx = 0.0f;
        float toplefty = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        float mindepth = 0.0f;
        float maxdepth = 0.0f;

        if (!Parse((GetParsers())["set_clip_space"], args, kwargs, __FUNCTION__, &itemraw,
            &topleftx, &toplefty, &width, &height, &mindepth, &maxdepth))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto aitem = GetItem((*GContext->itemRegistry), item);
        if (aitem == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "apply_transform",
                "Item not found: " + std::to_string(item), nullptr);
            return GetPyNone();
        }

        if (aitem->getType() == mvAppItemType::mvDrawLayer)
        {
            mvDrawLayer* graph = static_cast<mvDrawLayer*>(aitem);
            graph->_clipViewport[0] = topleftx;
            graph->_clipViewport[1] = toplefty + height;
            graph->_clipViewport[2] = width;
            graph->_clipViewport[3] = height;
            graph->_clipViewport[4] = mindepth;
            graph->_clipViewport[5] = maxdepth;

            graph->_transform = mvCreateMatrix(
                width, 0.0f, 0.0f, topleftx + (width / 2.0f),
                0.0f, -height, 0.0f, toplefty + (height / 2.0f),
                0.0f, 0.0f, 0.25f, 0.5f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "apply_transform",
                "Incompatible type. Expected types include: mvDrawLayer", aitem);
            return GetPyNone();
        }


        return GetPyNone();
    }
}
