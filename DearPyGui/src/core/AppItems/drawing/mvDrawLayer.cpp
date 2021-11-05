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

            args.push_back({ mvPyDataType::Bool, "perspective_divide", mvArgType::KEYWORD_ARG, "False", "apply perspective divide" });
            args.push_back({ mvPyDataType::Bool, "depth_clipping", mvArgType::KEYWORD_ARG, "False", "apply depth clipping" });
            args.push_back({ mvPyDataType::Integer, "cull_mode", mvArgType::KEYWORD_ARG, "0" });

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

            args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "item that the color map will be applied to" });
            args.push_back({ mvPyDataType::Float, "top_left_x", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "top_left_y", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "width", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "height", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "min_depth", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::Float, "max_depth", mvArgType::REQUIRED_ARG, "", "angle to rotate" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::None;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "set_clip_space", parser });
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

            parsers->insert({ "create_rotation_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "scales", mvArgType::REQUIRED_ARG, "", "scale value per axis" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_scale_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "translation", mvArgType::REQUIRED_ARG, "", "translation" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_translation_matrix", parser });
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

            parsers->insert({ "create_lookat_matrix", parser });
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

            parsers->insert({ "create_perspective_matrix", parser });
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

            parsers->insert({ "create_orthographic_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "eye", mvArgType::REQUIRED_ARG, "", "scale value per axis" });
            args.push_back({ mvPyDataType::Float, "pitch", mvArgType::REQUIRED_ARG, "", "scale value per axis" });
            args.push_back({ mvPyDataType::Float, "yaw", mvArgType::REQUIRED_ARG, "", "scale value per axis" });

            mvPythonParserSetup setup;
            setup.about = "Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Widgets" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_fps_matrix", parser });
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
        else
        {
            for (auto& item : _children[2])
            {
                // skip item if it's not shown
                if (!item->_show)
                    continue;

                item->_transform = _viewportTransform*_transform*_appliedTransform;

                item->_transformIsIdentity = false;
                item->_appliedTransformIsIdentity = false;
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
            graph->_clipViewport[1] = toplefty;
            graph->_clipViewport[2] = width;
            graph->_clipViewport[3] = height;
            graph->_clipViewport[4] = mindepth;
            graph->_clipViewport[5] = maxdepth;
            //graph->_viewportTransform = mvCreateMatrix(
            //    width / 2.0f,           0.0f, 0.0f,  width / 2.0f,
            //            0.0f, -height / 2.0f, 0.0f, height / 2.0f,
            //            0.0f,           0.0f, 0.5f,          0.5f,
            //            0.0f,           0.0f, 0.0f,          1.0f
            //);
            graph->_viewportTransform = mvCreateMatrix(
                       width,      0.0f, 0.0f,      width/2.0f,
                        0.0f,        -height, 0.0f, height /2.0f,
                        0.0f,           0.0f, 0.5f,    0.5f,
                        0.0f,           0.0f, 0.0f,    1.0f
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

    PyObject* mvDrawLayer::create_rotation_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        float angle = 0.0f;
        PyObject* axis;

        if (!Parse((GetParsers())["create_rotation_matrix"], args, kwargs, __FUNCTION__, &angle, &axis))
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

    PyObject* mvDrawLayer::create_perspective_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        float fov = 0.0f;
        float aspect = 0.0f;
        float zNear = 0.0f;
        float zFar = 0.0f;

        if (!Parse((GetParsers())["create_perspective_matrix"], args, kwargs, __FUNCTION__, 
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

    PyObject* mvDrawLayer::create_orthographic_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;
        float zNear = 0.0f;
        float zFar = 0.0f;

        if (!Parse((GetParsers())["create_orthographic_matrix"], args, kwargs, __FUNCTION__,
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

    PyObject* mvDrawLayer::create_translation_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* axis;

        if (!Parse((GetParsers())["create_translation_matrix"], args, kwargs, __FUNCTION__, &axis))
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

    PyObject* mvDrawLayer::create_scale_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* axis;

        if (!Parse((GetParsers())["create_scale_matrix"], args, kwargs, __FUNCTION__, &axis))
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

    PyObject* mvDrawLayer::create_lookat_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* eye;
        PyObject* center;
        PyObject* up;

        if (!Parse((GetParsers())["create_lookat_matrix"], args, kwargs, __FUNCTION__, 
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

    PyObject* mvDrawLayer::create_fps_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        mv_local_persist mvMat4 identity = mvIdentityMat4();
        PyObject* eye;
        f32 pitch = 0.0f;
        f32 yaw = 0.0f;

        if (!Parse((GetParsers())["create_fps_matrix"], args, kwargs, __FUNCTION__,
            &eye, &pitch, &yaw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvVec4 aeye = ToVec4(eye);
        PyObject* newbuffer = nullptr;
        PymvMat4* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

        newbufferview->m = mvFPSViewRH(aeye.xyz(), pitch, yaw);

        return newbuffer;
    }
}
