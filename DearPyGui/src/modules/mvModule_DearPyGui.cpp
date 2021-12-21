#include "mvModule_DearPyGui.h"
#include "mvAppItemCommons.h"
#include "mvFontManager.h"
#include "mvItemRegistry.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>
#include "mvToolManager.h"
#include "mvBuffer.h"
#include "mvVec4Type.h"
#include "mvMat4Type.h"
#include "mvPythonExceptions.h"
#include "mvViewport.h"
#include <stb_image.h>
#include "mvLog.h"
#include "mvProfiler.h"

#define MV_ADD_COMMAND(x) methods.push_back({ #x, (PyCFunction)x, METH_VARARGS | METH_KEYWORDS, GetParsers()[#x].documentation.c_str() });

namespace Marvel {

	mv_internal mv_python_function
	bind_colormap(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* sourceraw;

		if (!Parse((GetParsers())["bind_colormap"], args, kwargs, __FUNCTION__, &itemraw, &sourceraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvUUID source = GetIDFromPyObject(sourceraw);

		auto aitem = GetItem((*GContext->itemRegistry), item);
		if (aitem == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_colormap",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (source > 15)
		{
			auto asource = GetItem(*GContext->itemRegistry, source);
			if (asource == nullptr)
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_colormap",
					"Source Item not found: " + std::to_string(source), nullptr);
				return GetPyNone();
			}

			if (asource->_type == mvAppItemType::mvColorMap)
			{
				mvColorMap* colormap = static_cast<mvColorMap*>(asource);
				source = colormap->_colorMap;
			}
		}

		if (aitem->_type == mvAppItemType::mvPlot)
		{
			mvPlot* graph = static_cast<mvPlot*>(aitem);
			graph->SetColorMap((ImPlotColormap)source);
		}

		else if (aitem->_type == mvAppItemType::mvColorMapScale)
		{
			mvColorMapScale* graph = static_cast<mvColorMapScale*>(aitem);
			graph->setColorMap((ImPlotColormap)source);
		}

		else if (aitem->_type == mvAppItemType::mvColorMapButton)
		{
			mvColorMapButton* graph = static_cast<mvColorMapButton*>(aitem);
			graph->setColorMap((ImPlotColormap)source);
		}

		else if (aitem->_type == mvAppItemType::mvColorMapSlider)
		{
			mvColorMapSlider* graph = static_cast<mvColorMapSlider*>(aitem);
			graph->setColorMap((ImPlotColormap)source);
		}

		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_colormap",
				"Incompatible type. Expected types include: mvPlot, mvColorMapScale, mvColorMapButton", aitem);
			return GetPyNone();
		}


