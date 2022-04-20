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
#include "stb_image.h"
#include "stb_image_write.h"
#include "mvLog.h"
#include "mvProfiler.h"
#include "mvUtilities.h"

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

		if (asource->type == mvAppItemType::mvColorMap)
		{
			mvColorMap* colormap = static_cast<mvColorMap*>(asource);
			source = colormap->_colorMap;
		}
	}

	if (aitem->type == mvAppItemType::mvPlot)
	{
		mvPlot* graph = static_cast<mvPlot*>(aitem);
		graph->SetColorMap((ImPlotColormap)source);
	}

	else if (aitem->type == mvAppItemType::mvColorMapScale)
	{
		mvColorMapScale* graph = static_cast<mvColorMapScale*>(aitem);
		graph->setColorMap((ImPlotColormap)source);
	}

	else if (aitem->type == mvAppItemType::mvColorMapButton)
	{
		mvColorMapButton* graph = static_cast<mvColorMapButton*>(aitem);
		graph->setColorMap((ImPlotColormap)source);
	}

	else if (aitem->type == mvAppItemType::mvColorMapSlider)
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

		if (asource->type == mvAppItemType::mvColorMap)
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

		if (asource->type == mvAppItemType::mvColorMap)
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

	if (aplot->type != mvAppItemType::mvFileDialog)
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

	if (window->type == mvAppItemType::mvWindowAppItem)
	{

		auto pWindow = static_cast<mvWindowAppItem*>(window);

		pWindow->_scrollX = value;
		pWindow->_scrollXSet = true;
	}
	else if (window->type == mvAppItemType::mvChildWindow)
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

	if (window->type == mvAppItemType::mvWindowAppItem)
	{

		auto pWindow = static_cast<mvWindowAppItem*>(window);

		pWindow->_scrollY = value;
		pWindow->_scrollYSet = true;
	}
	else if (window->type == mvAppItemType::mvChildWindow)
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

	if (window->type == mvAppItemType::mvWindowAppItem)
	{

		auto pWindow = static_cast<mvWindowAppItem*>(window);

		return ToPyFloat(pWindow->_scrollX);
	}
	else if (window->type == mvAppItemType::mvChildWindow)
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

	if (window->type == mvAppItemType::mvWindowAppItem)
	{

		auto pWindow = static_cast<mvWindowAppItem*>(window);

		return ToPyFloat(pWindow->_scrollY);
	}
	else if (window->type == mvAppItemType::mvChildWindow)
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

	if (window->type == mvAppItemType::mvWindowAppItem)
	{

		auto pWindow = static_cast<mvWindowAppItem*>(window);

		return ToPyFloat(pWindow->_scrollMaxX);
	}
	else if (window->type == mvAppItemType::mvChildWindow)
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

	if (window->type == mvAppItemType::mvWindowAppItem)
	{

		auto pWindow = static_cast<mvWindowAppItem*>(window);

		return ToPyFloat(pWindow->_scrollMaxY);
	}
	else if (window->type == mvAppItemType::mvChildWindow)
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

	if (aitem->type == mvAppItemType::mvDrawLayer)
	{
		mvDrawLayer* graph = static_cast<mvDrawLayer*>(aitem);
		graph->drawInfo->clipViewport[0] = topleftx;
		graph->drawInfo->clipViewport[1] = toplefty + height;
		graph->drawInfo->clipViewport[2] = width;
		graph->drawInfo->clipViewport[3] = height;
		graph->drawInfo->clipViewport[4] = mindepth;
		graph->drawInfo->clipViewport[5] = maxdepth;

		graph->drawInfo->transform = mvCreateMatrix(
			width, 0.0f, 0.0f, topleftx + (width / 2.0f),
			0.0f, -height, 0.0f, toplefty + (height / 2.0f),
			0.0f, 0.0f, 0.25f, 0.5f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	else
	{
		mvThrowPythonError(mvErrorCode::mvIncompatibleType, "applydrawInfo->transform",
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

	if (aitem->type == mvAppItemType::mvDrawNode)
	{
		mvDrawNode* graph = static_cast<mvDrawNode*>(aitem);
		graph->drawInfo->appliedTransform = atransform->m;
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

	if (aplot->type != mvAppItemType::mvFont)
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

	if (afont->type != mvAppItemType::mvFont)
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

	if (anode_editor->type != mvAppItemType::mvNodeEditor)
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

	if (anode_editor->type != mvAppItemType::mvNodeEditor)
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

	if (anode_editor->type != mvAppItemType::mvNodeEditor)
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

	if (anode_editor->type != mvAppItemType::mvNodeEditor)
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

	if (aplot->type != mvAppItemType::mvPlot)
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

	if (aplot->type != mvAppItemType::mvPlot)
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

	if (aplot->type != mvAppItemType::mvPlotAxis)
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
	graph->resetYTicks();
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

	if (aplot->type != mvAppItemType::mvPlotAxis)
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

	if (aplot->type != mvAppItemType::mvPlotAxis)
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

	if (aplot->type != mvAppItemType::mvPlotAxis)
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

	if (aplot->type != mvAppItemType::mvPlotAxis)
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

	if (aplot->type != mvAppItemType::mvPlotAxis)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (atable->type != mvAppItemType::mvTable)
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

	if (aplot->type != mvAppItemType::mvTheme)
	{
		mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_theme",
			"Incompatible type. Expected types include: mvTheme", aplot);
		return GetPyNone();
	}

	mvTheme* graph = static_cast<mvTheme*>(aplot);

	ResetTheme(*GContext->itemRegistry);

	graph->config.show = true;

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
	PyObject* user_data=nullptr;

	if (!Parse((GetParsers())["set_frame_callback"], args, kwargs, __FUNCTION__,
		&frame, &callback, &user_data))
		return GetPyNone();

	if (frame > GContext->callbackRegistry->highestFrame)
		GContext->callbackRegistry->highestFrame = frame;

	// TODO: check previous entry and deprecate if existing
	Py_XINCREF(callback);

	if(user_data)
		Py_XINCREF(user_data);
	mvSubmitCallback([=]()
		{
			GContext->callbackRegistry->frameCallbacks[frame] = callback;
			GContext->callbackRegistry->frameCallbacksUserData[frame] = user_data;
		});

	return GetPyNone();
}

mv_internal mv_python_function
set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs)
{
	PyObject* callback;
	PyObject* user_data = nullptr;

	if (!Parse((GetParsers())["set_exit_callback"], args, kwargs, __FUNCTION__, &callback,
		&user_data))
		return GetPyNone();

	Py_XINCREF(callback);
	if(user_data)
		Py_XINCREF(user_data);
	mvSubmitCallback([=]()
		{
			GContext->callbackRegistry->onCloseCallback = SanitizeCallback(callback);
			GContext->callbackRegistry->onCloseCallbackUserData = user_data;
		});
	return GetPyNone();
}

mv_internal mv_python_function
set_viewport_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
{
	PyObject* callback = nullptr;
	PyObject* user_data = nullptr;

	if (!Parse((GetParsers())["set_viewport_resize_callback"], args, kwargs, __FUNCTION__,
		&callback, &user_data))
		return GetPyNone();

	if (callback)
		Py_XINCREF(callback);

	if (user_data)
		Py_XINCREF(user_data);

	mvSubmitCallback([=]()
		{
			GContext->callbackRegistry->resizeCallback = SanitizeCallback(callback);
			GContext->callbackRegistry->resizeCallbackUserData = user_data;
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
		mvShowViewport(*viewport, minimized, maximized);
		mvGraphicsSpec spec{}; // not used yet
		GContext->graphics = setup_graphics(*viewport, spec);
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
			mvMaximizeViewport(*GContext->viewport);
		});

	return GetPyNone();
}

mv_internal mv_python_function
minimize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
{
	if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
	mvSubmitTask([=]()
		{
			mvMinimizeViewport(*GContext->viewport);
		});

	return GetPyNone();
}

mv_internal mv_python_function
toggle_viewport_fullscreen(PyObject* self, PyObject* args, PyObject* kwargs)
{
	if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
	mvSubmitTask([=]()
		{
			mvToggleFullScreen(*GContext->viewport);
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
save_image(PyObject* self, PyObject* args, PyObject* kwargs)
{
	const char* file;
	i32 width;
	i32 height;
	PyObject* data;
	//i32 stride_in_bytes = 1;
	i32 components = 4;
	i32 quality = 50;

	if (!Parse((GetParsers())["save_image"], args, kwargs, __FUNCTION__,
		&file, &width, &height, &data, &components, &quality))
		return GetPyNone();

	enum ImageType_
	{
		MV_IMAGE_TYPE_INVALID_,
		MV_IMAGE_TYPE_PNG_,
		MV_IMAGE_TYPE_BMP_,
		MV_IMAGE_TYPE_TGA_,
		MV_IMAGE_TYPE_HDR_,
		MV_IMAGE_TYPE_JPG_
	};

	ImageType_ imageType = MV_IMAGE_TYPE_INVALID_;
	size_t filepathLength = strlen(file);

	// most include atleast 4 chars ".png"
	assert(filepathLength > 4 && "Invalid file for image");
	assert(components < 5 && components > 0);
	assert(quality > 0 && quality < 101);

	// sanity checks
	if (filepathLength < 5)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "File path for 'save_image(...)' must be of the form 'name.png'.");
		return GetPyNone();
	}

	if (components > 4 || components < 1)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "Component count for 'save_image(...)' must be between 1 and 4.");
		return GetPyNone();
	}

	if (quality < 1 || quality > 100)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "Quality must be between 1 and 100.");
		return GetPyNone();
	}

	// TODO: support other formats and make this better
	if (file[filepathLength - 3] == 'p' && file[filepathLength - 2] == 'n' && file[filepathLength - 1] == 'g')
	{
		imageType = MV_IMAGE_TYPE_PNG_;
	}
	else if (file[filepathLength - 3] == 'b' && file[filepathLength - 2] == 'm' && file[filepathLength - 1] == 'p')
	{
		imageType = MV_IMAGE_TYPE_BMP_;
	}
	else if (file[filepathLength - 3] == 't' && file[filepathLength - 2] == 'g' && file[filepathLength - 1] == 'a')
	{
		imageType = MV_IMAGE_TYPE_TGA_;
	}
	else if (file[filepathLength - 3] == 'h' && file[filepathLength - 2] == 'd' && file[filepathLength - 1] == 'r')
	{
		imageType = MV_IMAGE_TYPE_HDR_;
	}
	else if (file[filepathLength - 3] == 'j' && file[filepathLength - 2] == 'p' && file[filepathLength - 1] == 'g')
	{
		imageType = MV_IMAGE_TYPE_JPG_;
	}
	else
	{
		mvThrowPythonError(mvErrorCode::mvNone, "File path for 'save_image(...)' must be of the form 'name.png'.");
		return GetPyNone();
	}

	switch (imageType)
	{
	case MV_IMAGE_TYPE_PNG_:
	{
		std::vector<unsigned char> convertedData = ToUCharVect(data);
		int result = stbi_write_png(file, width, height, components, convertedData.data(), sizeof(unsigned char)*components*width);
		break;
	}
	case MV_IMAGE_TYPE_BMP_:
	{
		std::vector<unsigned char> convertedData = ToUCharVect(data);
		int result = stbi_write_bmp(file, width, height, components, convertedData.data());
		break;
	}
	case MV_IMAGE_TYPE_TGA_:
	{
		std::vector<unsigned char> convertedData = ToUCharVect(data);
		int result = stbi_write_tga(file, width, height, components, convertedData.data());
		break;
	}
	case MV_IMAGE_TYPE_HDR_:
	{
		std::vector<float> convertedData = ToFloatVect(data);
		int result = stbi_write_hdr(file, width, height, components, convertedData.data());
		break;
	}
	case MV_IMAGE_TYPE_JPG_:
	{
		std::vector<unsigned char> convertedData = ToUCharVect(data);
		int result = stbi_write_jpg(file, width, height, components, convertedData.data(), quality);
		break;
	}
	}

	return GetPyNone();
}

