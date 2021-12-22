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

        for (auto& item : childslots[2])
        {
            // skip item if it's not shown
            if (!item->config.show)
                continue;

            item->drawInfo->transform =   drawInfo->transform * drawInfo->appliedTransform;

            item->drawInfo->perspectiveDivide = drawInfo->perspectiveDivide;
            item->drawInfo->depthClipping = drawInfo->depthClipping;
            item->drawInfo->cullMode = drawInfo->cullMode;
            item->drawInfo->clipViewport[0] = drawInfo->clipViewport[0];
            item->drawInfo->clipViewport[1] = drawInfo->clipViewport[1];
            item->drawInfo->clipViewport[2] = drawInfo->clipViewport[2];
            item->drawInfo->clipViewport[3] = drawInfo->clipViewport[3];
            item->drawInfo->clipViewport[4] = drawInfo->clipViewport[4];
            item->drawInfo->clipViewport[5] = drawInfo->clipViewport[5];
            item->draw(drawlist, x, y);

            UpdateAppItemState(item->state);
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
