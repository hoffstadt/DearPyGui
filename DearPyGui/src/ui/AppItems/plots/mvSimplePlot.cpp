#include "mvSimplePlot.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/mvItemHandlers.h"

void mvSimplePlot::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvSimplePlot*>(item);
	if (config.source != 0) _value = titem->_value;
	_overlay = titem->_overlay;
	_min = titem->_min;
	_max = titem->_max;
	_histogram = titem->_histogram;
	_autosize = titem->_autosize;
}

void mvSimplePlot::draw(ImDrawList* drawlist, float x, float y)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!config.show)
		return;

	// focusing
	if (info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (info.dirtyPos)
		ImGui::SetCursorPos(state.pos);

	// update widget's position state
	state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (config.width != 0)
		ImGui::SetNextItemWidth((float)config.width);

	// set indent
	if (config.indent > 0.0f)
		ImGui::Indent(config.indent);

	// push font if a font object is attached
	if (font)
	{
		ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(this);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		ImGui::PushID(this);

		if (_histogram)
			ImGui::PlotHistogram(info.internalLabel.c_str(), _value->data(), (int)_value->size(), 0, _overlay.c_str(),
				_min, _max, ImVec2((float)config.width, (float)config.height));
		else
			ImGui::PlotLines(info.internalLabel.c_str(), _value->data(), (int)_value->size(), 0, _overlay.c_str(),
				_min, _max, ImVec2((float)config.width, (float)config.height));

		ImGui::PopID();
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------
	state.lastFrameUpdate = GContext->frame;
	state.hovered = ImGui::IsItemHovered();
	state.leftclicked = ImGui::IsItemClicked();
	state.rightclicked = ImGui::IsItemClicked(1);
	state.middleclicked = ImGui::IsItemClicked(2);
	state.visible = ImGui::IsItemVisible();
	state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
	state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
	state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
	state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (config.indent > 0.0f)
		ImGui::Unindent(config.indent);

	// pop font off stack
	if (font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(this);

	if (handlerRegistry)
		handlerRegistry->checkEvents(&state);

	// handle drag & drop if used
	apply_drag_drop(this);
}

PyObject* mvSimplePlot::getPyValue()
{
	return ToPyList(*_value);
}

void mvSimplePlot::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
}

void mvSimplePlot::setPyValue(PyObject* value)
{
	*_value = ToFloatVect(value);

	if (!_autosize)
		return;
	if (!_value->empty())
	{
		_max = _value->data()[0];
		_min = _value->data()[0];

		for (auto& item : *_value)
		{
			if (item > _max)
				_max = item;
			if (item < _min)
				_min = item;
		}
	}
}

void mvSimplePlot::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "overlay")) _overlay = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "minscale")) _min = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "maxscale")) _max = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "histogram")) _histogram = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "autosize")) _autosize = ToBool(item);
}

void mvSimplePlot::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "overlay", mvPyObject(ToPyString(_overlay)));
	PyDict_SetItemString(dict, "minscale", mvPyObject(ToPyFloat(_min)));
	PyDict_SetItemString(dict, "maxscale", mvPyObject(ToPyFloat(_max)));
	PyDict_SetItemString(dict, "histogram", mvPyObject(ToPyBool(_histogram)));
	PyDict_SetItemString(dict, "autosize", mvPyObject(ToPyBool(_autosize)));
}