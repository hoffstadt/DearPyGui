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

            mvPythonParserSetup setup;
            setup.about = "Creates a layer useful for grouping drawlist items.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::UUID;
            setup.createContextManager = true;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ s_command, parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "item that the color map will be applied to"});
            args.push_back({ mvPyDataType::Object, "transform", mvArgType::REQUIRED_ARG, "", "item that the color map will be applied to"});

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::None;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "apply_transform", parser });
        }
	}

	mvDrawLayer::mvDrawLayer(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvDrawLayer::draw(ImDrawList* drawlist, float x, float y)
	{

        if(_appliedTransformIsIdentity)
        {
            for (auto& item : _children[2])
            {
                // skip item if it's not shown
                if (!item->_show)
                    continue;

                item->_transformIsIdentity = true;
                item->draw(drawlist, x, y);

                UpdateAppItemState(item->_state);
            }
        }
        else
        {
            for (auto& item : _children[2])
            {
                // skip item if it's not shown
                if (!item->_show)
                    continue;

                item->_transform = _appliedTransform * _transform * item->_transform;
                item->_transformIsIdentity = false;
                
                item->draw(drawlist, x, y);

                UpdateAppItemState(item->_state);
            }
        }
	}
    
    PyObject* mvDrawLayer::apply_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        PyObject* transform;

        if (!Parse((GetParsers())["apply_transform"], args, kwargs, __FUNCTION__, &itemraw, &transform))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

      	PymvMat4* atransform = (PymvMat4*)transform;

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
            graph->_appliedTransform = atransform->m;
            graph->_appliedTransformIsIdentity = true;
            // TODO: added check for identity and set back to false
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
