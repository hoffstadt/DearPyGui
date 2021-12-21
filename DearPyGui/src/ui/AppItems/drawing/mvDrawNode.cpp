#include "mvDrawNode.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvMat4Type.h"

namespace Marvel {

    mvDrawNode::mvDrawNode(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

    mvDrawNode::~mvDrawNode()
    {
    }

	void mvDrawNode::draw(ImDrawList* drawlist, float x, float y)
	{

        for (auto& item : _children[2])
        {
            // skip item if it's not shown
            if (!item->_show)
                continue;

            item->_transform =   _transform * _appliedTransform;

            item->_perspectiveDivide = _perspectiveDivide;
            item->_depthClipping = _depthClipping;
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

    void mvDrawNode::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

    }

    void mvDrawNode::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
    }
   
}