		return GetPyNone();
	}

	mv_internal mv_python_function
	sample_colormap(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		float t;

		if (!Parse((GetParsers())["sample_colormap"], args, kwargs, __FUNCTION__, &itemraw, &t))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (item > 15)
		{
			auto asource = GetItem((*GContext->itemRegistry), item);
			if (asource == nullptr)
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "sample_colormap",
					"Source Item not found: " + std::to_string(item), nullptr);
				return GetPyNone();
			}

			if (asource->_type == mvAppItemType::mvColorMap)
			{
				mvColorMap* colormap = static_cast<mvColorMap*>(asource);
				item = colormap->_colorMap;
			}
		}


		if (!GContext->started)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "sample_colormap", "This command can only be ran once the app is started.", nullptr);
			return GetPyNone();
		}

		ImVec4 result = ImPlot::SampleColormap(t, (ImPlotColormap)item);
		mvColor resultColor = mvColor(result.x, result.y, result.z, result.w);
		return ToPyColor(resultColor);
	}

	mv_internal mv_python_function
	get_colormap_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		int index;

		if (!Parse((GetParsers())["get_colormap_color"], args, kwargs, __FUNCTION__, &itemraw, &index))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (item > 15)
		{
			auto asource = GetItem((*GContext->itemRegistry), item);
			if (asource == nullptr)
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_colormap_color",
					"Source Item not found: " + std::to_string(item), nullptr);
				return GetPyNone();
			}

			if (asource->_type == mvAppItemType::mvColorMap)
			{
				mvColorMap* colormap = static_cast<mvColorMap*>(asource);
				item = colormap->_colorMap;
			}
		}


		ImVec4 result = ImPlot::GetColormapColor(index, (ImPlotColormap)item);
		mvColor resultColor = mvColor(result.x, result.y, result.z, result.w);
		return ToPyColor(resultColor);
	}

	mv_internal mv_python_function
	get_file_dialog_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* file_dialog_raw;

		if (!Parse((GetParsers())["get_file_dialog_info"], args, kwargs, __FUNCTION__, &file_dialog_raw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID file_dialog = GetIDFromPyObject(file_dialog_raw);

		auto aplot = GetItem(*GContext->itemRegistry, file_dialog);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvNone, std::to_string(file_dialog) + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvFileDialog)
		{
			mvThrowPythonError(mvErrorCode::mvNone, std::to_string(file_dialog) + " is not a plot.");
			return GetPyNone();
		}

		mvFileDialog* graph = static_cast<mvFileDialog*>(aplot);

		return graph->getInfoDict();
	}

	mv_internal mv_python_function
	set_x_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		float value;

		if (!Parse((GetParsers())["set_x_scroll"], args, kwargs, __FUNCTION__,
			&itemraw, &value))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto window = GetItem((*GContext->itemRegistry), item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_x_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->_type == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			pWindow->_scrollX = value;
			pWindow->_scrollXSet = true;
		}
		else if (window->_type == mvAppItemType::mvChildWindow)
		{
			auto pChild = static_cast<mvChildWindow*>(window);

			pChild->setScrollX(value);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_x_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_y_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		float value;

		if (!Parse((GetParsers())["set_y_scroll"], args, kwargs, __FUNCTION__,
			&itemraw, &value))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto window = GetItem((*GContext->itemRegistry), item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_y_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->_type == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			pWindow->_scrollY = value;
			pWindow->_scrollYSet = true;
		}
		else if (window->_type == mvAppItemType::mvChildWindow)
		{
			auto pChild = static_cast<mvChildWindow*>(window);

			pChild->setScrollY(value);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_y_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_x_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["get_x_scroll"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto window = GetItem((*GContext->itemRegistry), item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_x_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->_type == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollX);
		}
		else if (window->_type == mvAppItemType::mvChildWindow)
		{
			auto pChild = static_cast<mvChildWindow*>(window);

			return ToPyFloat(pChild->getScrollX());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_x_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_y_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["get_y_scroll"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto window = GetItem((*GContext->itemRegistry), item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_y_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->_type == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollY);
		}
		else if (window->_type == mvAppItemType::mvChildWindow)
		{
			auto pChild = static_cast<mvChildWindow*>(window);

			return ToPyFloat(pChild->getScrollY());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_y_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_x_scroll_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["get_x_scroll_max"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto window = GetItem((*GContext->itemRegistry), item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_x_scroll_max",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->_type == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollMaxX);
		}
		else if (window->_type == mvAppItemType::mvChildWindow)
		{
			auto pChild = static_cast<mvChildWindow*>(window);

			return ToPyFloat(pChild->getScrollXMax());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_x_scroll_max",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_y_scroll_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["get_y_scroll_max"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto window = GetItem((*GContext->itemRegistry), item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_y_scroll_max",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->_type == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollMaxY);
		}
		else if (window->_type == mvAppItemType::mvChildWindow)
		{
			auto pChild = static_cast<mvChildWindow*>(window);

			return ToPyFloat(pChild->getScrollYMax());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_y_scroll_max",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_clip_space(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (aitem->_type == mvAppItemType::mvDrawLayer)
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

	mv_internal mv_python_function
	apply_transform(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (aitem->_type == mvAppItemType::mvDrawNode)
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

	mv_internal mv_python_function
	create_rotation_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	create_perspective_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	create_orthographic_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	create_translation_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	create_scale_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	create_lookat_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	create_fps_matrix(PyObject* self, PyObject* args, PyObject* kwargs)
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

	mv_internal mv_python_function
	bind_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["bind_font"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (item == 0)
		{
			for (auto& reg : GContext->itemRegistry->fontRegistryRoots)
				static_cast<mvFontRegistry*>(reg.get())->resetFont();
			return GetPyNone();
		}

		auto aplot = GetItem((*GContext->itemRegistry), item);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_font",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvFont)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_font",
				"Incompatible type. Expected types include: mvFont", aplot);
			return GetPyNone();
		}

		mvFont* graph = static_cast<mvFont*>(aplot);

		graph->_default = true;
		mvToolManager::GetFontManager()._newDefault = true;

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_text_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* text;
		float wrap_width = -1.0f;
		PyObject* fontRaw;

		if (!Parse((GetParsers())["get_text_size"], args, kwargs, __FUNCTION__,
			&text, &wrap_width, &fontRaw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID font = GetIDFromPyObject(fontRaw);

		if (font == 0)
		{
			if (GContext->frame < 1)
				return GetPyNone();

			ImVec2 size = ImGui::CalcTextSize(text, 0, false, wrap_width);
			return ToPyPair(size.x, size.y);
		}

		auto afont = GetItem((*GContext->itemRegistry), font);
		if (afont == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_text_size",
				"Item not found: " + std::to_string(font), nullptr);
			return GetPyNone();
		}

		if (afont->_type != mvAppItemType::mvFont)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_text_size",
				"Incompatible type. Expected types include: mvFont", afont);
			return GetPyNone();
		}

		mvFont* graph = static_cast<mvFont*>(afont);

		if (graph->_fontPtr)
		{
			ImGuiContext& g = *GImGui;
			ImFont* previousFont = g.Font;
			float previousFontSize = g.FontSize;
			g.Font = graph->_fontPtr;
			g.FontSize = graph->_size;
			ImVec2 size = ImGui::CalcTextSize(text, 0, false, wrap_width);
			g.Font = previousFont;
			g.FontSize = previousFontSize;
			return ToPyPair(size.x, size.y);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* node_editor_raw;

		if (!Parse((GetParsers())["get_selected_nodes"], args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

		auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_selected_nodes",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->_type != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_selected_nodes",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);

		auto selected_nodes = editor->getSelectedNodes();

		return ToPyList(selected_nodes);

	}

	mv_internal mv_python_function
	get_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* node_editor_raw;

		if (!Parse((GetParsers())["get_selected_links"], args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

		auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_selected_links",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->_type != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_selected_links",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);

		auto selected_links = editor->getSelectedLinks();

		return ToPyList(selected_links);
	}

	mv_internal mv_python_function
	clear_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* node_editor_raw;

		if (!Parse((GetParsers())["clear_selected_links"], args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

		auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "clear_selected_links",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->_type != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "clear_selected_links",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);
		editor->clearLinks();

		return GetPyNone();

	}

	mv_internal mv_python_function
	clear_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* node_editor_raw;

		if (!Parse((GetParsers())["clear_selected_nodes"], args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

		auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "clear_selected_nodes",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->_type != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "clear_selected_nodes",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);
		editor->clearNodes();

		return GetPyNone();

	}

	mv_internal mv_python_function
	is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;

		if (!Parse((GetParsers())["is_plot_queried"], args, kwargs, __FUNCTION__, &plotraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID plot = GetIDFromPyObject(plotraw);

		auto aplot = GetItem(*GContext->itemRegistry, plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_plot_queried",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlot)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_plot_queried",
				"Incompatible type. Expected types include: mvPlot", aplot);
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return ToPyBool(graph->isPlotQueried());
	}

	mv_internal mv_python_function
	get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;

		if (!Parse((GetParsers())["get_plot_query_area"], args, kwargs, __FUNCTION__, &plotraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID plot = GetIDFromPyObject(plotraw);

		auto aplot = GetItem(*GContext->itemRegistry, plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_plot_query_area",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlot)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_plot_queried",
				"Incompatible type. Expected types include: mvPlot", aplot);
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		double* result = graph->getPlotQueryArea();
		return Py_BuildValue("(dddd)", result[0], result[1], result[2], result[3]);
	}

	mv_internal mv_python_function
	set_axis_ticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;
		PyObject* label_pairs;

		if (!Parse((GetParsers())["set_axis_ticks"], args, kwargs, __FUNCTION__, &plotraw, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID plot = GetIDFromPyObject(plotraw);

		auto aplot = GetItem(*GContext->itemRegistry, plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_axis_ticks",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_axis_ticks",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}
		graph->setYTicks(labels, locations);

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_axis_limits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* axisraw;
		float ymin;
		float ymax;

		if (!Parse((GetParsers())["set_axis_limits"], args, kwargs, __FUNCTION__, &axisraw, &ymin, &ymax))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID axis = GetIDFromPyObject(axisraw);

		auto aplot = GetItem(*GContext->itemRegistry, axis);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_axis_limits",
				"Item not found: " + std::to_string(axis), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_axis_limits",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->setLimits(ymin, ymax);

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_axis_limits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* axisraw;

		if (!Parse((GetParsers())["set_axis_limits_auto"], args, kwargs, __FUNCTION__, &axisraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID axis = GetIDFromPyObject(axisraw);

		auto aplot = GetItem(*GContext->itemRegistry, axis);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_axis_limits",
				"Item not found: " + std::to_string(axis), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_axis_limits",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->setLimitsAuto();

		return GetPyNone();
	}

	mv_internal mv_python_function
	fit_axis_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* axisraw;

		if (!Parse((GetParsers())["fit_axis_data"], args, kwargs, __FUNCTION__, &axisraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID axis = GetIDFromPyObject(axisraw);

		auto aplot = GetItem(*GContext->itemRegistry, axis);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "fit_axis_data",
				"Item not found: " + std::to_string(axis), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "fit_axis_data",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->fitAxisData();

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_axis_limits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;

		if (!Parse((GetParsers())["get_axis_limits"], args, kwargs, __FUNCTION__, &plotraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID plot = GetIDFromPyObject(plotraw);

		auto aplot = GetItem(*GContext->itemRegistry, plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_axis_limits",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_axis_limits",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		const ImVec2& lim = graph->getYLimits();
		return ToPyPair(lim.x, lim.y);
	}

	mv_internal mv_python_function
	reset_axis_ticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* plotraw;

		if (!Parse((GetParsers())["reset_axis_ticks"], args, kwargs, __FUNCTION__, &plotraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID plot = GetIDFromPyObject(plotraw);

		auto aplot = GetItem(*GContext->itemRegistry, plot);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "reset_axis_ticks",
				"Item not found: " + std::to_string(plot), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvPlotAxis)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "reset_axis_ticks",
				"Incompatible type. Expected types include: mvPlotAxis", aplot);
			return GetPyNone();
		}

		mvPlotAxis* graph = static_cast<mvPlotAxis*>(aplot);

		graph->resetYTicks();

		return GetPyNone();
	}

	mv_internal mv_python_function
	highlight_table_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int column = 0;
		PyObject* color;

		if (!Parse((GetParsers())["highlight_table_column"], args, kwargs, __FUNCTION__, &tableraw, &column, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "highlight_table_column",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "highlight_table_column",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (column >= tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "highlight_table_column",
				"Column out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_columnColorsSet[column] = true;
		tablecast->_columnColors[column] = finalColor;

		return GetPyNone();
	}

	mv_internal mv_python_function
	unhighlight_table_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int column = 0;

		if (!Parse((GetParsers())["unhighlight_table_column"], args, kwargs, __FUNCTION__, &tableraw, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unhighlight_table_column",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unhighlight_table_column",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (column >= tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "unhighlight_table_column",
				"Column out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_columnColorsSet[column] = false;

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_table_row_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		PyObject* color;

		if (!Parse((GetParsers())["set_table_row_color"], args, kwargs, __FUNCTION__, &tableraw, &row, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_table_row_color",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_table_row_color",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "set_table_row_color",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_rowColorsSet[row] = finalColor.a >= 0.0f;
		tablecast->_rowColors[row] = finalColor;

		return GetPyNone();
	}

	mv_internal mv_python_function
	unset_table_row_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;

		if (!Parse((GetParsers())["unset_table_row_color"], args, kwargs, __FUNCTION__, &tableraw, &row))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unset_table_row_color",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unset_table_row_color",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "set_table_row_color",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_rowColorsSet[row] = false;
		return GetPyNone();
	}

	mv_internal mv_python_function
	highlight_table_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		PyObject* color;

		if (!Parse((GetParsers())["highlight_table_row"], args, kwargs, __FUNCTION__, &tableraw, &row, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "highlight_table_row",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "highlight_table_row",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "highlight_table_row",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_rowSelectionColorsSet[row] = true;
		tablecast->_rowSelectionColors[row] = finalColor;

		return GetPyNone();
	}

	mv_internal mv_python_function
	unhighlight_table_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;

		if (!Parse((GetParsers())["unhighlight_table_row"], args, kwargs, __FUNCTION__, &tableraw, &row))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unhighlight_table_row",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unhighlight_table_row",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "unselect_table_row",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_rowSelectionColorsSet[row] = false;

		return GetPyNone();
	}

	mv_internal mv_python_function
	highlight_table_cell(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		int column = 0;
		PyObject* color;

		if (!Parse((GetParsers())["highlight_table_cell"], args, kwargs, __FUNCTION__, &tableraw, &row, &column, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "highlight_table_cell",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "highlight_table_cell",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows || column > tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "highlight_table_cell",
				"Row/Column out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_cellColorsSet[row][column] = true;
		tablecast->_cellColors[row][column] = finalColor;

		return GetPyNone();
	}

	mv_internal mv_python_function
	unhighlight_table_cell(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		int column = 0;

		if (!Parse((GetParsers())["unhighlight_table_cell"], args, kwargs, __FUNCTION__, &tableraw, &row, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unhighlight_table_cell",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unhighlight_table_cell",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows || column > tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "unhighlight_table_cell",
				"Row/Column out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_cellColorsSet[row][column] = false;

		return GetPyNone();
	}

	mv_internal mv_python_function
	is_table_cell_highlighted(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		int column = 0;

		if (!Parse((GetParsers())["is_table_cell_highlighted"], args, kwargs, __FUNCTION__, &tableraw, &row, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_table_cell_highlighted",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_table_cell_highlighted",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows || column > tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "is_table_cell_highlighted",
				"Row/Column out of range", tablecast);
			return GetPyNone();
		}

		if (tablecast->_cellColorsSet[row][column])
			return ToPyBool(true);
		if (tablecast->_rowColorsSet[row])
			return ToPyBool(true);
		if (tablecast->_columnColorsSet[column])
			return ToPyBool(true);

		return ToPyBool(false);
	}

	mv_internal mv_python_function
	is_table_row_highlighted(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;

		if (!Parse((GetParsers())["is_table_row_highlighted"], args, kwargs, __FUNCTION__, &tableraw, &row))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_table_row_highlighted",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_table_row_highlighted",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "is_table_row_highlighted",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		return ToPyBool(tablecast->_rowSelectionColorsSet[row]);
	}

	mv_internal mv_python_function
	is_table_column_highlighted(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int column = 0;

		if (!Parse((GetParsers())["is_table_column_highlighted"], args, kwargs, __FUNCTION__, &tableraw, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_table_column_highlighted",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->_type != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_table_column_highlighted",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (column >= tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "is_table_column_highlighted",
				"Column out of range", tablecast);
			return GetPyNone();
		}

		return ToPyBool(tablecast->_columnColorsSet[column]);
	}

	mv_internal mv_python_function
	bind_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["bind_theme"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (item == 0)
		{
			GContext->resetTheme = true;
			ResetTheme((*GContext->itemRegistry));
			return GetPyNone();
		}

		auto aplot = GetItem((*GContext->itemRegistry), item);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_theme",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (aplot->_type != mvAppItemType::mvTheme)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_theme",
				"Incompatible type. Expected types include: mvTheme", aplot);
			return GetPyNone();
		}

		mvTheme* graph = static_cast<mvTheme*>(aplot);

		ResetTheme(*GContext->itemRegistry);

		graph->_show = true;

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!Parse((GetParsers())["set_global_font_scale"], args, kwargs, __FUNCTION__, &scale))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		mvToolManager::GetFontManager().setGlobalFontScale(scale);

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvToolManager::GetFontManager().getGlobalFontScale());
	}

	mv_internal mv_python_function
	show_tool(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* toolraw;

		if (!Parse((GetParsers())["show_tool"], args, kwargs, __FUNCTION__,
			&toolraw))
			return GetPyNone();

		mvUUID tool = GetIDFromPyObject(toolraw);

		mvToolManager::ShowTool(tool);

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_frame_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 frame = 0;
		PyObject* callback;

		if (!Parse((GetParsers())["set_frame_callback"], args, kwargs, __FUNCTION__,
			&frame, &callback))
			return GetPyNone();

		if (frame > GContext->callbackRegistry->highestFrame)
			GContext->callbackRegistry->highestFrame = frame;

		// TODO: check previous entry and deprecate if existing
		Py_XINCREF(callback);
		mvSubmitCallback([=]()
			{
				GContext->callbackRegistry->frameCallbacks[frame] = callback;
			});

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!Parse((GetParsers())["set_exit_callback"], args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		Py_XINCREF(callback);
		mvSubmitCallback([=]()
			{
				GContext->callbackRegistry->onCloseCallback = SanitizeCallback(callback);
			});
		return GetPyNone();
	}

	mv_internal mv_python_function
	set_viewport_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!Parse((GetParsers())["set_viewport_resize_callback"], args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvSubmitCallback([=]()
			{
				GContext->callbackRegistry->resizeCallback = SanitizeCallback(callback);
				return std::string();
			});

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_viewport_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		PyObject* pdict = PyDict_New();

		mvViewport* viewport = GContext->viewport;
		if (viewport)
		{
			PyDict_SetItemString(pdict, "clear_color", mvPyObject(ToPyColor(viewport->clearColor)));
			PyDict_SetItemString(pdict, "small_icon", mvPyObject(ToPyString(viewport->small_icon)));
			PyDict_SetItemString(pdict, "large_icon", mvPyObject(ToPyString(viewport->large_icon)));
			PyDict_SetItemString(pdict, "x_pos", mvPyObject(ToPyInt(viewport->xpos)));
			PyDict_SetItemString(pdict, "y_pos", mvPyObject(ToPyInt(viewport->ypos)));
			PyDict_SetItemString(pdict, "width", mvPyObject(ToPyInt(viewport->actualWidth)));
			PyDict_SetItemString(pdict, "height", mvPyObject(ToPyInt(viewport->actualHeight)));
			PyDict_SetItemString(pdict, "client_width", mvPyObject(ToPyInt(viewport->clientWidth)));
			PyDict_SetItemString(pdict, "client_height", mvPyObject(ToPyInt(viewport->clientHeight)));
			PyDict_SetItemString(pdict, "resizable", mvPyObject(ToPyBool(viewport->resizable)));
			PyDict_SetItemString(pdict, "vsync", mvPyObject(ToPyBool(viewport->vsync)));
			PyDict_SetItemString(pdict, "min_width", mvPyObject(ToPyInt(viewport->minwidth)));
			PyDict_SetItemString(pdict, "max_width", mvPyObject(ToPyInt(viewport->maxwidth)));
			PyDict_SetItemString(pdict, "min_height", mvPyObject(ToPyInt(viewport->minheight)));
			PyDict_SetItemString(pdict, "max_height", mvPyObject(ToPyInt(viewport->maxheight)));
			PyDict_SetItemString(pdict, "always_on_top", mvPyObject(ToPyBool(viewport->alwaysOnTop)));
			PyDict_SetItemString(pdict, "decorated", mvPyObject(ToPyBool(viewport->decorated)));
			PyDict_SetItemString(pdict, "title", mvPyObject(ToPyString(viewport->title)));
		}
		else
			mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

		return pdict;
	}

	mv_internal mv_python_function
	is_viewport_ok(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvViewport* viewport = GContext->viewport;
		if (viewport)
		{
			if (viewport->shown)
				return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	mv_internal mv_python_function
	create_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* title = "Dear PyGui";
		const char* small_icon = "";
		const char* large_icon = "";
		i32 width = 1280;
		i32 height = 800;
		i32 x_pos = 100;
		i32 y_pos = 100;
		i32 min_width = 250;
		i32 max_width = 10000;
		i32 min_height = 250;
		i32 max_height = 10000;

		b32 resizable = true;
		b32 vsync = true;
		b32 always_on_top = false;
		b32 decorated = true;

		PyObject* color = PyList_New(4);
		PyList_SetItem(color, 0, PyFloat_FromDouble(0.0));
		PyList_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyList_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyList_SetItem(color, 3, PyFloat_FromDouble(1.0));


		if (!Parse((GetParsers())["create_viewport"], args, kwargs, __FUNCTION__,
			&title, &small_icon, &large_icon, &width, &height, &x_pos, &y_pos, &min_width, &max_width, &min_height, &max_height,
			&resizable, &vsync, &always_on_top, &decorated, &color
		))
			return GetPyNone();

		mvViewport* viewport = mvCreateViewport(width, height);
		if (PyObject* item = PyDict_GetItemString(kwargs, "clear_color")) viewport->clearColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "small_icon")) viewport->small_icon = ToString(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "large_icon")) viewport->large_icon = ToString(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "x_pos")) { viewport->posDirty = true;  viewport->xpos = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "y_pos")) { viewport->posDirty = true;  viewport->ypos = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "width")) { viewport->sizeDirty = true;  viewport->actualWidth = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "height")) { viewport->sizeDirty = true;  viewport->actualHeight = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "resizable")) { viewport->modesDirty = true;  viewport->resizable = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "vsync")) viewport->vsync = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "min_width")) viewport->minwidth = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "max_width")) viewport->maxwidth = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "min_height")) viewport->minheight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "max_height")) viewport->maxheight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "always_on_top")) { viewport->modesDirty = true; viewport->alwaysOnTop = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "decorated")) { viewport->modesDirty = true; viewport->decorated = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(kwargs, "title")) { viewport->titleDirty = true; viewport->title = ToString(item); }

		GContext->viewport = viewport;

		return GetPyNone();
	}

	mv_internal mv_python_function
	show_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		b32 minimized = false;
		b32 maximized = false;

		if (!Parse((GetParsers())["show_viewport"], args, kwargs, __FUNCTION__,
			&minimized, &maximized))
			return GetPyNone();

		mvViewport* viewport = GContext->viewport;
		if (viewport)
		{
			mvShowViewport(minimized, maximized);
			viewport->shown = true;
		}
		else
			mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

		return GetPyNone();
	}

	mv_internal mv_python_function
	configure_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvViewport* viewport = GContext->viewport;
		if (viewport)
		{
			if (PyObject* item = PyDict_GetItemString(kwargs, "clear_color")) viewport->clearColor = ToColor(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "small_icon")) viewport->small_icon = ToString(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "large_icon")) viewport->large_icon = ToString(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "x_pos")) { viewport->posDirty = true;  viewport->xpos = ToInt(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "y_pos")) { viewport->posDirty = true;  viewport->ypos = ToInt(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "width")) { viewport->sizeDirty = true;  viewport->actualWidth = ToInt(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "height")) { viewport->sizeDirty = true;  viewport->actualHeight = ToInt(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "resizable")) { viewport->modesDirty = true;  viewport->resizable = ToBool(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "vsync")) viewport->vsync = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "min_width")) viewport->minwidth = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "max_width")) viewport->maxwidth = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "min_height")) viewport->minheight = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "max_height")) viewport->maxheight = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "always_on_top")) { viewport->modesDirty = true; viewport->alwaysOnTop = ToBool(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "decorated")) { viewport->modesDirty = true; viewport->decorated = ToBool(item); }
			if (PyObject* item = PyDict_GetItemString(kwargs, "title")) { viewport->titleDirty = true; viewport->title = ToString(item); }

		}
		else
			mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

		return GetPyNone();
	}

	mv_internal mv_python_function
	maximize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		mvSubmitTask([=]()
			{
				mvMaximizeViewport();
			});

		return GetPyNone();
	}

	mv_internal mv_python_function
	minimize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		mvSubmitTask([=]()
			{
				mvMinimizeViewport();
			});

		return GetPyNone();
	}

	mv_internal mv_python_function
	toggle_viewport_fullscreen(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		mvSubmitTask([=]()
			{
				mvToggleFullScreen();
			});

		return GetPyNone();
	}

	mv_internal mv_python_function
	save_init_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!Parse((GetParsers())["save_init_file"], args, kwargs, __FUNCTION__, &file))
			return GetPyNone();

		if (GContext->started)
			ImGui::SaveIniSettingsToDisk(file);
		else
			mvThrowPythonError(mvErrorCode::mvNone, "Dear PyGui must be started to use \"save_init_file\".");

		return GetPyNone();
	}

	mv_internal mv_python_function
	split_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 delay = 32;

		if (!Parse((GetParsers())["split_frame"], args, kwargs, __FUNCTION__,
			&delay))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		Py_BEGIN_ALLOW_THREADS;
		GContext->waitOneFrame = true;
		while (GContext->waitOneFrame)
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	mv_internal mv_python_function
	lock_mutex(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		GContext->mutex.lock();
		GContext->manualMutexControl = true;

		return GetPyNone();
	}

	mv_internal mv_python_function
	unlock_mutex(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		GContext->mutex.unlock();
		GContext->manualMutexControl = false;

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_frame_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		return ToPyInt(GContext->frame);
	}

	mv_internal mv_python_function
	load_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;
		f32 gamma = 1.0f;
		f32 gamma_scale = 1.0f;

		if (!Parse((GetParsers())["load_image"], args, kwargs, __FUNCTION__,
			&file, &gamma, &gamma_scale))
			return GetPyNone();

		// Vout = (Vin / 255)^v; Where v = gamma

		if (stbi_is_hdr(file))
		{
			stbi_hdr_to_ldr_gamma(gamma);
			stbi_hdr_to_ldr_scale(gamma_scale);
		}
		else
		{
			stbi_ldr_to_hdr_gamma(gamma);
			stbi_ldr_to_hdr_scale(gamma_scale);
		}


		// Load from disk into a raw RGBA buffer
		i32 image_width = 0;
		i32 image_height = 0;

		// automatic gamma correction
		f32* image_data = stbi_loadf(file, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return GetPyNone();

		PyObject* newbuffer = nullptr;
		PymvBuffer* newbufferview = nullptr;
		newbufferview = PyObject_New(PymvBuffer, &PymvBufferType);
		newbufferview->arr.length = image_width * image_height * 4;
		newbufferview->arr.data = (f32*)image_data;
		newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvBufferType);

		PyObject* result = PyTuple_New(4);
		PyTuple_SetItem(result, 0, Py_BuildValue("i", image_width));
		PyTuple_SetItem(result, 1, Py_BuildValue("i", image_height));
		PyTuple_SetItem(result, 2, PyLong_FromLong(4));
		PyTuple_SetItem(result, 3, newbuffer);

		return result;
	}

	mv_internal mv_python_function
	is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(GContext->started);
	}

	mv_internal mv_python_function
	setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		Py_BEGIN_ALLOW_THREADS;

		if (GContext->started)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "Cannot call \"setup_dearpygui\" while a Dear PyGUI app is already running.");
			return GetPyNone();
		}

		while (!GContext->itemRegistry->containers.empty())
			GContext->itemRegistry->containers.pop();
		MV_ITEM_REGISTRY_INFO("Container stack emptied.");

		GContext->started = true;
		GContext->future = std::async(std::launch::async, []() {return mvRunCallbacks(); });

		MV_CORE_INFO("application starting");

		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	mv_internal mv_python_function
	render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_PROFILE_SCOPE("Frame")

			Py_BEGIN_ALLOW_THREADS;
		auto window = GContext->viewport;
		mvRenderFrame();
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	mv_internal mv_python_function
	create_context(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		Py_BEGIN_ALLOW_THREADS;

		if (GContext)
		{
			assert(false);
		}
		else
		{

			GContext = new mvContext();

			GContext->itemRegistry = new mvItemRegistry();
			GContext->callbackRegistry = new mvCallbackRegistry();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImPlot::CreateContext();
			imnodes::CreateContext();
		}

		mvToolManager::GetFontManager()._dirty = true;

		Py_END_ALLOW_THREADS;
		return GetPyNone();
	}

	mv_internal mv_python_function
	destroy_context(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		//if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		Py_BEGIN_ALLOW_THREADS;

		if (GContext == nullptr)
		{
			assert(false);
		}

		else
		{
			// hacky fix, started was set to false
			// to exit the event loop, but needs to be
			// true in order to run DPG commands for the 
			// exit callback.
			GContext->started = true;
			mvSubmitCallback([=]() {
				mvRunCallback(GContext->callbackRegistry->onCloseCallback, 0, nullptr, nullptr);
				GContext->started = false;  // return to false after
				});

			imnodes::DestroyContext();
			ImPlot::DestroyContext();
			ImGui::DestroyContext();

			mvToolManager::Reset();
			ClearItemRegistry(*GContext->itemRegistry);



			//#define X(el) el::s_class_theme_component = nullptr; el::s_class_theme_disabled_component = nullptr;
			#define X(el) GetClassThemeComponent(mvAppItemType::el) = nullptr; GetDisabledClassThemeComponent(mvAppItemType::el) = nullptr;
			MV_ITEM_TYPES
			#undef X

			mvSubmitCallback([=]() {
				GContext->callbackRegistry->running = false;
					});
			if (GContext->future.valid())
				GContext->future.get();
			if (GContext->viewport)
				delete GContext->viewport;

			delete GContext->itemRegistry;
			delete GContext->callbackRegistry;
			delete GContext;
			GContext = nullptr;
		}
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	mv_internal mv_python_function
	stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		GContext->started = false;
		auto viewport = GContext->viewport;
		if (viewport)
			viewport->running = false;
		return GetPyNone();
	}

	mv_internal mv_python_function
	get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		return ToPyFloat((f32)GContext->time);
	}

	mv_internal mv_python_function
	get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		return ToPyFloat(GContext->deltaTime);

	}

	mv_internal mv_python_function
	get_frame_rate(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		return ToPyFloat((f32)GContext->framerate);

	}

	mv_internal mv_python_function
	generate_uuid(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyUUID(GenerateUUID());
	}

	mv_internal mv_python_function
	configure_app(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (kwargs == nullptr)
			return GetPyNone();

		if (VerifyKeywordArguments(GetParsers()["configure_app"], kwargs))
			return GetPyNone();

		if (PyArg_ValidateKeywordArguments(kwargs) == 0)
		{
			assert(false);
			mvThrowPythonError(mvErrorCode::mvNone, "Dictionary keywords must be strings");
			return GetPyNone();
		}

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		if (PyObject* item = PyDict_GetItemString(kwargs, "auto_device")) GContext->IO.info_auto_device = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "docking")) GContext->IO.docking = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "docking_space")) GContext->IO.dockingViewport = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "load_init_file"))
		{
			std::string load_init_file = ToString(item);
			GContext->IO.iniFile = load_init_file;
			GContext->IO.loadIniFile = true;
		}

		if (PyObject* item = PyDict_GetItemString(kwargs, "allow_alias_overwrites")) GContext->IO.allowAliasOverwrites = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "manual_alias_management")) GContext->IO.manualAliasManagement = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "skip_keyword_args")) GContext->IO.skipKeywordArgs = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "skip_positional_args")) GContext->IO.skipPositionalArgs = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "skip_required_args")) GContext->IO.skipRequiredArgs = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "auto_save_init_file")) GContext->IO.autoSaveIniFile = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "wait_for_input")) GContext->IO.waitForInput = ToBool(item);

		if (PyObject* item = PyDict_GetItemString(kwargs, "init_file")) GContext->IO.iniFile = ToString(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "device_name")) GContext->IO.info_device_name = ToString(item);
		if (PyObject* item = PyDict_GetItemString(kwargs, "device")) GContext->IO.info_device = ToInt(item);

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_app_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		PyObject* pdict = PyDict_New();
		PyDict_SetItemString(pdict, "auto_device", mvPyObject(ToPyBool(GContext->IO.info_auto_device)));
		PyDict_SetItemString(pdict, "docking", mvPyObject(ToPyBool(GContext->IO.docking)));
		PyDict_SetItemString(pdict, "docking_space", mvPyObject(ToPyBool(GContext->IO.docking)));
		PyDict_SetItemString(pdict, "load_init_file", mvPyObject(ToPyBool(GContext->IO.loadIniFile)));
		PyDict_SetItemString(pdict, "version", mvPyObject(ToPyString(MV_SANDBOX_VERSION)));
		PyDict_SetItemString(pdict, "major_version", mvPyObject(ToPyInt(MV_DPG_MAJOR_VERSION)));
		PyDict_SetItemString(pdict, "minor_version", mvPyObject(ToPyInt(MV_DPG_MINOR_VERSION)));
		PyDict_SetItemString(pdict, "init_file", mvPyObject(ToPyString(GContext->IO.iniFile)));
		PyDict_SetItemString(pdict, "platform", mvPyObject(ToPyString(MV_PLATFORM)));
		PyDict_SetItemString(pdict, "device", mvPyObject(ToPyInt(GContext->IO.info_device)));
		PyDict_SetItemString(pdict, "device_name", mvPyObject(ToPyString(GContext->IO.info_device_name)));
		PyDict_SetItemString(pdict, "allow_alias_overwrites", mvPyObject(ToPyBool(GContext->IO.allowAliasOverwrites)));
		PyDict_SetItemString(pdict, "manual_alias_management", mvPyObject(ToPyBool(GContext->IO.manualAliasManagement)));
		PyDict_SetItemString(pdict, "skip_keyword_args", mvPyObject(ToPyBool(GContext->IO.skipKeywordArgs)));
		PyDict_SetItemString(pdict, "skip_positional_args", mvPyObject(ToPyBool(GContext->IO.skipPositionalArgs)));
		PyDict_SetItemString(pdict, "skip_required_args", mvPyObject(ToPyBool(GContext->IO.skipRequiredArgs)));
		PyDict_SetItemString(pdict, "auto_save_init_file", mvPyObject(ToPyBool(GContext->IO.autoSaveIniFile)));
		PyDict_SetItemString(pdict, "wait_for_input", mvPyObject(ToPyBool(GContext->IO.waitForInput)));
		return pdict;
	}

	mv_internal mv_python_function
	get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		b32 local = true;

		if (!Parse((GetParsers())["get_mouse_pos"], args, kwargs, __FUNCTION__, &local))
			return GetPyNone();

		auto pos = mvVec2();

		if (local)
			pos = { (f32)GContext->input.mousePos.x, (f32)GContext->input.mousePos.y };
		else
			pos = { (f32)GContext->input.mouseGlobalPos.x, (f32)GContext->input.mouseGlobalPos.y };

		return ToPyPair(pos.x, pos.y);
	}

	mv_internal mv_python_function
	get_plot_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!Parse((GetParsers())["get_plot_mouse_pos"], args, kwargs, __FUNCTION__))
			return GetPyNone();

		mvVec2 pos = { (f32)GContext->input.mousePlotPos.x, (f32)GContext->input.mousePlotPos.y };

		return ToPyPair(pos.x, pos.y);
	}

	mv_internal mv_python_function
	get_drawing_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!Parse((GetParsers())["get_drawing_mouse_pos"], args, kwargs, __FUNCTION__))
			return GetPyNone();

		mvVec2 pos = { (f32)GContext->input.mouseDrawingPos.x, (f32)GContext->input.mouseDrawingPos.y };

		return ToPyPair(pos.x, pos.y);
	}

	mv_internal mv_python_function
	get_mouse_drag_delta(PyObject* self, PyObject* arg, PyObject* kwargss)
	{

		mvVec2 pos = { (f32)GContext->input.mouseDragDelta.x, (f32)GContext->input.mouseDragDelta.y };
		return ToPyPair(pos.x, pos.y);
	}

	mv_internal mv_python_function
	is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 key;

		if (!Parse((GetParsers())["is_key_pressed"], args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(GContext->input.keyspressed[key]);
	}

	mv_internal mv_python_function
	is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 key;

		if (!Parse((GetParsers())["is_key_released"], args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(GContext->input.keysreleased[key]);
	}

	mv_internal mv_python_function
	is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 key;

		if (!Parse((GetParsers())["is_key_down"], args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(GContext->input.keysdown[key]);
	}

	mv_internal mv_python_function
	is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 button;
		f32 threshold;

		if (!Parse((GetParsers())["is_mouse_button_dragging"], args, kwargs, __FUNCTION__, &button, &threshold))
			return GetPyNone();

		return ToPyBool((f32)GContext->input.mousedownduration[button] / 100.0f >= threshold);
	}

	mv_internal mv_python_function
	is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 button;

		if (!Parse((GetParsers())["is_mouse_button_down"], args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(GContext->input.mousedown[button]);
	}

	mv_internal mv_python_function
	is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 button;

		if (!Parse((GetParsers())["is_mouse_button_clicked"], args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(GContext->input.mouseclick[button]);
	}

	mv_internal mv_python_function
	is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 button;

		if (!Parse((GetParsers())["is_mouse_button_double_clicked"], args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(GContext->input.mousedoubleclick[button]);
	}

	mv_internal mv_python_function
	is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		i32 button;

		if (!Parse((GetParsers())["is_mouse_button_released"], args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(GContext->input.mousereleased[button]);
	}

	mv_internal mv_python_function
	pop_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		if (GContext->itemRegistry->containers.empty())
		{
			mvThrowPythonError(mvErrorCode::mvContainerStackEmpty, "No container to pop.");
			MV_ITEM_REGISTRY_WARN("No container to pop.");
			assert(false);
			return GetPyNone();
		}

		mvAppItem* item = GContext->itemRegistry->containers.top();
		GContext->itemRegistry->containers.pop();

		if (item)
			return ToPyUUID(item->_uuid);
		else
			return GetPyNone();

	}

	mv_internal mv_python_function
	empty_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		while (!GContext->itemRegistry->containers.empty())
			GContext->itemRegistry->containers.pop();
		return GetPyNone();
	}

	mv_internal mv_python_function
	top_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvAppItem* item = nullptr;
		if (!GContext->itemRegistry->containers.empty())
			item = GContext->itemRegistry->containers.top();

		if (item)
			return ToPyUUID(item->_uuid);
		else
			return GetPyNone();
	}

	mv_internal mv_python_function
	last_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->lastItemAdded);
	}

	mv_internal mv_python_function
	last_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->lastContainerAdded);
	}

	mv_internal mv_python_function
	last_root(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->lastRootAdded);
	}

	mv_internal mv_python_function
	push_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["push_container_stack"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		mvAppItem* parent = GetItem((*GContext->itemRegistry), item);
		if (parent)
		{
			if (GetEntityDesciptionFlags(parent->_type) & MV_ITEM_DESC_CONTAINER)
			{
				GContext->itemRegistry->containers.push(parent);
				return ToPyBool(true);
			}
		}
		return ToPyBool(false);
	}

	mv_internal mv_python_function
	set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		i32 value;

		if (!VerifyRequiredArguments(GetParsers()["set_primary_window"], args))
			return GetPyNone();

		if (!Parse((GetParsers())["set_primary_window"], args, kwargs, __FUNCTION__, &itemraw, &value))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		mvWindowAppItem* window = GetWindow(*GContext->itemRegistry, item);

		if (window)
		{
			if (window->getWindowAsMainStatus() == value)
				return GetPyNone();
			else
				window->setWindowAsMainStatus(value);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_primary_window",
				"Item not found: " + std::to_string(item), nullptr);
			assert(false);
		}

		// reset other windows
		for (auto& window : GContext->itemRegistry->windowRoots)
		{
			if (window->_uuid != item)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->activeWindow);
	}

	mv_internal mv_python_function
	move_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		PyObject* parentraw = nullptr;
		PyObject* beforeraw = nullptr;

		if (!Parse((GetParsers())["move_item"], args, kwargs, __FUNCTION__,
			&itemraw, &parentraw, &beforeraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvUUID parent = GetIDFromPyObject(parentraw);
		mvUUID before = GetIDFromPyObject(beforeraw);

		MoveItem((*GContext->itemRegistry), item, parent, before);

		return GetPyNone();
	}

	mv_internal mv_python_function
	delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		b32 childrenOnly = false;
		i32 slot = -1;

		if (!Parse((GetParsers())["delete_item"], args, kwargs, __FUNCTION__, &itemraw, &childrenOnly, &slot))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		DeleteItem((*GContext->itemRegistry), item, childrenOnly, slot);

		return GetPyNone();

	}

	mv_internal mv_python_function
	does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["does_item_exist"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (GetItem((*GContext->itemRegistry), item))
			return ToPyBool(true);
		return ToPyBool(false);
	}

	mv_internal mv_python_function
	move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["move_item_up"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		MoveItemUp((*GContext->itemRegistry), item);

		return GetPyNone();

	}

	mv_internal mv_python_function
	move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["move_item_down"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		MoveItemDown((*GContext->itemRegistry), item);

		return GetPyNone();
	}

	mv_internal mv_python_function
	reorder_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* containerraw;
		i32 slot = 0;
		PyObject* new_order = nullptr;

		if (!Parse((GetParsers())["reorder_items"], args, kwargs, __FUNCTION__,
			&containerraw, &slot, &new_order))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		auto anew_order = ToUUIDVect(new_order);
		mvUUID container = GetIDFromPyObject(containerraw);

		mvAppItem* parent = GetItem((*GContext->itemRegistry), container);

		std::vector<mvRef<mvAppItem>>& children = parent->_children[slot];

		std::vector<mvRef<mvAppItem>> newchildren;
		newchildren.reserve(children.size());

		// todo: better sorting algorithm
		for (const auto& item : anew_order)
		{
			for (auto& child : children)
			{
				if (child->_uuid == item)
				{
					newchildren.emplace_back(child);
					break;
				}
			}
		}
		children = newchildren;
		return GetPyNone();
	}

	mv_internal mv_python_function
	unstage(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw = nullptr;

		if (!Parse((GetParsers())["unstage"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		b8 item_found = false;
		for (auto& aitem : GContext->itemRegistry->stagingRoots)
		{
			if (aitem->_uuid == item && aitem->_type == mvAppItemType::mvStage)
			{
				for (auto& children : aitem->_children)
				{
					for (auto& child : children)
						AddItemWithRuntimeChecks(*GContext->itemRegistry, child, 0, 0);
				}

				item_found = true;
				break;
			}
		}

		if (item_found)
		{
			CleanUpItem(*GContext->itemRegistry, item);
			return GetPyNone();
		}

		mvThrowPythonError(mvErrorCode::mvItemNotFound, "unstage",
			"Stage not found: " + std::to_string(item), nullptr);
		assert(false);

		return GetPyNone();
	}

	mv_internal mv_python_function
	show_item_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw = nullptr;

		if (!Parse((GetParsers())["show_item_debug"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		auto actualItem = GetRefItem((*GContext->itemRegistry), item);
		if (actualItem)
		{
			actualItem->_showDebug = true;
			GContext->itemRegistry->debugWindows.push_back(actualItem);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "show_item_debug",
				"Item not found: " + std::to_string(item), nullptr);
		}

		return GetPyNone();
	}

	mv_internal void
	GetAllItemsRoot(std::vector<mvRef<mvAppItem>>& roots, std::vector<mvUUID>& childList)
	{
		// to help recursively retrieve children
		std::function<void(mvRef<mvAppItem>)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvRef<mvAppItem> item) {
			auto& children0 = item->_children[0];
			auto& children1 = item->_children[1];
			auto& children2 = item->_children[2];
			for (auto& child : children0)
			{
				childList.emplace_back(child->_uuid);
				if (GetEntityDesciptionFlags(child->_type) & MV_ITEM_DESC_CONTAINER)
					ChildRetriever(child);
			}
			for (auto& child : children1)
			{
				childList.emplace_back(child->_uuid);
				if (GetEntityDesciptionFlags(child->_type) & MV_ITEM_DESC_CONTAINER)
					ChildRetriever(child);
			}
			for (auto& child : children2)
			{
				childList.emplace_back(child->_uuid);
				if (GetEntityDesciptionFlags(child->_type) & MV_ITEM_DESC_CONTAINER)
					ChildRetriever(child);
			}

		};

		for (auto& root : roots)
		{
			childList.emplace_back(root->_uuid);
			ChildRetriever(root);
		}
	}

	mv_internal mv_python_function
	get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		std::vector<mvUUID> childList;

		GetAllItemsRoot(GContext->itemRegistry->colormapRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->filedialogRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->stagingRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->viewportMenubarRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->windowRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->fontRegistryRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->handlerRegistryRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->textureRegistryRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->valueRegistryRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->themeRegistryRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->itemTemplatesRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->itemHandlerRegistryRoots, childList);
		GetAllItemsRoot(GContext->itemRegistry->viewportDrawlistRoots, childList);

		return ToPyList(childList);
	}

	mv_internal mv_python_function
	show_imgui_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		GContext->itemRegistry->showImGuiDebug = true;
		return GetPyNone();
	}

	mv_internal mv_python_function
	show_implot_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		GContext->itemRegistry->showImPlotDebug = true;
		return GetPyNone();
	}

	mv_internal mv_python_function
	get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		std::vector<mvUUID> childList;
		for (auto& root : GContext->itemRegistry->colormapRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->filedialogRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->stagingRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->viewportMenubarRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->windowRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->fontRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->handlerRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->textureRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->valueRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->themeRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->itemTemplatesRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->itemHandlerRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : GContext->itemRegistry->viewportDrawlistRoots) childList.emplace_back(root->_uuid);

		return ToPyList(childList);
	}

	mv_internal mv_python_function
	add_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;
		PyObject* itemraw;

		if (!Parse((GetParsers())["add_alias"], args, kwargs, __FUNCTION__, &alias, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		AddAlias((*GContext->itemRegistry), alias, item);

		return GetPyNone();

	}

	mv_internal mv_python_function
	remove_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((GetParsers())["remove_alias"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		RemoveAlias((*GContext->itemRegistry), alias);

		return GetPyNone();

	}

	mv_internal mv_python_function
	does_alias_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((GetParsers())["does_alias_exist"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		bool result = GContext->itemRegistry->aliases.count(alias) != 0;

		return ToPyBool(result);
	}

	mv_internal mv_python_function
	get_alias_id(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((GetParsers())["get_alias_id"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID result = GetIdFromAlias((*GContext->itemRegistry), alias);

		return ToPyUUID(result);
	}

	mv_internal mv_python_function
	get_aliases(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		std::vector<std::string> aliases;

		for (const auto& alias : GContext->itemRegistry->aliases)
			aliases.push_back(alias.first);

		return ToPyList(aliases);
	}

	mv_internal mv_python_function
	bind_template_registry(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["bind_template_registry"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);


		if (item == 0)
			GContext->itemRegistry->boundedTemplateRegistry = nullptr;
		else
		{
			auto actualItem = GetRefItem((*GContext->itemRegistry), item);
			if (actualItem)
				GContext->itemRegistry->boundedTemplateRegistry = std::move(actualItem);
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_template_registry",
					"Item not found: " + std::to_string(item), nullptr);
				return GetPyNone();
			}
		}

		return GetPyNone();
	}

	mv_internal mv_python_function
	focus_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["focus_item"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (!GContext->started)
		{
			for (size_t i = 0; i < GContext->itemRegistry->windowRoots.size(); i++)
			{
				if (GContext->itemRegistry->windowRoots[i]->_uuid == item)
				{
					mvRef<mvAppItem> oldItem = GContext->itemRegistry->windowRoots.back();
					GContext->itemRegistry->windowRoots[GContext->itemRegistry->windowRoots.size() - 1] = GContext->itemRegistry->windowRoots[i];
					GContext->itemRegistry->windowRoots[i] = oldItem;
					break;
				}
			}
		}

		mvAppItem* appitem = GetItem(*GContext->itemRegistry, item);

		if (appitem)
		{
			appitem->_focusNextFrame = true;
			if (auto parent = GetItemRoot(*GContext->itemRegistry, item))
				parent->_focusNextFrame = true;
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "focus_item",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	mv_internal std::vector<std::vector<mvUUID>>
	GetItemChildren(mvItemRegistry& registry, mvUUID uuid)
	{

		mvAppItem* item = GetItem(registry, uuid);

		std::vector<std::vector<mvUUID>> childList;

		if (item)
		{
			for (auto& children : item->_children)
			{
				std::vector<mvUUID> childSlot;
				for (auto& child : children)
					childSlot.emplace_back(child->_uuid);
				childList.push_back(childSlot);
			}

		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_children",
				"Item not found: " + std::to_string(uuid), nullptr);
			assert(false && "Item not found.");
		}

		return childList;
	}

	mv_internal mv_python_function
	get_item_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["get_item_info"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		PyObject* pdict = PyDict_New();

		if (appitem)
		{

			std::string parserCommand = GetEntityCommand(appitem->_type);

			auto children = GetItemChildren(*GContext->itemRegistry, appitem->_uuid);
			if (children.empty())
				PyDict_SetItemString(pdict, "children", mvPyObject(GetPyNone()));
			else
			{
				PyObject* pyChildren = PyDict_New();
				i32 i = 0;
				for (const auto& slot : children)
				{
					PyDict_SetItem(pyChildren, ToPyInt(i), mvPyObject(ToPyList(slot)));
					i++;
				}
				PyDict_SetItemString(pdict, "children", mvPyObject(pyChildren));
			}

			PyDict_SetItemString(pdict, "type", mvPyObject(ToPyString(GetEntityTypeString(appitem->_type))));
			PyDict_SetItemString(pdict, "target", mvPyObject(ToPyInt(GetEntityTargetSlot(appitem->_type))));

			if (appitem->_parentPtr)
				PyDict_SetItemString(pdict, "parent", mvPyObject(ToPyUUID(appitem->_parentPtr->_uuid)));
			else
				PyDict_SetItemString(pdict, "parent", mvPyObject(GetPyNone()));

			if (appitem->_theme)
				PyDict_SetItemString(pdict, "theme", mvPyObject(ToPyUUID(appitem->_theme->_uuid)));
			else
				PyDict_SetItemString(pdict, "theme", mvPyObject(GetPyNone()));

			if (appitem->_font)
				PyDict_SetItemString(pdict, "font", mvPyObject(ToPyUUID(appitem->_font->_uuid)));
			else
				PyDict_SetItemString(pdict, "font", mvPyObject(GetPyNone()));

			if (GetEntityDesciptionFlags(appitem->_type) & MV_ITEM_DESC_CONTAINER)
				PyDict_SetItemString(pdict, "container", mvPyObject(ToPyBool(true)));
			else
				PyDict_SetItemString(pdict, "container", mvPyObject(ToPyBool(false)));

			i32 applicableState = GetApplicableState(appitem->_type);
			PyDict_SetItemString(pdict, "hover_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_HOVER)));
			PyDict_SetItemString(pdict, "active_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_ACTIVE)));
			PyDict_SetItemString(pdict, "focus_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_FOCUSED)));
			PyDict_SetItemString(pdict, "clicked_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_CLICKED)));
			PyDict_SetItemString(pdict, "visible_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_VISIBLE)));
			PyDict_SetItemString(pdict, "edited_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_EDITED)));
			PyDict_SetItemString(pdict, "activated_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_ACTIVATED)));
			PyDict_SetItemString(pdict, "deactivated_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_DEACTIVATED)));
			PyDict_SetItemString(pdict, "deactivatedae_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_DEACTIVATEDAE)));
			PyDict_SetItemString(pdict, "toggled_open_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_TOGGLED_OPEN)));
			PyDict_SetItemString(pdict, "resized_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_RECT_SIZE)));

		}

		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_info",
				"Item not found: " + std::to_string(item), nullptr);

		return pdict;
	}

	mv_internal mv_python_function
	get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["get_item_configuration"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		PyObject* pdict = PyDict_New();

		if (appitem)
		{
			// config py objects
			mvPyObject py_filter_key = ToPyString(appitem->_filter);
			mvPyObject py_payload_type = ToPyString(appitem->_payloadType);
			mvPyObject py_label = ToPyString(appitem->_specifiedLabel);
			mvPyObject py_use_internal_label = ToPyBool(appitem->_useInternalLabel);
			mvPyObject py_source = ToPyUUID(appitem->_source);
			mvPyObject py_show = ToPyBool(appitem->_show);
			mvPyObject py_enabled = ToPyBool(appitem->_enabled);
			mvPyObject py_tracked = ToPyBool(appitem->_tracked);
			mvPyObject py_width = ToPyInt(appitem->_width);
			mvPyObject py_track_offset = ToPyFloat(appitem->_trackOffset);
			mvPyObject py_height = ToPyInt(appitem->_height);
			mvPyObject py_indent = ToPyInt((i32)appitem->_indent);

			PyDict_SetItemString(pdict, "filter_key", py_filter_key);
			PyDict_SetItemString(pdict, "payload_type", py_payload_type);
			PyDict_SetItemString(pdict, "label", py_label);
			PyDict_SetItemString(pdict, "use_internal_label", py_use_internal_label);
			PyDict_SetItemString(pdict, "source", py_source);
			PyDict_SetItemString(pdict, "show", py_show);
			PyDict_SetItemString(pdict, "enabled", py_enabled);
			PyDict_SetItemString(pdict, "tracked", py_tracked);
			PyDict_SetItemString(pdict, "width", py_width);
			PyDict_SetItemString(pdict, "track_offset", py_track_offset);
			PyDict_SetItemString(pdict, "height", py_height);
			PyDict_SetItemString(pdict, "indent", py_indent);

			if (appitem->_callback)
			{
				Py_XINCREF(appitem->_callback);
				PyDict_SetItemString(pdict, "callback", appitem->_callback);
			}
			else
				PyDict_SetItemString(pdict, "callback", GetPyNone());

			if (appitem->_dropCallback)
			{
				Py_XINCREF(appitem->_dropCallback);
				PyDict_SetItemString(pdict, "drop_callback", appitem->_dropCallback);
			}
			else
				PyDict_SetItemString(pdict, "drop_callback", GetPyNone());

			if (appitem->_dragCallback)
			{
				Py_XINCREF(appitem->_dragCallback);
				PyDict_SetItemString(pdict, "drag_callback", appitem->_dragCallback);
			}
			else
				PyDict_SetItemString(pdict, "drag_callback", GetPyNone());

			if (appitem->_user_data)
			{
				Py_XINCREF(appitem->_user_data);
				PyDict_SetItemString(pdict, "user_data", appitem->_user_data);
			}
			else
				PyDict_SetItemString(pdict, "user_data", GetPyNone());

			appitem->getSpecificConfiguration(pdict);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_configuration",
				"Item not found: " + std::to_string(item), nullptr);

		return pdict;
	}

	mv_internal mv_python_function
	set_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* sourceraw;
		i32 slot;

		if (!Parse((GetParsers())["set_item_children"], args, kwargs, __FUNCTION__,
			&itemraw, &sourceraw, &slot))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvUUID source = GetIDFromPyObject(sourceraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		auto& staging = GContext->itemRegistry->stagingRoots;

		b8 stage_found = false;
		mvRef<mvAppItem> staging_container = nullptr;

		for (auto& stage : staging)
		{
			if (stage->_uuid == source)
			{
				staging_container = stage;
				stage_found = true;
				break;
			}
		}

		if (!stage_found)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_item_children",
				"Stage item not found: " + std::to_string(item), nullptr);
			assert(false);
			return GetPyNone();
		}


		if (appitem)
		{
			auto& oldChildren = appitem->_children[slot];
			oldChildren.reserve(staging_container->_children[slot].size());
			oldChildren = std::move(staging_container->_children[slot]);
			for (auto& child : oldChildren)
			{
				child->_parent = item;
				child->_parentPtr = appitem;
			}

			// update locations
			for (i32 i = 0; i < 4; i++)
			{
				i32 index = 0;
				for (auto& child : appitem->_children[i])
				{
					child->_location = index;
					index++;
				}
			}
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_item_children",
				"Item not found: " + std::to_string(item), nullptr);

		DeleteItem(*GContext->itemRegistry, source);

		return GetPyNone();
	}

	mv_internal mv_python_function
	bind_item_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* fontraw;

		if (!Parse((GetParsers())["bind_item_font"], args, kwargs, __FUNCTION__,
			&itemraw, &fontraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvUUID font = GetIDFromPyObject(fontraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);
		auto appfont = GetRefItem(*GContext->itemRegistry, font);

		if (appitem)
		{
			if (font == 0)
			{
				appitem->_font = nullptr;
				return GetPyNone();
			}

			if (appfont)
			{
				appitem->_font = appfont;
				appfont->onBind(appitem);
			}
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_font",
					"Font item not found: " + std::to_string(item), nullptr);
			}
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_font",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	mv_internal mv_python_function
	bind_item_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* themeraw;

		if (!Parse((GetParsers())["bind_item_theme"], args, kwargs, __FUNCTION__,
			&itemraw, &themeraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvUUID theme = GetIDFromPyObject(themeraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		if (appitem)
		{
			if (theme == 0)
			{
				appitem->_theme = nullptr;
				return GetPyNone();
			}

			auto apptheme = GetRefItem(*GContext->itemRegistry, theme);

			if (apptheme)
			{
				appitem->_theme = apptheme;
				apptheme->onBind(appitem);
				return GetPyNone();
			}
			else
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_theme",
					"Theme item not found: " + std::to_string(item), nullptr);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_theme",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	mv_internal mv_python_function
	bind_item_handler_registry(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* regraw;

		if (!Parse((GetParsers())["bind_item_handler_registry"], args, kwargs, __FUNCTION__,
			&itemraw, &regraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvUUID reg = GetIDFromPyObject(regraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		if (appitem)
		{
			if (reg == 0)
			{
				appitem->_handlerRegistry = nullptr;
				return GetPyNone();
			}

			auto apptheme = GetRefItem(*GContext->itemRegistry, reg);

			if (apptheme)
			{
				appitem->_handlerRegistry = apptheme;
				apptheme->onBind(appitem);
				return GetPyNone();
			}
			else
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_handler_registry",
					"Theme item not found: " + std::to_string(item), nullptr);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_handler_registry",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	mv_internal mv_python_function
	reset_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["reset_pos"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		if (appitem)
			appitem->_dirtyPos = false;
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "reset_pos",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_item_state(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["get_item_state"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		PyObject* pdict = PyDict_New();

		if (appitem)
			FillAppItemState(pdict, appitem->_state, GetApplicableState(appitem->_type));
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_state",
				"Item not found: " + std::to_string(item), nullptr);

		return pdict;
	}

	mv_internal mv_python_function
	get_item_types(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		PyObject* pdict = PyDict_New();
		#define X(el) PyDict_SetItemString(pdict, #el, PyLong_FromLong((int)mvAppItemType::el));
		MV_ITEM_TYPES
		#undef X

		return pdict;
	}

	mv_internal mv_python_function
	configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(PyTuple_GetItem(args, 0));
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

		if (appitem)
		{
			//appitem->checkArgs(args, kwargs);
			appitem->handleKeywordArgs(kwargs, GetEntityCommand(appitem->_type));
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "configure_item",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* nameraw;

		if (!Parse((GetParsers())["get_value"], args, kwargs, __FUNCTION__, &nameraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID name = GetIDFromPyObject(nameraw);
		mvAppItem* item = GetItem(*GContext->itemRegistry, name);
		if (item)
			return item->getPyValue();

		return GetPyNone();
	}

	mv_internal mv_python_function
	get_values(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* items;

		if (!Parse((GetParsers())["get_values"], args, kwargs, __FUNCTION__, &items))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		auto aitems = ToUUIDVect(items);
		PyObject* pyvalues = PyList_New(aitems.size());

		for (size_t i = 0; i < aitems.size(); i++)
		{
			auto item = GetItem(*GContext->itemRegistry, aitems[i]);
			if (item)
				PyList_SetItem(pyvalues, i, item->getPyValue());
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_values",
					"Item not found: " + std::to_string(aitems[i]), nullptr);
				PyList_SetItem(pyvalues, i, GetPyNone());
			}
		}

		return pyvalues;
	}

	mv_internal mv_python_function
	set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* nameraw;
		PyObject* value;

		if (!Parse((GetParsers())["set_value"], args, kwargs, __FUNCTION__, &nameraw, &value))
			return GetPyNone();

		if (value)
			Py_XINCREF(value);

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID name = GetIDFromPyObject(nameraw);

		mvAppItem* item = GetItem(*GContext->itemRegistry, name);
		if (item)
			item->setPyValue(value);
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_value",
				"Item not found: " + std::to_string(name), nullptr);
		}

		Py_XDECREF(value);

		return GetPyNone();
	}

	mv_internal mv_python_function
	set_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		const char* alias;

		if (!Parse((GetParsers())["set_item_alias"], args, kwargs, __FUNCTION__,
			&itemraw, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);
		if (appitem)
			appitem->_alias = alias;
		return GetPyNone();
	}

	mv_internal mv_python_function
	get_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["get_item_alias"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);
		mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);
		if (appitem)
			return ToPyString(appitem->_alias);
		return GetPyNone();
	}

	mv_internal mv_python_function
	capture_next_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callable;

		if (!Parse((GetParsers())["capture_next_item"], args, kwargs, __FUNCTION__,
			&callable))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		if (GContext->itemRegistry->captureCallback)
			Py_XDECREF(GContext->itemRegistry->captureCallback);

		Py_XINCREF(callable);
		if (callable == Py_None)
			GContext->itemRegistry->captureCallback = nullptr;
		else
			GContext->itemRegistry->captureCallback = callable;

		return GetPyNone();
	}

	mv_internal void
	InsertParser_Block0(std::map<std::string, mvPythonParser>& parsers)
	{
		//-----------------------------------------------------------------------------
		// callback registry
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "frame" });
			args.push_back({ mvPyDataType::Callable, "callback" });

			mvPythonParserSetup setup;
			setup.about = "Sets a callback to run on first frame.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::String;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_frame_callback", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Callable, "callback" });

			mvPythonParserSetup setup;
			setup.about = "Sets a callback to run on last frame.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::String;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_exit_callback", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Callable, "callback" });

			mvPythonParserSetup setup;
			setup.about = "Sets a callback to run on viewport resize.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::String;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_viewport_resize_callback", parser });
		}

		//-----------------------------------------------------------------------------
		// themes
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "theme" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global theme.";
			setup.category = { "Themes" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "bind_theme", parser });
		}

		//-----------------------------------------------------------------------------
		// tables
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "column" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Highlight specified table column.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "highlight_table_column", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Unhighlight specified table column.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "unhighlight_table_column", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Set table row color.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_table_row_color", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });

			mvPythonParserSetup setup;
			setup.about = "Remove user set table row color.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "unset_table_row_color", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(4);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::Integer, "column" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Highlight specified table cell.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "highlight_table_cell", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Unhighlight specified table cell.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "unhighlight_table_cell", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Highlight specified table row.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "highlight_table_row", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });

			mvPythonParserSetup setup;
			setup.about = "Unhighlight specified table row.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "unhighlight_table_row", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Checks if a table column is highlighted.";
			setup.category = { "Tables", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_table_column_highlighted", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });

			mvPythonParserSetup setup;
			setup.about = "Checks if a table row is highlighted.";
			setup.category = { "Tables", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_table_row_highlighted", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Checks if a table cell is highlighted.";
			setup.category = { "Tables", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_table_cell_highlighted", parser });
		}

		//-----------------------------------------------------------------------------
		// plots
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "plot" });

			mvPythonParserSetup setup;
			setup.about = "Returns true if the plot is currently being queried. (Requires plot 'query' kwarg to be enabled)";
			setup.category = { "Plotting", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_plot_queried", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "plot" });

			mvPythonParserSetup setup;
			setup.about = "Returns the last/current query area of the plot. (Requires plot 'query' kwarg to be enabled)";
			setup.category = { "Plotting", "App Item Operations" };
			setup.returnType = mvPyDataType::FloatList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_plot_query_area", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "axis" });

			mvPythonParserSetup setup;
			setup.about = "Get the specified axis limits.";
			setup.category = { "Plotting", "App Item Operations" };
			setup.returnType = mvPyDataType::FloatList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_axis_limits", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "axis" });
			args.push_back({ mvPyDataType::Float, "ymin" });
			args.push_back({ mvPyDataType::Float, "ymax" });

			mvPythonParserSetup setup;
			setup.about = "Sets limits on the axis for pan and zoom.";
			setup.category = { "Plotting", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_axis_limits", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "axis" });

			mvPythonParserSetup setup;
			setup.about = "Removes all limits on specified axis.";
			setup.category = { "Plotting", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_axis_limits_auto", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "axis" });

			mvPythonParserSetup setup;
			setup.about = "Sets the axis boundaries max/min in the data series currently on the plot.";
			setup.category = { "Plotting", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "fit_axis_data", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "axis" });

			mvPythonParserSetup setup;
			setup.about = "Removes the manually set axis ticks and applies the default axis ticks";
			setup.category = { "Plotting", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "reset_axis_ticks", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "axis" });
			args.push_back({ mvPyDataType::Object, "label_pairs", mvArgType::REQUIRED_ARG, "...", "Tuples of label and value in the form '((label, axis_value), (label, axis_value), ...)'" });

			mvPythonParserSetup setup;
			setup.about = "Replaces axis ticks with 'label_pairs' argument.";
			setup.category = { "Plotting", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_axis_ticks", parser });
		}

		//-----------------------------------------------------------------------------
		// viewport
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.reserve(16);
			args.push_back({ mvPyDataType::String, "title", mvArgType::KEYWORD_ARG, "'Dear PyGui'", "Sets the title of the viewport." });
			args.push_back({ mvPyDataType::String, "small_icon", mvArgType::KEYWORD_ARG, "''", "Sets the small icon that is found in the viewport's decorator bar. Must be ***.ico on windows and either ***.ico or ***.png on mac." });
			args.push_back({ mvPyDataType::String, "large_icon", mvArgType::KEYWORD_ARG, "''", "Sets the large icon that is found in the task bar while the app is running. Must be ***.ico on windows and either ***.ico or ***.png on mac." });
			args.push_back({ mvPyDataType::Integer, "width", mvArgType::KEYWORD_ARG, "1280", "Sets the width of the drawable space on the viewport. Does not inclue the border." });
			args.push_back({ mvPyDataType::Integer, "height", mvArgType::KEYWORD_ARG, "800", "Sets the height of the drawable space on the viewport. Does not inclue the border or decorator bar." });
			args.push_back({ mvPyDataType::Integer, "x_pos", mvArgType::KEYWORD_ARG, "100", "Sets x position the viewport will be drawn in screen coordinates." });
			args.push_back({ mvPyDataType::Integer, "y_pos", mvArgType::KEYWORD_ARG, "100", "Sets y position the viewport will be drawn in screen coordinates." });
			args.push_back({ mvPyDataType::Integer, "min_width", mvArgType::KEYWORD_ARG, "250", "Applies a minimuim limit to the width of the viewport." });
			args.push_back({ mvPyDataType::Integer, "max_width", mvArgType::KEYWORD_ARG, "10000", "Applies a maximum limit to the width of the viewport." });
			args.push_back({ mvPyDataType::Integer, "min_height", mvArgType::KEYWORD_ARG, "250", "Applies a minimuim limit to the height of the viewport." });
			args.push_back({ mvPyDataType::Integer, "max_height", mvArgType::KEYWORD_ARG, "10000", "Applies a maximum limit to the height of the viewport." });
			args.push_back({ mvPyDataType::Bool, "resizable", mvArgType::KEYWORD_ARG, "True", "Enables and Disables user ability to resize the viewport." });
			args.push_back({ mvPyDataType::Bool, "vsync", mvArgType::KEYWORD_ARG, "True", "Enables and Disables the renderloop vsync limit. vsync frame value is set by refresh rate of display." });
			args.push_back({ mvPyDataType::Bool, "always_on_top", mvArgType::KEYWORD_ARG, "False", "Forces the viewport to always be drawn ontop of all other viewports." });
			args.push_back({ mvPyDataType::Bool, "decorated", mvArgType::KEYWORD_ARG, "True", "Enabled and disabled the decorator bar at the top of the viewport." });
			args.push_back({ mvPyDataType::FloatList, "clear_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 255)", "Sets the color of the back of the viewport." });

			mvPythonParserSetup setup;
			setup.about = "Creates a viewport. Viewports are required.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "create_viewport", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Bool, "minimized", mvArgType::KEYWORD_ARG, "False", "Sets the state of the viewport to minimized" });
			args.push_back({ mvPyDataType::Bool, "maximized", mvArgType::KEYWORD_ARG, "False", "Sets the state of the viewport to maximized" });
			args.push_back({ mvPyDataType::UUID, "viewport", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });

			mvPythonParserSetup setup;
			setup.about = "Shows the main viewport.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "show_viewport", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Configures a viewport.";
			setup.category = { "General" };
			setup.unspecifiedKwargs = true;
			setup.internal = true;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "configure_viewport", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns a viewport's configuration.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_viewport_configuration", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Checks if a viewport has been created and shown.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_viewport_ok", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Maximizes the viewport.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "maximize_viewport", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Minimizes a viewport.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "minimize_viewport", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Toggle viewport fullscreen mode..";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "toggle_viewport_fullscreen", parser });
		}
	}

	mv_internal void
	InsertParser_Block1(std::map<std::string, mvPythonParser>& parsers)
	{
		//-----------------------------------------------------------------------------
		// context
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns app configuration.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_app_configuration", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(11);
			args.push_back({ mvPyDataType::Bool, "docking", mvArgType::KEYWORD_ARG, "False", "Enables docking support." });
			args.push_back({ mvPyDataType::Bool, "docking_space", mvArgType::KEYWORD_ARG, "False", "add explicit dockspace over viewport" });
			args.push_back({ mvPyDataType::String, "load_init_file", mvArgType::KEYWORD_ARG, "''", "Load .ini file." });
			args.push_back({ mvPyDataType::String, "init_file", mvArgType::KEYWORD_ARG, "''" });
			args.push_back({ mvPyDataType::Bool, "auto_save_init_file", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Integer, "device", mvArgType::KEYWORD_ARG, "-1", "Which display adapter to use. (-1 will use default)" });
			args.push_back({ mvPyDataType::Bool, "auto_device", mvArgType::KEYWORD_ARG, "False", "Let us pick the display adapter." });
			args.push_back({ mvPyDataType::Bool, "allow_alias_overwrites", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Bool, "manual_alias_management", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Bool, "skip_required_args", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Bool, "skip_positional_args", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Bool, "skip_keyword_args", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Bool, "wait_for_input", mvArgType::KEYWORD_ARG, "False", "New in 1.1. Only update when user input occurs" });

			mvPythonParserSetup setup;
			setup.about = "Configures app.";
			setup.category = { "General" };
			setup.unspecifiedKwargs = true;
			setup.internal = true;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "configure_app", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "file" });

			mvPythonParserSetup setup;
			setup.about = "Save dpg.ini file.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "save_init_file", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "delay", mvArgType::KEYWORD_ARG, "32", "Minimal delay in in milliseconds" });

			mvPythonParserSetup setup;
			setup.about = "Waits one frame.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "split_frame", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns frame count.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Integer;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_frame_count", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::String, "file" });
			args.push_back({ mvPyDataType::Float, "gamma", mvArgType::KEYWORD_ARG, "1.0", "Gamma correction factor. (default is 1.0 to avoid automatic gamma correction on loading." });
			args.push_back({ mvPyDataType::Float, "gamma_scale_factor", mvArgType::KEYWORD_ARG, "1.0", "Gamma scale factor." });

			mvPythonParserSetup setup;
			setup.about = "Loads an image. Returns width, height, channels, mvBuffer";
			setup.category = { "Textures", "Utilities" };
			setup.returnType = mvPyDataType::Object;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "load_image", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Generate a new UUID.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "generate_uuid", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Locks render thread mutex.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "lock_mutex", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Unlocks render thread mutex";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "unlock_mutex", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Checks if Dear PyGui is running";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_dearpygui_running", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Sets up Dear PyGui";
			setup.category = { "General" };

			args.push_back({ mvPyDataType::UUID, "viewport", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "setup_dearpygui", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Render a single Dear PyGui frame.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "render_dearpygui_frame", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Destroys the Dear PyGui context.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "destroy_context", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Creates the Dear PyGui context.";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "create_context", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Stops Dear PyGui";
			setup.category = { "General" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "stop_dearpygui", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns total time since Dear PyGui has started.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Float;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_total_time", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns time since last frame.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Float;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_delta_time", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the average frame rate across 120 frames.";
			setup.category = { "General" };
			setup.returnType = mvPyDataType::Float;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_frame_rate", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Bool, "local", mvArgType::KEYWORD_ARG, "True" });

			mvPythonParserSetup setup;
			setup.about = "Returns mouse position.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::IntList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_mouse_pos", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns mouse position in plot.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::IntList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_plot_mouse_pos", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns mouse position in drawing.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::IntList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_drawing_mouse_pos", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns mouse drag delta.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Float;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_mouse_drag_delta", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "button" });
			args.push_back({ mvPyDataType::Float, "threshold" });

			mvPythonParserSetup setup;
			setup.about = "Checks if mouse button is down and dragging.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_mouse_button_dragging", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "button" });

			mvPythonParserSetup setup;
			setup.about = "Checks if mouse button is down.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_mouse_button_down", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "button" });

			mvPythonParserSetup setup;
			setup.about = "Checks if mouse button is clicked.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_mouse_button_clicked", parser });
		}


		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "button" });

			mvPythonParserSetup setup;
			setup.about = "Checks if mouse button is released.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_mouse_button_released", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "button" });

			mvPythonParserSetup setup;
			setup.about = "Checks if mouse button is double clicked.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_mouse_button_double_clicked", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "key" });

			mvPythonParserSetup setup;
			setup.about = "Checks if key is pressed.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_key_pressed", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "key" });

			mvPythonParserSetup setup;
			setup.about = "Checks if key is released.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_key_released", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Integer, "key" });

			mvPythonParserSetup setup;
			setup.about = "Checks if key is down.";
			setup.category = { "Input Polling" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "is_key_down", parser });
		}
	}

	mv_internal void
	InsertParser_Block2(std::map<std::string, mvPythonParser>& parsers)
	{
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Adds an alias.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "add_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::Callable, "callback" });

			mvPythonParserSetup setup;
			setup.about = "Captures the next item.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "capture_next_item", parser });
		}


		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Removes an alias.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "remove_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Checks if an alias exist.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "does_alias_exist", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Returns the ID associated with an alias.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_alias_id", parser });
		}

		{

			mvPythonParserSetup setup;
			setup.about = "Pops the top item off the parent stack and return its ID.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, {});
			parsers.insert({ "pop_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Shows the imgui demo.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "show_imgui_demo", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Shows the implot demo.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "show_implot_demo", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "container" });
			args.push_back({ mvPyDataType::Integer, "slot" });
			args.push_back({ mvPyDataType::UUIDList, "new_order" });

			mvPythonParserSetup setup;
			setup.about = "Reorders an item's children.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "reorder_items", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "template_registry" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global template registry.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "bind_template_registry", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Unstages an item.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "unstage", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Shows an item's debug window";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "show_item_debug", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Pushes an item onto the container stack.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "push_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the item on the top of the container stack.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "top_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the last item added.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "last_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the last container item added.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "last_container", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the last root added (registry or window).";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "last_root", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Emptyes the container stack.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "empty_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "0" });
			args.push_back({ mvPyDataType::UUID, "before", mvArgType::KEYWORD_ARG, "0" });

			mvPythonParserSetup setup;
			setup.about = "Moves an item to a new location.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "move_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns all windows.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUIDList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_windows", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns all items.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUIDList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_all_items", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns all aliases.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::StringList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_aliases", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::Bool, "children_only", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Integer, "slot", mvArgType::KEYWORD_ARG, "-1" });

			mvPythonParserSetup setup;
			setup.about = "Deletes an item..";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "delete_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Checks if an item exist..";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "does_item_exist", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Moves an item up.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "move_item_up", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Moves an item down.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "move_item_down", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the active window.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_active_window", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "window" });
			args.push_back({ mvPyDataType::Bool, "value" });

			mvPythonParserSetup setup;
			setup.about = "Sets the primary window.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_primary_window", parser });
		}
	}

	mv_internal void
	InsertParser_Block3(std::map<std::string, mvPythonParser>& parsers)
	{

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Focuses an item.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "focus_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's information.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_item_info", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's configuration.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_item_configuration", parser });
		}

		{

			mvPythonParserSetup setup;
			setup.about = "Returns an item types.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, {});
			parsers.insert({ "get_item_types", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "source" });
			args.push_back({ mvPyDataType::Integer, "slot" });

			mvPythonParserSetup setup;
			setup.about = "Sets an item's children.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_item_children", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "font" });

			mvPythonParserSetup setup;
			setup.about = "Sets an item's font.";
			setup.category = { "Fonts", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "bind_item_font", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Sets an item's alias.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_item_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's alias.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::String;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_item_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "handler_registry" });

			mvPythonParserSetup setup;
			setup.about = "Binds an item handler registry to an item.";
			setup.category = { "App Item Operations", "Events" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "bind_item_handler_registry", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "theme" });

			mvPythonParserSetup setup;
			setup.about = "Binds a theme to an item.";
			setup.category = { "App Item Operations", "Themes" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "bind_item_theme", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's state.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_item_state", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Configures an item.";
			setup.category = { "App Item Operations" };
			setup.unspecifiedKwargs = true;
			setup.internal = true;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "configure_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's value.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Any;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_value", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUIDList, "items" });

			mvPythonParserSetup setup;
			setup.about = "Returns values of a list of items.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Any;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_values", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::Object, "value" });

			mvPythonParserSetup setup;
			setup.about = "Set's an item's value.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_value", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Resets an item's position after using 'set_item_pos'.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "reset_pos", parser });
		}
	}

	mv_internal void
	InsertParser_Block4(std::map<std::string, mvPythonParser>& parsers)
	{
		//-----------------------------------------------------------------------------
		// node editor
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "node_editor" });

			mvPythonParserSetup setup;
			setup.about = "Returns a node editor's selected nodes.";
			setup.category = { "Node Editor", "App Item Operations" };
			setup.returnType = mvPyDataType::UUIDList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_selected_nodes", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "node_editor" });

			mvPythonParserSetup setup;
			setup.about = "Returns a node editor's selected links.";
			setup.category = { "Node Editor", "App Item Operations" };
			setup.returnType = mvPyDataType::ListStrList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_selected_links", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "node_editor" });

			mvPythonParserSetup setup;
			setup.about = "Clears a node editor's selected links.";
			setup.category = { "Node Editor", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "clear_selected_links", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "node_editor" });

			mvPythonParserSetup setup;
			setup.about = "Clears a node editor's selected nodes.";
			setup.category = { "Node Editor", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "clear_selected_nodes", parser });
		}

		//-----------------------------------------------------------------------------
		// fonts
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::Float, "scale" });

			mvPythonParserSetup setup;
			setup.about = "Sets global font scale.";
			setup.category = { "Fonts" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_global_font_scale", parser });
		}

		{

			mvPythonParserSetup setup;
			setup.about = "Returns global font scale.";
			setup.category = { "Fonts" };
			setup.returnType = mvPyDataType::Float;
			mvPythonParser parser = FinalizeParser(setup, {});
			parsers.insert({ "get_global_font_scale", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "font" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global font.";
			setup.category = { "Fonts" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "bind_font", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "text" });
			args.push_back({ mvPyDataType::Float, "wrap_width", mvArgType::KEYWORD_ARG, "-1.0", "Wrap width to use (-1.0 turns wrap off)." });
			args.push_back({ mvPyDataType::UUID, "font", mvArgType::KEYWORD_ARG, "0", "Font to use." });

			mvPythonParserSetup setup;
			setup.about = "Returns width/height of text with specified font (must occur after 1st frame).";
			setup.category = { "Fonts" };
			setup.returnType = mvPyDataType::FloatList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_text_size", parser });
		}

		//-----------------------------------------------------------------------------
		// drawings
		//-----------------------------------------------------------------------------
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

			parsers.insert({ "set_clip_space", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "Drawing node to apply transform to." });
			args.push_back({ mvPyDataType::Object, "transform", mvArgType::REQUIRED_ARG, "", "Transformation matrix." });

			mvPythonParserSetup setup;
			setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
			setup.category = { "Drawlist", "Matrix Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "apply_transform", parser });
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

			parsers.insert({ "create_rotation_matrix", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::FloatList, "scales", mvArgType::REQUIRED_ARG, "", "scale values per axis" });

			mvPythonParserSetup setup;
			setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
			setup.category = { "Drawlist", "Matrix Operations" };
			setup.returnType = mvPyDataType::Object;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "create_scale_matrix", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::FloatList, "translation", mvArgType::REQUIRED_ARG, "", "translation vector" });

			mvPythonParserSetup setup;
			setup.about = "New in 1.1. Creates a translation matrix.";
			setup.category = { "Drawlist", "Matrix Operations" };
			setup.returnType = mvPyDataType::Object;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "create_translation_matrix", parser });
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

			parsers.insert({ "create_lookat_matrix", parser });
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

			parsers.insert({ "create_perspective_matrix", parser });
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

			parsers.insert({ "create_orthographic_matrix", parser });
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

			parsers.insert({ "create_fps_matrix", parser });
		}

		//-----------------------------------------------------------------------------
		// windows
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::Float, "value" });

			mvPythonParserSetup setup;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_x_scroll", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::Float, "value" });
			mvPythonParserSetup setup;
			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "set_y_scroll", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID , "item" });
			mvPythonParserSetup setup;
			setup.returnType = mvPyDataType::Float;
			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_x_scroll", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			mvPythonParserSetup setup;
			setup.returnType = mvPyDataType::Float;
			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_y_scroll", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID , "item" });
			mvPythonParserSetup setup;
			setup.returnType = mvPyDataType::Float;
			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_x_scroll_max", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			mvPythonParserSetup setup;
			setup.returnType = mvPyDataType::Float;
			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_y_scroll_max", parser });
		}

		//-----------------------------------------------------------------------------
		// file dialogs
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "file_dialog" });

			mvPythonParserSetup setup;
			setup.about = "Returns information related to the file dialog. Typically used while the file dialog is in use to query data about the state or info related to the file dialog.";
			setup.category = { "Widgets", "File Dialog" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers.insert({ "get_file_dialog_info", parser });
		}

		//-----------------------------------------------------------------------------
		// color maps
		//-----------------------------------------------------------------------------
		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "item that the color map will be applied to" });
			args.push_back({ mvPyDataType::UUID, "source", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry.  Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });

			mvPythonParserSetup setup;
			setup.about = "Sets the color map for widgets that accept it.";
			setup.category = { "Widget Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "bind_colormap", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry. Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });
			args.push_back({ mvPyDataType::Float, "t", mvArgType::REQUIRED_ARG, "", "Value of the colormap to sample between 0.0-1.0" });

			mvPythonParserSetup setup;
			setup.about = "Returns a color from a colormap given t between 0.0-1.0.";
			setup.category = { "Widget Operations" };
			setup.returnType = mvPyDataType::IntList;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "sample_colormap", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry. Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });
			args.push_back({ mvPyDataType::Integer, "index", mvArgType::REQUIRED_ARG, "", "Desired position of the color in the colors list value of the colormap being quiered " });

			mvPythonParserSetup setup;
			setup.about = "Returns a color from a colormap given an index >= 0. (ex. 0 will be the first color in the color list of the color map) Modulo will be performed against the number of items in the color list.";
			setup.category = { "Widget Operations" };
			setup.returnType = mvPyDataType::IntList;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "get_colormap_color", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "tool" });

			mvPythonParserSetup setup;
			setup.about = "Shows a built in tool.";
			setup.category = { "Widgets" };
			setup.returnType = mvPyDataType::String;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers.insert({ "show_tool", parser });
		}
	}

	const std::map<std::string, mvPythonParser>& 
	GetModuleParsers()
	{

		mv_local_persist auto parsers = std::map<std::string, mvPythonParser>();

		if (parsers.empty())
		{
			#define X(el) parsers.insert({GetEntityCommand(mvAppItemType::el), GetEntityParser(mvAppItemType::el)});
			MV_ITEM_TYPES
			#undef X

			// to prevent exceeding function stack size
			InsertParser_Block0(parsers);
			InsertParser_Block1(parsers);
			InsertParser_Block2(parsers);
			InsertParser_Block3(parsers);
			InsertParser_Block4(parsers);
		}
		return parsers;
	}

	const std::vector<std::pair<std::string, long>>&
	GetModuleConstants()
	{
		mv_local_persist bool First_Run = true;
		mv_local_persist std::vector<std::pair<std::string, long>> ModuleConstants;

		if (First_Run)
		{
			InsertConstants_mvContext(ModuleConstants);

			ModuleConstants.push_back({ "mvAll", 0L });
			ModuleConstants.push_back({"mvTool_About", MV_TOOL_ABOUT_UUID });
			ModuleConstants.push_back({"mvTool_Debug", MV_TOOL_DEBUG_UUID });
			ModuleConstants.push_back({"mvTool_Doc", MV_TOOL_DOC_UUID });
			ModuleConstants.push_back({"mvTool_ItemRegistry", MV_TOOL_ITEM_REGISTRY_UUID });
			ModuleConstants.push_back({"mvTool_Metrics", MV_TOOL_METRICS_UUID });
			ModuleConstants.push_back({"mvTool_Style", MV_TOOL_STYLE_UUID });
			ModuleConstants.push_back({"mvTool_Font", MV_TOOL_FONT_UUID });
			ModuleConstants.push_back({"mvFontAtlas", MV_ATLAS_UUID });
			ModuleConstants.push_back({"mvAppUUID", MV_APP_UUID });
			ModuleConstants.push_back({"mvInvalidUUID", MV_INVALID_UUID });
			ModuleConstants.push_back({"mvDir_None", ImGuiDir_None });
			ModuleConstants.push_back({"mvDir_Left", ImGuiDir_Left });
			ModuleConstants.push_back({"mvDir_Right", ImGuiDir_Right });
			ModuleConstants.push_back({"mvDir_Up", ImGuiDir_Up });
			ModuleConstants.push_back({"mvDir_Down", ImGuiDir_Down });
			ModuleConstants.push_back({"mvComboHeight_Small", 0L });
			ModuleConstants.push_back({"mvComboHeight_Regular", 1L });
			ModuleConstants.push_back({"mvComboHeight_Large", 2L });
			ModuleConstants.push_back({"mvComboHeight_Largest", 3L });

			ModuleConstants.push_back({"mvColorEdit_AlphaPreviewNone", 0L });
			ModuleConstants.push_back({"mvColorEdit_AlphaPreview", ImGuiColorEditFlags_AlphaPreview });
			ModuleConstants.push_back({"mvColorEdit_AlphaPreviewHalf", ImGuiColorEditFlags_AlphaPreviewHalf });
			ModuleConstants.push_back({"mvColorEdit_uint8", ImGuiColorEditFlags_Uint8});
			ModuleConstants.push_back({"mvColorEdit_float", ImGuiColorEditFlags_Float});
			ModuleConstants.push_back({"mvColorEdit_rgb", ImGuiColorEditFlags_DisplayRGB});
			ModuleConstants.push_back({"mvColorEdit_hsv", ImGuiColorEditFlags_DisplayHSV});
			ModuleConstants.push_back({"mvColorEdit_hex", ImGuiColorEditFlags_DisplayHex});
			ModuleConstants.push_back({"mvColorEdit_input_rgb", ImGuiColorEditFlags_InputRGB});
			ModuleConstants.push_back({"mvColorEdit_input_hsv", ImGuiColorEditFlags_InputHSV});

			ModuleConstants.push_back({ "mvPlotColormap_Default", 0L }); // ImPlot default colormap         (n=10)
			ModuleConstants.push_back({ "mvPlotColormap_Deep", 0L }); // ImPlot default colormap         (n=10)
			ModuleConstants.push_back({ "mvPlotColormap_Dark", 1L }); // a.k.a. matplotlib "Set1"        (n=9)
			ModuleConstants.push_back({ "mvPlotColormap_Pastel", 2L}); // a.k.a. matplotlib "Pastel1"     (n=9)
			ModuleConstants.push_back({ "mvPlotColormap_Paired", 3L}); // a.k.a. matplotlib "Paired"      (n=12)
			ModuleConstants.push_back({ "mvPlotColormap_Viridis", 4L }); // a.k.a. matplotlib "viridis"     (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Plasma", 5L }); // a.k.a. matplotlib "plasma"      (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Hot", 6L }); // a.k.a. matplotlib/MATLAB "hot"  (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Cool", 7L }); // a.k.a. matplotlib/MATLAB "cool" (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Pink", 8L }); // a.k.a. matplotlib/MATLAB "pink" (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Jet", 9L }); // a.k.a. MATLAB "jet"             (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Twilight", 10L }); // a.k.a. MATLAB "jet"             (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_RdBu", 11L }); // a.k.a. MATLAB "jet"             (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_BrBG", 12L }); // a.k.a. MATLAB "jet"             (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_PiYG", 13L }); // a.k.a. MATLAB "jet"             (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Spectral", 14L }); // a.k.a. MATLAB "jet"             (n=11)
			ModuleConstants.push_back({ "mvPlotColormap_Greys", 15L }); // a.k.a. MATLAB "jet"             (n=11)

			ModuleConstants.push_back({ "mvColorPicker_bar", ImGuiColorEditFlags_PickerHueBar });
			ModuleConstants.push_back({ "mvColorPicker_wheel", ImGuiColorEditFlags_PickerHueWheel });

			ModuleConstants.push_back({ "mvTabOrder_Reorderable", 0L });
			ModuleConstants.push_back({ "mvTabOrder_Fixed", 1L });
			ModuleConstants.push_back({ "mvTabOrder_Leading", 2L });
			ModuleConstants.push_back({ "mvTabOrder_Trailing", 3L });

			ModuleConstants.push_back({ "mvDatePickerLevel_Day", 0L });
			ModuleConstants.push_back({ "mvDatePickerLevel_Month", 1L });
			ModuleConstants.push_back({ "mvDatePickerLevel_Year", 2L });

			ModuleConstants.push_back({ "mvCullMode_None", 0L });
			ModuleConstants.push_back({ "mvCullMode_Back", 1L });
			ModuleConstants.push_back({ "mvCullMode_Front", 2L });

			ModuleConstants.push_back({ "mvFontRangeHint_Default", 0L });
			ModuleConstants.push_back({ "mvFontRangeHint_Japanese", 1L });
			ModuleConstants.push_back({ "mvFontRangeHint_Korean", 2L });
			ModuleConstants.push_back({ "mvFontRangeHint_Chinese_Full", 3L });
			ModuleConstants.push_back({ "mvFontRangeHint_Chinese_Simplified_Common", 4L });
			ModuleConstants.push_back({ "mvFontRangeHint_Cyrillic", 5L });
			ModuleConstants.push_back({ "mvFontRangeHint_Thai", 6L });
			ModuleConstants.push_back({ "mvFontRangeHint_Vietnamese", 7L });

			ModuleConstants.push_back({ "mvNode_PinShape_Circle", imnodes::PinShape::PinShape_Circle });
			ModuleConstants.push_back({ "mvNode_PinShape_CircleFilled", imnodes::PinShape::PinShape_CircleFilled });
			ModuleConstants.push_back({ "mvNode_PinShape_Triangle", imnodes::PinShape::PinShape_Triangle });
			ModuleConstants.push_back({ "mvNode_PinShape_TriangleFilled", imnodes::PinShape::PinShape_TriangleFilled });
			ModuleConstants.push_back({ "mvNode_PinShape_Quad", imnodes::PinShape::PinShape_Quad });
			ModuleConstants.push_back({ "mvNode_PinShape_QuadFilled", imnodes::PinShape::PinShape_QuadFilled });

			ModuleConstants.push_back({ "mvNode_Attr_Input", 0L });
			ModuleConstants.push_back({ "mvNode_Attr_Output", 1L});
			ModuleConstants.push_back({ "mvNode_Attr_Static", 2L});

			ModuleConstants.push_back({ "mvPlotBin_Sqrt", -1L });
			ModuleConstants.push_back({ "mvPlotBin_Sturges", -2L });
			ModuleConstants.push_back({ "mvPlotBin_Rice", -3L });
			ModuleConstants.push_back({ "mvPlotBin_Scott", -4L });

			ModuleConstants.push_back({ "mvXAxis", 0L});
			ModuleConstants.push_back({ "mvYAxis", 1L});

			ModuleConstants.push_back({ "mvPlotMarker_None", -1L});  // no marker
			ModuleConstants.push_back({ "mvPlotMarker_Circle",  0L});  // a circle marker will be rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Square",  1L});  // a square maker will be rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Diamond",  2L});  // a diamond marker will be rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Up",  3L});  // an upward-pointing triangle marker will up rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Down",  4L});  // an downward-pointing triangle marker will up rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Left",  5L});  // an leftward-pointing triangle marker will up rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Right",  6L});  // an rightward-pointing triangle marker will up rendered at each point
			ModuleConstants.push_back({ "mvPlotMarker_Cross",  7L});  // a cross marker will be rendered at each point (not filled)
			ModuleConstants.push_back({ "mvPlotMarker_Plus",  8L});  // a plus marker will be rendered at each point (not filled)
			ModuleConstants.push_back({ "mvPlotMarker_Asterisk",  9L}); // a asterisk marker will be rendered at each point (not filled)

			ModuleConstants.push_back({ "mvPlot_Location_Center", ImPlotLocation_Center });
			ModuleConstants.push_back({ "mvPlot_Location_North", ImPlotLocation_North });
			ModuleConstants.push_back({ "mvPlot_Location_South", ImPlotLocation_South });
			ModuleConstants.push_back({ "mvPlot_Location_West", ImPlotLocation_West});
			ModuleConstants.push_back({ "mvPlot_Location_East", ImPlotLocation_East});
			ModuleConstants.push_back({ "mvPlot_Location_NorthWest", ImPlotLocation_NorthWest});
			ModuleConstants.push_back({ "mvPlot_Location_NorthEast", ImPlotLocation_NorthEast});
			ModuleConstants.push_back({ "mvPlot_Location_SouthWest", ImPlotLocation_SouthWest});
			ModuleConstants.push_back({ "mvPlot_Location_SouthEast", ImPlotLocation_SouthEast});

			ModuleConstants.push_back({ "mvTable_SizingFixedFit", ImGuiTableFlags_SizingFixedFit });
			ModuleConstants.push_back({ "mvTable_SizingFixedSame", ImGuiTableFlags_SizingFixedSame });
			ModuleConstants.push_back({ "mvTable_SizingStretchProp", ImGuiTableFlags_SizingStretchProp});
			ModuleConstants.push_back({ "mvTable_SizingStretchSame", ImGuiTableFlags_SizingStretchSame});

			ModuleConstants.push_back({ "mvFormat_Float_rgba", 0L });
			ModuleConstants.push_back({ "mvFormat_Float_rgb", 1L });

			ModuleConstants.push_back({ "mvThemeCat_Core", 0L });
			ModuleConstants.push_back({ "mvThemeCat_Plots", 1L});
			ModuleConstants.push_back({ "mvThemeCat_Nodes", 2L});

			ModuleConstants.push_back({ "mvThemeCol_Text", ImGuiCol_Text });
			ModuleConstants.push_back({ "mvThemeCol_TextDisabled", ImGuiCol_TextDisabled });
			ModuleConstants.push_back({ "mvThemeCol_WindowBg", ImGuiCol_WindowBg });            // Background of normal windows
			ModuleConstants.push_back({ "mvThemeCol_ChildBg", ImGuiCol_ChildBg });              // Background of child windows
			ModuleConstants.push_back({ "mvThemeCol_Border", ImGuiCol_Border });                // Background of popups, menus, tooltips windows
			ModuleConstants.push_back({ "mvThemeCol_PopupBg", ImGuiCol_PopupBg });              // Background of popups, menus, tooltips windows
			ModuleConstants.push_back({ "mvThemeCol_BorderShadow", ImGuiCol_BorderShadow });
			ModuleConstants.push_back({ "mvThemeCol_FrameBg", ImGuiCol_FrameBg });             // Background of checkbox, radio button, plot, slider, text input
			ModuleConstants.push_back({ "mvThemeCol_FrameBgHovered", ImGuiCol_FrameBgHovered });
			ModuleConstants.push_back({ "mvThemeCol_FrameBgActive", ImGuiCol_FrameBgActive });
			ModuleConstants.push_back({ "mvThemeCol_TitleBg", ImGuiCol_TitleBg });
			ModuleConstants.push_back({ "mvThemeCol_TitleBgActive", ImGuiCol_TitleBgActive });
			ModuleConstants.push_back({ "mvThemeCol_TitleBgCollapsed", ImGuiCol_TitleBgCollapsed });
			ModuleConstants.push_back({ "mvThemeCol_MenuBarBg", ImGuiCol_MenuBarBg });
			ModuleConstants.push_back({ "mvThemeCol_ScrollbarBg", ImGuiCol_ScrollbarBg });
			ModuleConstants.push_back({ "mvThemeCol_ScrollbarGrab", ImGuiCol_ScrollbarGrab });
			ModuleConstants.push_back({ "mvThemeCol_ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered });
			ModuleConstants.push_back({ "mvThemeCol_ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive });
			ModuleConstants.push_back({ "mvThemeCol_CheckMark", ImGuiCol_CheckMark });
			ModuleConstants.push_back({ "mvThemeCol_SliderGrab", ImGuiCol_SliderGrab });
			ModuleConstants.push_back({ "mvThemeCol_SliderGrabActive", ImGuiCol_SliderGrabActive });
			ModuleConstants.push_back({ "mvThemeCol_Button", ImGuiCol_Button });
			ModuleConstants.push_back({ "mvThemeCol_ButtonHovered", ImGuiCol_ButtonHovered });
			ModuleConstants.push_back({ "mvThemeCol_ButtonActive", ImGuiCol_ButtonActive });
			ModuleConstants.push_back({ "mvThemeCol_Header", ImGuiCol_Header });              // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
			ModuleConstants.push_back({ "mvThemeCol_HeaderHovered", ImGuiCol_HeaderHovered });
			ModuleConstants.push_back({ "mvThemeCol_HeaderActive", ImGuiCol_HeaderActive });
			ModuleConstants.push_back({ "mvThemeCol_Separator", ImGuiCol_Separator });
			ModuleConstants.push_back({ "mvThemeCol_SeparatorHovered", ImGuiCol_SeparatorHovered });
			ModuleConstants.push_back({ "mvThemeCol_SeparatorActive", ImGuiCol_SeparatorActive });
			ModuleConstants.push_back({ "mvThemeCol_ResizeGrip", ImGuiCol_ResizeGrip });
			ModuleConstants.push_back({ "mvThemeCol_ResizeGripHovered", ImGuiCol_ResizeGripHovered });
			ModuleConstants.push_back({ "mvThemeCol_ResizeGripActive", ImGuiCol_ResizeGripActive });
			ModuleConstants.push_back({ "mvThemeCol_Tab", ImGuiCol_Tab });
			ModuleConstants.push_back({ "mvThemeCol_TabHovered", ImGuiCol_TabHovered });
			ModuleConstants.push_back({ "mvThemeCol_TabActive", ImGuiCol_TabActive });
			ModuleConstants.push_back({ "mvThemeCol_TabUnfocused", ImGuiCol_TabUnfocused });
			ModuleConstants.push_back({ "mvThemeCol_TabUnfocusedActive", ImGuiCol_TabUnfocusedActive });
			ModuleConstants.push_back({ "mvThemeCol_DockingPreview", ImGuiCol_DockingPreview });      // Preview overlay color when about to docking something
			ModuleConstants.push_back({ "mvThemeCol_DockingEmptyBg", ImGuiCol_DockingEmptyBg });      // Background color for empty node (e.g. CentralNode with no window docked into it)
			ModuleConstants.push_back({ "mvThemeCol_PlotLines", ImGuiCol_PlotLines });
			ModuleConstants.push_back({ "mvThemeCol_PlotLinesHovered", ImGuiCol_PlotLinesHovered });
			ModuleConstants.push_back({ "mvThemeCol_PlotHistogram", ImGuiCol_PlotHistogram });
			ModuleConstants.push_back({ "mvThemeCol_PlotHistogramHovered", ImGuiCol_PlotHistogramHovered });
			ModuleConstants.push_back({ "mvThemeCol_TableHeaderBg", ImGuiCol_TableHeaderBg });           // Table header background
			ModuleConstants.push_back({ "mvThemeCol_TableBorderStrong", ImGuiCol_TableBorderStrong });   // Table outer and header borders (prefer using Alpha=1.0 here)
			ModuleConstants.push_back({ "mvThemeCol_TableBorderLight", ImGuiCol_TableBorderLight });     // Table inner borders (prefer using Alpha=1.0 here)
			ModuleConstants.push_back({ "mvThemeCol_TableRowBg", ImGuiCol_TableRowBg });                 // Table row background (even rows)
			ModuleConstants.push_back({ "mvThemeCol_TableRowBgAlt", ImGuiCol_TableRowBgAlt });           // Table row background (odd rows)
			ModuleConstants.push_back({ "mvThemeCol_TextSelectedBg", ImGuiCol_TextSelectedBg });
			ModuleConstants.push_back({ "mvThemeCol_DragDropTarget", ImGuiCol_DragDropTarget });
			ModuleConstants.push_back({ "mvThemeCol_NavHighlight", ImGuiCol_NavHighlight });                   // Gamepad/keyboard: current highlighted item
			ModuleConstants.push_back({ "mvThemeCol_NavWindowingHighlight", ImGuiCol_NavWindowingHighlight }); // Highlight window when using CTRL+TAB
			ModuleConstants.push_back({ "mvThemeCol_NavWindowingDimBg", ImGuiCol_NavWindowingDimBg });         // Darken/colorize entire screen behind the CTRL+TAB window list", when active
			ModuleConstants.push_back({ "mvThemeCol_ModalWindowDimBg", ImGuiCol_ModalWindowDimBg });           // Darken/colorize entire screen behind a modal window", when one is active

			// plotting

			// item styling colors
			ModuleConstants.push_back({ "mvPlotCol_Line", ImPlotCol_Line });                   // plot line/outline color (defaults to next unused color in current colormap)
			ModuleConstants.push_back({ "mvPlotCol_Fill", ImPlotCol_Fill });                   // plot fill color for bars (defaults to the current line color)
			ModuleConstants.push_back({ "mvPlotCol_MarkerOutline", ImPlotCol_MarkerOutline }); // marker outline color (defaults to the current line color)
			ModuleConstants.push_back({ "mvPlotCol_MarkerFill", ImPlotCol_MarkerFill });       // marker fill color (defaults to the current line color)
			ModuleConstants.push_back({ "mvPlotCol_ErrorBar", ImPlotCol_ErrorBar });           // error bar color (defaults to ImGuiCol_Text)

			// plot styling colors
			ModuleConstants.push_back({ "mvPlotCol_FrameBg", ImPlotCol_FrameBg });           // plot frame background color (defaults to ImGuiCol_FrameBg)
			ModuleConstants.push_back({ "mvPlotCol_PlotBg", ImPlotCol_PlotBg });             // plot area background color (defaults to ImGuiCol_WindowBg)
			ModuleConstants.push_back({ "mvPlotCol_PlotBorder", ImPlotCol_PlotBorder });     // plot area border color (defaults to ImGuiCol_Border)
			ModuleConstants.push_back({ "mvPlotCol_LegendBg", ImPlotCol_LegendBg });         // legend background color (defaults to ImGuiCol_PopupBg)
			ModuleConstants.push_back({ "mvPlotCol_LegendBorder", ImPlotCol_LegendBorder }); // legend border color (defaults to ImPlotCol_PlotBorder)
			ModuleConstants.push_back({ "mvPlotCol_LegendText", ImPlotCol_LegendText });     // legend text color (defaults to ImPlotCol_InlayText)
			ModuleConstants.push_back({ "mvPlotCol_TitleText", ImPlotCol_TitleText });       // plot title text color (defaults to ImGuiCol_Text)
			ModuleConstants.push_back({ "mvPlotCol_InlayText", ImPlotCol_InlayText });       // color of text appearing inside of plots (defaults to ImGuiCol_Text)
			ModuleConstants.push_back({ "mvPlotCol_XAxis", ImPlotCol_XAxis });               // x-axis label and tick lables color (defaults to ImGuiCol_Text)
			ModuleConstants.push_back({ "mvPlotCol_XAxisGrid", ImPlotCol_XAxisGrid });       // x-axis grid color (defaults to 25% ImPlotCol_XAxis)
			ModuleConstants.push_back({ "mvPlotCol_YAxis", ImPlotCol_YAxis });               // y-axis label and tick labels color (defaults to ImGuiCol_Text)
			ModuleConstants.push_back({ "mvPlotCol_YAxisGrid", ImPlotCol_YAxisGrid });       // y-axis grid color (defaults to 25% ImPlotCol_YAxis)
			ModuleConstants.push_back({ "mvPlotCol_YAxis2", ImPlotCol_YAxis2 });             // 2nd y-axis label and tick labels color (defaults to ImGuiCol_Text)
			ModuleConstants.push_back({ "mvPlotCol_YAxisGrid2", ImPlotCol_YAxisGrid2 });     // 2nd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis2)
			ModuleConstants.push_back({ "mvPlotCol_YAxis3", ImPlotCol_YAxis3 });             // 3rd y-axis label and tick labels color (defaults to ImGuiCol_Text)
			ModuleConstants.push_back({ "mvPlotCol_YAxisGrid3", ImPlotCol_YAxisGrid3 });     // 3rd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis3)
			ModuleConstants.push_back({ "mvPlotCol_Selection", ImPlotCol_Selection });       // box-selection color (defaults to yellow)
			ModuleConstants.push_back({ "mvPlotCol_Query", ImPlotCol_Query });               // box-query color (defaults to green)
			ModuleConstants.push_back({ "mvPlotCol_Crosshairs", ImPlotCol_Crosshairs });     // crosshairs color (defaults to ImPlotCol_PlotBorder)

			// nodes
			ModuleConstants.push_back({ "mvNodeCol_NodeBackground", imnodes::ColorStyle::ColorStyle_NodeBackground });
			ModuleConstants.push_back({ "mvNodeCol_NodeBackgroundHovered", imnodes::ColorStyle::ColorStyle_NodeBackgroundHovered });
			ModuleConstants.push_back({ "mvNodeCol_NodeBackgroundSelected", imnodes::ColorStyle::ColorStyle_NodeBackgroundSelected });
			ModuleConstants.push_back({ "mvNodeCol_NodeOutline", imnodes::ColorStyle::ColorStyle_NodeOutline });
			ModuleConstants.push_back({ "mvNodeCol_TitleBar", imnodes::ColorStyle::ColorStyle_TitleBar });
			ModuleConstants.push_back({ "mvNodeCol_TitleBarHovered", imnodes::ColorStyle::ColorStyle_TitleBarHovered });
			ModuleConstants.push_back({ "mvNodeCol_TitleBarSelected", imnodes::ColorStyle::ColorStyle_TitleBarSelected });
			ModuleConstants.push_back({ "mvNodeCol_Link", imnodes::ColorStyle::ColorStyle_Link });
			ModuleConstants.push_back({ "mvNodeCol_LinkHovered", imnodes::ColorStyle::ColorStyle_LinkHovered });
			ModuleConstants.push_back({ "mvNodeCol_LinkSelected", imnodes::ColorStyle::ColorStyle_LinkSelected });
			ModuleConstants.push_back({ "mvNodeCol_Pin", imnodes::ColorStyle::ColorStyle_Pin });
			ModuleConstants.push_back({ "mvNodeCol_PinHovered", imnodes::ColorStyle::ColorStyle_PinHovered });
			ModuleConstants.push_back({ "mvNodeCol_BoxSelector", imnodes::ColorStyle::ColorStyle_BoxSelector });
			ModuleConstants.push_back({ "mvNodeCol_BoxSelectorOutline", imnodes::ColorStyle::ColorStyle_BoxSelectorOutline });
			ModuleConstants.push_back({ "mvNodeCol_GridBackground", imnodes::ColorStyle::ColorStyle_GridBackground });
			ModuleConstants.push_back({ "mvNodeCol_GridLine", imnodes::ColorStyle::ColorStyle_GridLine });

			ModuleConstants.push_back({ "mvStyleVar_Alpha", ImGuiStyleVar_Alpha });                             // float     Alpha
			ModuleConstants.push_back({ "mvStyleVar_WindowPadding", ImGuiStyleVar_WindowPadding });             // ImVec2    WindowPadding
			ModuleConstants.push_back({ "mvStyleVar_WindowRounding", ImGuiStyleVar_WindowRounding });           // float     WindowRounding
			ModuleConstants.push_back({ "mvStyleVar_WindowBorderSize", ImGuiStyleVar_WindowBorderSize });       // float     WindowBorderSize
			ModuleConstants.push_back({ "mvStyleVar_WindowMinSize", ImGuiStyleVar_WindowMinSize });             // ImVec2    WindowMinSize
			ModuleConstants.push_back({ "mvStyleVar_WindowTitleAlign", ImGuiStyleVar_WindowTitleAlign });       // ImVec2    WindowTitleAlign
			ModuleConstants.push_back({ "mvStyleVar_ChildRounding", ImGuiStyleVar_ChildRounding });             // float     ChildRounding
			ModuleConstants.push_back({ "mvStyleVar_ChildBorderSize", ImGuiStyleVar_ChildBorderSize });         // float     ChildBorderSize
			ModuleConstants.push_back({ "mvStyleVar_PopupRounding", ImGuiStyleVar_PopupRounding });             // float     PopupRounding
			ModuleConstants.push_back({ "mvStyleVar_PopupBorderSize", ImGuiStyleVar_PopupBorderSize });         // float     PopupBorderSize
			ModuleConstants.push_back({ "mvStyleVar_FramePadding", ImGuiStyleVar_FramePadding });               // ImVec2    FramePadding
			ModuleConstants.push_back({ "mvStyleVar_FrameRounding", ImGuiStyleVar_FrameRounding });             // float     FrameRounding
			ModuleConstants.push_back({ "mvStyleVar_FrameBorderSize", ImGuiStyleVar_FrameBorderSize });         // float     FrameBorderSize
			ModuleConstants.push_back({ "mvStyleVar_ItemSpacing", ImGuiStyleVar_ItemSpacing });                 // ImVec2    ItemSpacing
			ModuleConstants.push_back({ "mvStyleVar_ItemInnerSpacing", ImGuiStyleVar_ItemInnerSpacing });       // ImVec2    ItemInnerSpacing
			ModuleConstants.push_back({ "mvStyleVar_IndentSpacing", ImGuiStyleVar_IndentSpacing });             // float     IndentSpacing
			ModuleConstants.push_back({ "mvStyleVar_CellPadding", ImGuiStyleVar_CellPadding });                 // ImVec2    CellPadding
			ModuleConstants.push_back({ "mvStyleVar_ScrollbarSize", ImGuiStyleVar_ScrollbarSize });             // float     ScrollbarSize
			ModuleConstants.push_back({ "mvStyleVar_ScrollbarRounding", ImGuiStyleVar_ScrollbarRounding });     // float     ScrollbarRounding
			ModuleConstants.push_back({ "mvStyleVar_GrabMinSize", ImGuiStyleVar_GrabMinSize });                 // float     GrabMinSize
			ModuleConstants.push_back({ "mvStyleVar_GrabRounding", ImGuiStyleVar_GrabRounding });               // float     GrabRounding
			ModuleConstants.push_back({ "mvStyleVar_TabRounding", ImGuiStyleVar_TabRounding });                 // float     TabRounding
			ModuleConstants.push_back({ "mvStyleVar_ButtonTextAlign", ImGuiStyleVar_ButtonTextAlign });         // ImVec2    ButtonTextAlign
			ModuleConstants.push_back({ "mvStyleVar_SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign }); // ImVec2    SelectableTextAlign

			// item styling variables
			ModuleConstants.push_back({ "mvPlotStyleVar_LineWeight",         ImPlotStyleVar_LineWeight });         // float,  plot item line weight in pixels
			ModuleConstants.push_back({ "mvPlotStyleVar_Marker",             ImPlotStyleVar_Marker });             // int,    marker specification
			ModuleConstants.push_back({ "mvPlotStyleVar_MarkerSize",         ImPlotStyleVar_MarkerSize });         // float,  marker size in pixels (roughly the marker's "radius")
			ModuleConstants.push_back({ "mvPlotStyleVar_MarkerWeight",       ImPlotStyleVar_MarkerWeight });       // float,  plot outline weight of markers in pixels
			ModuleConstants.push_back({ "mvPlotStyleVar_FillAlpha",          ImPlotStyleVar_FillAlpha });          // float,  alpha modifier applied to all plot item fills
			ModuleConstants.push_back({ "mvPlotStyleVar_ErrorBarSize",       ImPlotStyleVar_ErrorBarSize });       // float,  error bar whisker width in pixels
			ModuleConstants.push_back({ "mvPlotStyleVar_ErrorBarWeight",     ImPlotStyleVar_ErrorBarWeight });     // float,  error bar whisker weight in pixels
			ModuleConstants.push_back({ "mvPlotStyleVar_DigitalBitHeight",   ImPlotStyleVar_DigitalBitHeight });   // float,  digital channels bit height (at 1) in pixels
			ModuleConstants.push_back({ "mvPlotStyleVar_DigitalBitGap",      ImPlotStyleVar_DigitalBitGap });      // float,  digital channels bit padding gap in pixels

			// plot styling variables
			ModuleConstants.push_back({ "mvPlotStyleVar_PlotBorderSize", ImPlotStyleVar_PlotBorderSize });         // float,  thickness of border around plot area
			ModuleConstants.push_back({ "mvPlotStyleVar_MinorAlpha", ImPlotStyleVar_MinorAlpha });                 // float,  alpha multiplier applied to minor axis grid lines
			ModuleConstants.push_back({ "mvPlotStyleVar_MajorTickLen", ImPlotStyleVar_MajorTickLen });             // ImVec2, major tick lengths for X and Y axes
			ModuleConstants.push_back({ "mvPlotStyleVar_MinorTickLen", ImPlotStyleVar_MinorTickLen });             // ImVec2, minor tick lengths for X and Y axes
			ModuleConstants.push_back({ "mvPlotStyleVar_MajorTickSize", ImPlotStyleVar_MajorTickSize });           // ImVec2, line thickness of major ticks
			ModuleConstants.push_back({ "mvPlotStyleVar_MinorTickSize", ImPlotStyleVar_MinorTickSize });           // ImVec2, line thickness of minor ticks
			ModuleConstants.push_back({ "mvPlotStyleVar_MajorGridSize", ImPlotStyleVar_MajorGridSize });           // ImVec2, line thickness of major grid lines
			ModuleConstants.push_back({ "mvPlotStyleVar_MinorGridSize", ImPlotStyleVar_MinorGridSize });           // ImVec2, line thickness of minor grid lines
			ModuleConstants.push_back({ "mvPlotStyleVar_PlotPadding", ImPlotStyleVar_PlotPadding });               // ImVec2, padding between widget frame and plot area, labels, or outside legends (i.e. main padding)
			ModuleConstants.push_back({ "mvPlotStyleVar_LabelPadding", ImPlotStyleVar_LabelPadding });             // ImVec2, padding between axes labels, tick labels, and plot edge
			ModuleConstants.push_back({ "mvPlotStyleVar_LegendPadding", ImPlotStyleVar_LegendPadding });           // ImVec2, legend padding from plot edges
			ModuleConstants.push_back({ "mvPlotStyleVar_LegendInnerPadding", ImPlotStyleVar_LegendInnerPadding }); // ImVec2, legend inner padding from legend edges
			ModuleConstants.push_back({ "mvPlotStyleVar_LegendSpacing", ImPlotStyleVar_LegendSpacing });           // ImVec2, spacing between legend entries
			ModuleConstants.push_back({ "mvPlotStyleVar_MousePosPadding", ImPlotStyleVar_MousePosPadding });       // ImVec2, padding between plot edge and interior info text
			ModuleConstants.push_back({ "mvPlotStyleVar_AnnotationPadding", ImPlotStyleVar_AnnotationPadding });   // ImVec2, text padding around annotation labels
			ModuleConstants.push_back({ "mvPlotStyleVar_FitPadding", ImPlotStyleVar_FitPadding });                 // ImVec2, additional fit padding as a percentage of the fit extents (e.g. ImVec2(0.1f,0.1f) adds 10% to the fit extents of X and Y)
			ModuleConstants.push_back({ "mvPlotStyleVar_PlotDefaultSize", ImPlotStyleVar_PlotDefaultSize });       // ImVec2, default size used when ImVec2(0,0) is passed to BeginPlot
			ModuleConstants.push_back({ "mvPlotStyleVar_PlotMinSize", ImPlotStyleVar_PlotMinSize });               // ImVec2, minimum size plot frame can be when shrunk

			// nodes
			ModuleConstants.push_back({ "mvNodeStyleVar_GridSpacing", imnodes::StyleVar::StyleVar_GridSpacing });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodeCornerRounding", imnodes::StyleVar::StyleVar_NodeCornerRounding });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodePaddingHorizontal", imnodes::StyleVar::StyleVar_NodePaddingHorizontal });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodePaddingVertical", imnodes::StyleVar::StyleVar_NodePaddingVertical });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodeBorderThickness", imnodes::StyleVar::StyleVar_NodeBorderThickness });
			ModuleConstants.push_back({ "mvNodeStyleVar_LinkThickness", imnodes::StyleVar::StyleVar_LinkThickness });
			ModuleConstants.push_back({ "mvNodeStyleVar_LinkLineSegmentsPerLength", imnodes::StyleVar::StyleVar_LinkLineSegmentsPerLength });
			ModuleConstants.push_back({ "mvNodeStyleVar_LinkHoverDistance", imnodes::StyleVar::StyleVar_LinkHoverDistance });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinCircleRadius", imnodes::StyleVar::StyleVar_PinCircleRadius });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinQuadSideLength", imnodes::StyleVar::StyleVar_PinQuadSideLength });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinTriangleSideLength", imnodes::StyleVar::StyleVar_PinTriangleSideLength });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinLineThickness", imnodes::StyleVar::StyleVar_PinLineThickness });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinHoverRadius", imnodes::StyleVar::StyleVar_PinHoverRadius });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinOffset", imnodes::StyleVar::StyleVar_PinOffset });

			#define X(el) ModuleConstants.push_back({ std::string(#el), (long)mvAppItemType::el });
			MV_ITEM_TYPES
			#undef X

			int j = 0;
			for (int i = MV_RESERVED_UUID_start; i < MV_RESERVED_UUID_start+MV_RESERVED_UUIDs; i++)
			{
				ModuleConstants.push_back({"mvReservedUUID_" + std::to_string(j), (long)i});
				j++;
			}

		}

		First_Run = false;
		return ModuleConstants;
	}

	mv_internal PyObject* 
	common_constructor(const char* command, mvAppItemType type, PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID id = 0;
		mvRef<mvAppItem> item = nullptr;
		auto [parent, before, alias] = GetNameFromArgs(id, args, kwargs);
		if(id == 0) id = GenerateUUID();
		item = CreateEntity(type, id);
		item->_type = type;
		if (!item->_alias.empty()) RemoveAlias(*GContext->itemRegistry, item->_alias, true);
		item->_alias = alias;
		if (!item->_alias.empty()) AddAlias(*GContext->itemRegistry, item->_alias, item->_uuid);
		TryBoundTemplateRegistry((*GContext->itemRegistry), item.get());
		VerifyArgumentCount(GetParsers()[command], args);
		if(!GContext->IO.skipRequiredArgs)
			item->handleSpecificRequiredArgs(args);
		if(!GContext->IO.skipPositionalArgs)
			item->handleSpecificPositionalArgs(args);
		if(!GContext->IO.skipKeywordArgs)
			item->handleKeywordArgs(kwargs, command);
		AddItemWithRuntimeChecks((*GContext->itemRegistry), item, parent, before);
		if(item->_alias.empty()) return Py_BuildValue("K", id);
		return ToPyString(item->_alias);
	}

	#define X(el) static PyObject* el##_command(PyObject* self, PyObject* args, PyObject* kwargs){return common_constructor(GetEntityCommand(mvAppItemType::el), mvAppItemType::el, self, args, kwargs);}
	MV_ITEM_TYPES
	#undef X

	PyMODINIT_FUNC
	PyInit__dearpygui(void)
	{

		mv_local_persist std::vector<PyMethodDef> methods;
		methods.clear();

		#define X(el) methods.push_back({GetEntityCommand(mvAppItemType::el), (PyCFunction)el##_command, METH_VARARGS | METH_KEYWORDS, GetParsers()[GetEntityCommand(mvAppItemType::el)].documentation.c_str()});
		MV_ITEM_TYPES
		#undef X

		// color maps
		MV_ADD_COMMAND(bind_colormap);
		MV_ADD_COMMAND(sample_colormap);
		MV_ADD_COMMAND(get_colormap_color);

		// file dialog
		MV_ADD_COMMAND(get_file_dialog_info);

		// window commands
		MV_ADD_COMMAND(set_x_scroll);
		MV_ADD_COMMAND(set_y_scroll);
		MV_ADD_COMMAND(get_x_scroll);
		MV_ADD_COMMAND(get_y_scroll);
		MV_ADD_COMMAND(get_x_scroll_max);
		MV_ADD_COMMAND(get_y_scroll_max);

		// draw layer
		MV_ADD_COMMAND(set_clip_space);

		// draw node
		MV_ADD_COMMAND(apply_transform);
		MV_ADD_COMMAND(create_rotation_matrix);
		MV_ADD_COMMAND(create_translation_matrix);
		MV_ADD_COMMAND(create_scale_matrix);
		MV_ADD_COMMAND(create_lookat_matrix);
		MV_ADD_COMMAND(create_perspective_matrix);
		MV_ADD_COMMAND(create_orthographic_matrix);
		MV_ADD_COMMAND(create_fps_matrix);

		// fonts
		MV_ADD_COMMAND(bind_font);
		MV_ADD_COMMAND(get_text_size);
		MV_ADD_COMMAND(get_global_font_scale);
		MV_ADD_COMMAND(set_global_font_scale);

		// node editor
		MV_ADD_COMMAND(get_selected_nodes);
		MV_ADD_COMMAND(get_selected_links);
		MV_ADD_COMMAND(clear_selected_nodes);
		MV_ADD_COMMAND(clear_selected_links);

		// plots
		MV_ADD_COMMAND(is_plot_queried);
		MV_ADD_COMMAND(get_plot_query_area);

		// plot axes
		MV_ADD_COMMAND(reset_axis_ticks);
		MV_ADD_COMMAND(set_axis_ticks);
		MV_ADD_COMMAND(set_axis_limits);
		MV_ADD_COMMAND(set_axis_limits_auto);
		MV_ADD_COMMAND(get_axis_limits);
		MV_ADD_COMMAND(fit_axis_data);

		// tables
		MV_ADD_COMMAND(highlight_table_column);
		MV_ADD_COMMAND(unhighlight_table_column);
		MV_ADD_COMMAND(set_table_row_color);
		MV_ADD_COMMAND(unset_table_row_color);
		MV_ADD_COMMAND(highlight_table_cell);
		MV_ADD_COMMAND(unhighlight_table_cell);
		MV_ADD_COMMAND(highlight_table_row);
		MV_ADD_COMMAND(unhighlight_table_row);
		MV_ADD_COMMAND(is_table_column_highlighted);
		MV_ADD_COMMAND(is_table_row_highlighted);
		MV_ADD_COMMAND(is_table_cell_highlighted);

		// themes
		MV_ADD_COMMAND(bind_theme);

		// tools
		MV_ADD_COMMAND(show_tool);

		// callback registry
		MV_ADD_COMMAND(set_frame_callback);
		MV_ADD_COMMAND(set_exit_callback);
		MV_ADD_COMMAND(set_viewport_resize_callback);

		// viewport
		MV_ADD_COMMAND(show_viewport);
		MV_ADD_COMMAND(create_viewport);
		MV_ADD_COMMAND(configure_viewport);
		MV_ADD_COMMAND(maximize_viewport);
		MV_ADD_COMMAND(minimize_viewport);
		MV_ADD_COMMAND(get_viewport_configuration);
		MV_ADD_COMMAND(is_viewport_ok);
		MV_ADD_COMMAND(toggle_viewport_fullscreen);

		// context
		MV_ADD_COMMAND(create_context);
		MV_ADD_COMMAND(destroy_context);
		MV_ADD_COMMAND(lock_mutex);
		MV_ADD_COMMAND(unlock_mutex);
		MV_ADD_COMMAND(setup_dearpygui);
		MV_ADD_COMMAND(render_dearpygui_frame);
		MV_ADD_COMMAND(get_delta_time);
		MV_ADD_COMMAND(get_total_time);
		MV_ADD_COMMAND(stop_dearpygui);
		MV_ADD_COMMAND(is_dearpygui_running);
		MV_ADD_COMMAND(generate_uuid);
		MV_ADD_COMMAND(save_init_file);
		MV_ADD_COMMAND(load_image);
		MV_ADD_COMMAND(split_frame);
		MV_ADD_COMMAND(get_frame_count);
		MV_ADD_COMMAND(get_frame_rate);
		MV_ADD_COMMAND(get_app_configuration);
		MV_ADD_COMMAND(configure_app);
		MV_ADD_COMMAND(get_drawing_mouse_pos);
		MV_ADD_COMMAND(is_mouse_button_dragging);
		MV_ADD_COMMAND(is_mouse_button_down);
		MV_ADD_COMMAND(is_mouse_button_clicked);
		MV_ADD_COMMAND(is_mouse_button_double_clicked);
		MV_ADD_COMMAND(is_mouse_button_released);
		MV_ADD_COMMAND(get_mouse_drag_delta);
		MV_ADD_COMMAND(get_mouse_pos);
		MV_ADD_COMMAND(get_plot_mouse_pos);
		MV_ADD_COMMAND(is_key_pressed);
		MV_ADD_COMMAND(is_key_released);
		MV_ADD_COMMAND(is_key_down);

		// item registry
		MV_ADD_COMMAND(focus_item);
		MV_ADD_COMMAND(bind_template_registry);
		MV_ADD_COMMAND(get_aliases);
		MV_ADD_COMMAND(add_alias);
		MV_ADD_COMMAND(remove_alias);
		MV_ADD_COMMAND(does_alias_exist);
		MV_ADD_COMMAND(get_alias_id);
		MV_ADD_COMMAND(move_item);
		MV_ADD_COMMAND(delete_item);
		MV_ADD_COMMAND(does_item_exist);
		MV_ADD_COMMAND(move_item_down);
		MV_ADD_COMMAND(move_item_up);
		MV_ADD_COMMAND(get_windows);
		MV_ADD_COMMAND(get_all_items);
		MV_ADD_COMMAND(get_active_window);
		MV_ADD_COMMAND(set_primary_window);
		MV_ADD_COMMAND(push_container_stack);
		MV_ADD_COMMAND(pop_container_stack);
		MV_ADD_COMMAND(top_container_stack);
		MV_ADD_COMMAND(empty_container_stack);
		MV_ADD_COMMAND(last_item);
		MV_ADD_COMMAND(last_container);
		MV_ADD_COMMAND(last_root);
		MV_ADD_COMMAND(unstage);
		MV_ADD_COMMAND(reorder_items);
		MV_ADD_COMMAND(show_imgui_demo);
		MV_ADD_COMMAND(show_implot_demo);
		MV_ADD_COMMAND(show_item_debug);
		MV_ADD_COMMAND(get_item_info);
		MV_ADD_COMMAND(set_item_alias);
		MV_ADD_COMMAND(get_item_alias);
		MV_ADD_COMMAND(get_item_types);
		MV_ADD_COMMAND(get_item_configuration);
		MV_ADD_COMMAND(get_item_state);
		MV_ADD_COMMAND(configure_item);
		MV_ADD_COMMAND(get_value);
		MV_ADD_COMMAND(get_values);
		MV_ADD_COMMAND(set_value);
		MV_ADD_COMMAND(reset_pos);
		MV_ADD_COMMAND(set_item_children);
		MV_ADD_COMMAND(bind_item_handler_registry);
		MV_ADD_COMMAND(bind_item_font);
		MV_ADD_COMMAND(bind_item_theme);
		MV_ADD_COMMAND(capture_next_item);

		methods.push_back({ NULL, NULL, 0, NULL });

		mv_local_persist PyModuleDef dearpyguiModule = {
			PyModuleDef_HEAD_INIT, "_dearpygui", NULL, -1, methods.data(),
			NULL, NULL, NULL, NULL
		};

		PyObject* m;
		PymvBufferType.tp_new = PyType_GenericNew;
		if (PyType_Ready(&PymvBufferType) < 0)
			return NULL;

		PymvVec4Type.tp_new = PyType_GenericNew;
		if (PyType_Ready(&PymvVec4Type) < 0)
			return NULL;

		PymvMat4Type.tp_new = PyType_GenericNew;
		if (PyType_Ready(&PymvMat4Type) < 0)
			return NULL;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		const auto& constants = GetModuleConstants();

		// handled in the stub file
		for (auto& item : constants)
			PyModule_AddIntConstant(m, item.first.c_str(), item.second);

		auto MarvelError = PyErr_NewException("dearpygui.error", NULL, NULL);
		Py_XINCREF(MarvelError);
		if (PyModule_AddObject(m, "error", MarvelError) < 0) {
			Py_XDECREF(MarvelError);
			Py_CLEAR(MarvelError);
			Py_DECREF(m);
			return NULL;
		}

		Py_INCREF(&PymvBufferType);
		PyModule_AddObject(m, "mvBuffer", (PyObject*)&PymvBufferType);

		Py_INCREF(&PymvVec4Type);
		PyModule_AddObject(m, "mvVec4", (PyObject*)&PymvVec4Type);

		Py_INCREF(&PymvMat4Type);
		PyModule_AddObject(m, "mvMat4", (PyObject*)&PymvMat4Type);

		return m;
	}
}