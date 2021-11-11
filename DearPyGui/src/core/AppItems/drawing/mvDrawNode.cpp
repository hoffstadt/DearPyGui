#include "mvDrawNode.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvMat4Type.h"

namespace Marvel {

	void mvDrawNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
            setup.about = "New in 1.1. Creates a drawing node to associate a transformation matrix. Child node matricies will concatenate.";
            setup.category = { "Drawlist", "Widgets", "Matrix Operations"};
            setup.returnType = mvPyDataType::UUID;
            setup.createContextManager = true;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ s_command, parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "Drawing node to apply transform to."});
            args.push_back({ mvPyDataType::Object, "transform", mvArgType::REQUIRED_ARG, "", "Transformation matrix."});

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Matrix Operations" };

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "apply_transform", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::Float, "angle", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
            args.push_back({ mvPyDataType::FloatList, "axis", mvArgType::REQUIRED_ARG, "", "axis to rotate around" });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_rotation_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "scales", mvArgType::REQUIRED_ARG, "", "scale values per axis" });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_scale_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "translation", mvArgType::REQUIRED_ARG, "", "translation vector" });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Creates a translation matrix.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_translation_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "eye", mvArgType::REQUIRED_ARG, "", "eye position" });
            args.push_back({ mvPyDataType::FloatList, "target", mvArgType::REQUIRED_ARG, "", "target position" });
            args.push_back({ mvPyDataType::FloatList, "up", mvArgType::REQUIRED_ARG, "", "up vector" });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Creates a 'Look at matrix'.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_lookat_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::Float, "fov", mvArgType::REQUIRED_ARG, "", "Field of view (in radians)" });
            args.push_back({ mvPyDataType::Float, "aspect", mvArgType::REQUIRED_ARG, "", "Aspect ratio (width/height)" });
            args.push_back({ mvPyDataType::Float, "zNear", mvArgType::REQUIRED_ARG, "", "Near clipping plane." });
            args.push_back({ mvPyDataType::Float, "zFar", mvArgType::REQUIRED_ARG, "", "Far clipping plane." });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Creates a perspective matrix.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_perspective_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::Float, "left", mvArgType::REQUIRED_ARG, "", "left plane" });
            args.push_back({ mvPyDataType::Float, "right", mvArgType::REQUIRED_ARG, "", "right plane" });
            args.push_back({ mvPyDataType::Float, "bottom", mvArgType::REQUIRED_ARG, "", "bottom plane" });
            args.push_back({ mvPyDataType::Float, "top", mvArgType::REQUIRED_ARG, "", "top plane" });
            args.push_back({ mvPyDataType::Float, "zNear", mvArgType::REQUIRED_ARG, "", "Near clipping plane." });
            args.push_back({ mvPyDataType::Float, "zFar", mvArgType::REQUIRED_ARG, "", "Far clipping plane." });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Creates an orthographic matrix.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_orthographic_matrix", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            args.push_back({ mvPyDataType::FloatList, "eye", mvArgType::REQUIRED_ARG, "", "eye position" });
            args.push_back({ mvPyDataType::Float, "pitch", mvArgType::REQUIRED_ARG, "", "pitch (in radians)" });
            args.push_back({ mvPyDataType::Float, "yaw", mvArgType::REQUIRED_ARG, "", "yaw (in radians)" });

            mvPythonParserSetup setup;
            setup.about = "New in 1.1. Create a 'first person shooter' matrix.";
            setup.category = { "Drawlist", "Matrix Operations" };
            setup.returnType = mvPyDataType::Object;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ "create_fps_matrix", parser });
        }
	}

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
    
    PyObject* mvDrawNode::apply_transform(PyObject* self, PyObject* args, PyObject* kwargs)
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

        if (aitem->getType() == mvAppItemType::mvDrawNode)
        {
            mvDrawNode* graph = static_cast<mvDrawNode*>(aitem);
            graph->_appliedTransform = atransform->m;
        }

        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "apply_transform",
                "Incompatible type. Expected types include: mvDrawNode", aitem);
            return GetPyNone();
        }

        
        return GetPyNone();
    }
    
    PyObject* mvDrawNode::create_rotation_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

    PyObject* mvDrawNode::create_perspective_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

    PyObject* mvDrawNode::create_orthographic_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

    PyObject* mvDrawNode::create_translation_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

    PyObject* mvDrawNode::create_scale_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

    PyObject* mvDrawNode::create_lookat_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

    PyObject* mvDrawNode::create_fps_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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
