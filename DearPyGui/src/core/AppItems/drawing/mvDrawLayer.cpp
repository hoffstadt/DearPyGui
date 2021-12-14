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
    
}
