#include "mvDrawLayer.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvMat4Type.h"

namespace Marvel {

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
