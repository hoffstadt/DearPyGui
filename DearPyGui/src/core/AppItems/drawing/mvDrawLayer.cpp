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

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::Float, "angle", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::FloatList, "axis", mvArgType::REQUIRED_ARG, "", "axis to rotate around" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_rotation_transform", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "scales", mvArgType::REQUIRED_ARG, "", "scale value per axis" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_scale_transform", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "translation", mvArgType::REQUIRED_ARG, "", "translation" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_translation_transform", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "eye", mvArgType::REQUIRED_ARG, "", "scale value per axis" });
            args.push_back({ mvPyDataType::FloatList, "center", mvArgType::REQUIRED_ARG, "", "scale value per axis" });
            args.push_back({ mvPyDataType::FloatList, "up", mvArgType::REQUIRED_ARG, "", "scale value per axis" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_lookat_transform", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::Float, "fov", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "aspect", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "zNear", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "zFar", mvArgType::REQUIRED_ARG, "", "angle to rotate" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_perspective_transform", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::Float, "left", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "right", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "bottom", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "top", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "zNear", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "zFar", mvArgType::REQUIRED_ARG, "", "angle to rotate" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_orthographic_transform", parser });
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
                item->_appliedTransformIsIdentity = true;
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

                item->_transform = _transform*_appliedTransform;

                item->_transformIsIdentity = false;
                item->_appliedTransformIsIdentity = false;
                
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
            graph->_appliedTransformIsIdentity = false;
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

    PyObject* mvDrawLayer::create_rotation_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        float angle = 0.0f;
        PyObject* axis;

        if (!Parse((GetParsers())["create_rotation_transform"], args, kwargs, __FUNCTION__, &angle, &axis))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvVec4 aaxis = ToVec4(axis);

        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvRotate(identity, angle, aaxis.xyz());

        return newbuffer;
    }

    PyObject* mvDrawLayer::create_perspective_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        float fov = 0.0f;
        float aspect = 0.0f;
        float zNear = 0.0f;
        float zFar = 0.0f;

        if (!Parse((GetParsers())["create_perspective_transform"], args, kwargs, __FUNCTION__, 
            &fov, &aspect, &zNear, &zFar))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvPerspectiveRH(fov, aspect, zNear, zFar);

        return newbuffer;
    }

    PyObject* mvDrawLayer::create_orthographic_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;
        float zNear = 0.0f;
        float zFar = 0.0f;

        if (!Parse((GetParsers())["create_orthographic_transform"], args, kwargs, __FUNCTION__,
            &left, &right, &bottom, &top, &zNear, &zFar))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvOrthoRH(left, right, bottom, top, zNear, zFar);

        return newbuffer;
    }

    PyObject* mvDrawLayer::create_translation_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* axis;

        if (!Parse((GetParsers())["create_translation_transform"], args, kwargs, __FUNCTION__, &axis))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvVec4 aaxis = ToVec4(axis);

        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvTranslate(identity, aaxis.xyz());

        return newbuffer;
    }

    PyObject* mvDrawLayer::create_scale_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* axis;

        if (!Parse((GetParsers())["create_scale_transform"], args, kwargs, __FUNCTION__, &axis))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvVec4 aaxis = ToVec4(axis);

        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvScale(identity, aaxis.xyz());

        return newbuffer;
    }

    PyObject* mvDrawLayer::create_lookat_transform(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* eye;
        PyObject* center;
        PyObject* up;

        if (!Parse((GetParsers())["create_lookat_transform"], args, kwargs, __FUNCTION__, 
            &eye, &center, &up))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvVec4 aeye = ToVec4(eye);
        mvVec4 acenter = ToVec4(center);
        mvVec4 aup = ToVec4(up);

        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvLookAtRH(aeye.xyz(), acenter.xyz(), aup.xyz());

        return newbuffer;
    }
}