mv_internal mv_python_function
output_frame_buffer(PyObject* self, PyObject* args, PyObject* kwargs)
{
	const char* file = "";
	PyObject* callback = nullptr;

	if (!Parse((GetParsers())["output_frame_buffer"], args, kwargs, __FUNCTION__,
		&file, &callback))
		return GetPyNone();


	size_t filepathLength = strlen(file);

	if (filepathLength == 0 && callback) // not specified, return array instead
	{
		//Py_XINCREF(callback);
		PyObject* newbuffer = nullptr;
		PymvBuffer* newbufferview = PyObject_New(PymvBuffer, &PymvBufferType);
		newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvBufferType);
		mvSubmitTask([newbuffer, callback, newbufferview]() {
			OutputFrameBufferArray(newbufferview);
			mvAddCallback(callback, 0, newbuffer, nullptr, false);
			});

		return GetPyNone();
	}

	// most include atleast 4 chars ".png"
	assert(filepathLength > 4 && "Invalid file for image");

	// sanity checks
	if (filepathLength < 5)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "File path for 'output_frame_buffer(...)' must be of the form 'name.png'.");
		return GetPyNone();
	}

	// TODO: support other formats
	if (file[filepathLength - 3] == 'p' && file[filepathLength - 2] == 'n' && file[filepathLength - 1] == 'g')
	{
        std::string fileStored = file;
        mvSubmitTask([fileStored](){
            OutputFrameBuffer(fileStored.c_str());
        });

	}
	else
	{
		mvThrowPythonError(mvErrorCode::mvNone, "File path for 'output_frame_buffer(...)' must be of the form 'name.png'.");
		return GetPyNone();
	}

	return GetPyNone();
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

	if (GContext->viewport->resized)
	{
		mvOnResize();
		GContext->viewport->resized = false;
	}

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
			mvRunCallback(GContext->callbackRegistry->onCloseCallback, 0, nullptr, GContext->callbackRegistry->onCloseCallbackUserData);
			GContext->started = false;  // return to false after
			});

		imnodes::DestroyContext();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		mvToolManager::Reset();
		ClearItemRegistry(*GContext->itemRegistry);



		//#define X(el) el::s_class_theme_component = nullptr; el::s_class_theme_disabled_component = nullptr;
		#define X(el) DearPyGui::GetClassThemeComponent(mvAppItemType::el) = nullptr; DearPyGui::GetDisabledClassThemeComponent(mvAppItemType::el) = nullptr;
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
	if (PyObject* item = PyDict_GetItemString(kwargs, "manual_callback_management")) GContext->IO.manualCallbacks = ToBool(item);

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
	PyDict_SetItemString(pdict, "manual_callback_management", mvPyObject(ToPyBool(GContext->IO.manualCallbacks)));
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
		return ToPyUUID(item->uuid);
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
		return ToPyUUID(item->uuid);
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
		if (DearPyGui::GetEntityDesciptionFlags(parent->type) & MV_ITEM_DESC_CONTAINER)
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
		if (window->getWindowAsMainStatus() == (bool)value)
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
		if (window->uuid != item)
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

	std::vector<mvRef<mvAppItem>>& children = parent->childslots[slot];

	std::vector<mvRef<mvAppItem>> newchildren;
	newchildren.reserve(children.size());

	// todo: better sorting algorithm
	for (const auto& item : anew_order)
	{
		for (auto& child : children)
		{
			if (child->uuid == item)
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
		if (aitem->uuid == item && aitem->type == mvAppItemType::mvStage)
		{
			for (auto& children : aitem->childslots)
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
		actualItem->info.showDebug = true;
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
		auto& children0 = item->childslots[0];
		auto& children1 = item->childslots[1];
		auto& children2 = item->childslots[2];
		for (auto& child : children0)
		{
			childList.emplace_back(child->uuid);
			if (DearPyGui::GetEntityDesciptionFlags(child->type) & MV_ITEM_DESC_CONTAINER)
				ChildRetriever(child);
		}
		for (auto& child : children1)
		{
			childList.emplace_back(child->uuid);
			if (DearPyGui::GetEntityDesciptionFlags(child->type) & MV_ITEM_DESC_CONTAINER)
				ChildRetriever(child);
		}
		for (auto& child : children2)
		{
			childList.emplace_back(child->uuid);
			if (DearPyGui::GetEntityDesciptionFlags(child->type) & MV_ITEM_DESC_CONTAINER)
				ChildRetriever(child);
		}

	};

	for (auto& root : roots)
	{
		childList.emplace_back(root->uuid);
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
	for (auto& root : GContext->itemRegistry->colormapRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->filedialogRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->stagingRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->viewportMenubarRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->windowRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->fontRegistryRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->handlerRegistryRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->textureRegistryRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->valueRegistryRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->themeRegistryRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->itemTemplatesRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->itemHandlerRegistryRoots) childList.emplace_back(root->uuid);
	for (auto& root : GContext->itemRegistry->viewportDrawlistRoots) childList.emplace_back(root->uuid);

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
			if (GContext->itemRegistry->windowRoots[i]->uuid == item)
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
		appitem->info.focusNextFrame = true;
		if (auto parent = GetItemRoot(*GContext->itemRegistry, item))
			parent->info.focusNextFrame = true;
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
		for (auto& children : item->childslots)
		{
			std::vector<mvUUID> childSlot;
			for (auto& child : children)
			{
				if (child)
					childSlot.emplace_back(child->uuid);
			}
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

		std::string parserCommand = GetEntityCommand(appitem->type);

		auto children = GetItemChildren(*GContext->itemRegistry, appitem->uuid);
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

		PyDict_SetItemString(pdict, "type", mvPyObject(ToPyString(DearPyGui::GetEntityTypeString(appitem->type))));
		PyDict_SetItemString(pdict, "target", mvPyObject(ToPyInt(DearPyGui::GetEntityTargetSlot(appitem->type))));

		if (appitem->info.parentPtr)
			PyDict_SetItemString(pdict, "parent", mvPyObject(ToPyUUID(appitem->info.parentPtr->uuid)));
		else
			PyDict_SetItemString(pdict, "parent", mvPyObject(GetPyNone()));

		if (appitem->theme)
			PyDict_SetItemString(pdict, "theme", mvPyObject(ToPyUUID(appitem->theme->uuid)));
		else
			PyDict_SetItemString(pdict, "theme", mvPyObject(GetPyNone()));

		if (appitem->font)
			PyDict_SetItemString(pdict, "font", mvPyObject(ToPyUUID(appitem->font->uuid)));
		else
			PyDict_SetItemString(pdict, "font", mvPyObject(GetPyNone()));

		if (DearPyGui::GetEntityDesciptionFlags(appitem->type) & MV_ITEM_DESC_CONTAINER)
			PyDict_SetItemString(pdict, "container", mvPyObject(ToPyBool(true)));
		else
			PyDict_SetItemString(pdict, "container", mvPyObject(ToPyBool(false)));

		i32 applicableState = DearPyGui::GetApplicableState(appitem->type);
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
		mvPyObject py_filter_key = ToPyString(appitem->config.filter);
		mvPyObject py_payload_type = ToPyString(appitem->config.payloadType);
		mvPyObject py_label = ToPyString(appitem->config.specifiedLabel);
		mvPyObject py_use_internal_label = ToPyBool(appitem->config.useInternalLabel);
		mvPyObject py_source = ToPyUUID(appitem->config.source);
		mvPyObject py_show = ToPyBool(appitem->config.show);
		mvPyObject py_enabled = ToPyBool(appitem->config.enabled);
		mvPyObject py_tracked = ToPyBool(appitem->config.tracked);
		mvPyObject py_width = ToPyInt(appitem->config.width);
		mvPyObject py_track_offset = ToPyFloat(appitem->config.trackOffset);
		mvPyObject py_height = ToPyInt(appitem->config.height);
		mvPyObject py_indent = ToPyInt((i32)appitem->config.indent);

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

		if (appitem->config.callback)
		{
			Py_XINCREF(appitem->config.callback);
			PyDict_SetItemString(pdict, "callback", appitem->config.callback);
		}
		else
			PyDict_SetItemString(pdict, "callback", GetPyNone());

		if (appitem->config.dropCallback)
		{
			Py_XINCREF(appitem->config.dropCallback);
			PyDict_SetItemString(pdict, "drop_callback", appitem->config.dropCallback);
		}
		else
			PyDict_SetItemString(pdict, "drop_callback", GetPyNone());

		if (appitem->config.dragCallback)
		{
			Py_XINCREF(appitem->config.dragCallback);
			PyDict_SetItemString(pdict, "drag_callback", appitem->config.dragCallback);
		}
		else
			PyDict_SetItemString(pdict, "drag_callback", GetPyNone());

		if (appitem->config.user_data)
		{
			Py_XINCREF(appitem->config.user_data);
			PyDict_SetItemString(pdict, "user_data", appitem->config.user_data);
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
		if (stage->uuid == source)
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
		auto& oldChildren = appitem->childslots[slot];
		oldChildren.reserve(staging_container->childslots[slot].size());
		oldChildren = std::move(staging_container->childslots[slot]);
		for (auto& child : oldChildren)
		{
			child->config.parent = item;
			child->info.parentPtr = appitem;
		}

		// update locations
		for (i32 i = 0; i < 4; i++)
		{
			i32 index = 0;
			for (auto& child : appitem->childslots[i])
			{
				child->info.location = index;
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
			appitem->font = nullptr;
			return GetPyNone();
		}

		if (appfont)
		{
			appitem->font = appfont;
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
			appitem->theme = nullptr;
			return GetPyNone();
		}

		auto apptheme = GetRefItem(*GContext->itemRegistry, theme);

		if (apptheme)
		{
			if (apptheme->type != mvAppItemType::mvTheme)
			{
				mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_item_theme",
					"Item not a theme: " + std::to_string(theme), nullptr);
			}
			appitem->theme = *(mvRef<mvTheme>*)(&apptheme);
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
			appitem->handlerRegistry = nullptr;
			return GetPyNone();
		}

		auto apptheme = GetRefItem(*GContext->itemRegistry, reg);

		if (apptheme)
		{
			if (apptheme->type != mvAppItemType::mvItemHandlerRegistry)
			{
				mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_item_handler_registry",
					"Item not handler registry: " + std::to_string(reg), nullptr);
			}
			appitem->handlerRegistry = *(mvRef<mvItemHandlerRegistry>*)(&apptheme);
			appitem->handlerRegistry->onBind(appitem);
			return GetPyNone();
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_handler_registry",
				"Item Handler Registry not found: " + std::to_string(reg), nullptr);
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
		appitem->info.dirtyPos = false;
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
		FillAppItemState(pdict, appitem->state, DearPyGui::GetApplicableState(appitem->type));
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
		appitem->handleKeywordArgs(kwargs, GetEntityCommand(appitem->type));
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
		appitem->config.alias = alias;
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
		return ToPyString(appitem->config.alias);
	return GetPyNone();
}

mv_internal mv_python_function
capture_next_item(PyObject* self, PyObject* args, PyObject* kwargs)
{
	PyObject* callable;
	PyObject* user_data;

	if (!Parse((GetParsers())["capture_next_item"], args, kwargs, __FUNCTION__,
		&callable, &user_data))
		return GetPyNone();

	if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

	if (GContext->itemRegistry->captureCallback)
		Py_XDECREF(GContext->itemRegistry->captureCallback);

	if (GContext->itemRegistry->captureCallbackUserData)
		Py_XDECREF(GContext->itemRegistry->captureCallbackUserData);

	Py_XINCREF(callable);
	if(user_data)
		Py_XINCREF(user_data);
	if (callable == Py_None)
		GContext->itemRegistry->captureCallback = nullptr;
	else
		GContext->itemRegistry->captureCallback = callable;
			
	GContext->itemRegistry->captureCallbackUserData = user_data;

	return GetPyNone();
}

mv_internal mv_python_function
get_callback_queue(PyObject* self, PyObject* args, PyObject* kwargs)
{
	if (GContext->callbackRegistry->jobs.empty())
		return GetPyNone();

	PyObject* pArgs = PyTuple_New(GContext->callbackRegistry->jobs.size());
	for (int i = 0; i < GContext->callbackRegistry->jobs.size(); i++)
	{
		PyObject* job = PyTuple_New(4);
		if (GContext->callbackRegistry->jobs[i].callback)
			PyTuple_SetItem(job, 0, GContext->callbackRegistry->jobs[i].callback);
		else
			PyTuple_SetItem(job, 0, GetPyNone());

		if(GContext->callbackRegistry->jobs[i].sender == 0)
			PyTuple_SetItem(job, 1, ToPyString(GContext->callbackRegistry->jobs[i].sender_str));
		else
			PyTuple_SetItem(job, 1, ToPyUUID(GContext->callbackRegistry->jobs[i].sender));

		if (GContext->callbackRegistry->jobs[i].app_data)
			PyTuple_SetItem(job, 2, GContext->callbackRegistry->jobs[i].app_data); // steals data, so don't deref
		else
			PyTuple_SetItem(job, 2, GetPyNone());

		if (GContext->callbackRegistry->jobs[i].user_data)
			PyTuple_SetItem(job, 3, GContext->callbackRegistry->jobs[i].user_data); // steals data, so don't deref
		else
			PyTuple_SetItem(job, 3, GetPyNone());

		PyTuple_SetItem(pArgs, i, job);
	}

	GContext->callbackRegistry->jobs.clear();
	return pArgs;
}

mv_internal mv_python_function
set_clipboard_text(PyObject* self, PyObject* args, PyObject* kwargs)
{

	const char* text;

	if (!Parse((GetParsers())["set_clipboard_text"], args, kwargs, __FUNCTION__,
		&text))
		return GetPyNone();

	if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

	ImGui::SetClipboardText(text);

	return GetPyNone();
}

mv_internal mv_python_function
get_clipboard_text(PyObject* self, PyObject* args, PyObject* kwargs)
{

	if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

	const char* text = ImGui::GetClipboardText();

	return ToPyString(text);
}