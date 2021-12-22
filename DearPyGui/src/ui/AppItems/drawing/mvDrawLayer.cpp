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

        for (auto& item : childslots[2])
        {
            // skip item if it's not shown
            if (!item->config.show)
                continue;

            item->drawInfo->perspectiveDivide = drawInfo->perspectiveDivide;
            item->drawInfo->depthClipping = drawInfo->depthClipping;

            item->drawInfo->transform = drawInfo->transform;

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

    void mvDrawLayer::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "perspective_divide")) drawInfo->perspectiveDivide = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "depth_clipping")) drawInfo->depthClipping = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "cull_mode")) drawInfo->cullMode = ToInt(item);

    }

    void mvDrawLayer::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        PyDict_SetItemString(dict, "perspective_divide", mvPyObject(ToPyBool(drawInfo->perspectiveDivide)));
        PyDict_SetItemString(dict, "depth_clipping", mvPyObject(ToPyBool(drawInfo->depthClipping)));
        PyDict_SetItemString(dict, "cull_mode", mvPyObject(ToPyInt(drawInfo->cullMode)));
    }
    
}
