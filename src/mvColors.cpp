#include "mvColors.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvPyUtils.h"
#include "mvFontItems.h"
#include "mvThemes.h"
#include "mvContainers.h"
#include "mvItemHandlers.h"

void
DearPyGui::draw_color_button(ImDrawList* drawlist, mvAppItem& item, mvColorButtonConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!item.config.show)
		return;

	// focusing
	if (item.info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		item.info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (item.config.width != 0)
		ImGui::SetNextItemWidth((float)item.config.width);

	// set indent
	if (item.config.indent > 0.0f)
		ImGui::Indent(item.config.indent);

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		ScopedID id(item.uuid);

		ImVec4 col = { (*config.value)[0], (*config.value)[1], (*config.value)[2], (*config.value)[3] };

		if (ImGui::ColorButton(item.info.internalLabel.c_str(), col, config.flags, ImVec2((float)item.config.width, (float)item.config.height)))
		{
			if(item.config.alias.empty())
				mvAddCallback(item.getCallback(false), item.uuid, nullptr, item.config.user_data);
			else
				mvAddCallback(item.getCallback(false), item.config.alias, nullptr, item.config.user_data);
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(item.state);

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (item.config.indent > 0.0f)
		ImGui::Unindent(item.config.indent);

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// handle drag & drop if used
	apply_drag_drop(&item);
}

void
DearPyGui::draw_color_edit(ImDrawList* drawlist, mvAppItem& item, mvColorEditConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!item.config.show)
		return;

	// focusing
	if (item.info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		item.info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (item.config.width != 0)
		ImGui::SetNextItemWidth((float)item.config.width);

	// set indent
	if (item.config.indent > 0.0f)
		ImGui::Indent(item.config.indent);

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{
		ScopedID id(item.uuid);

		if (!item.config.enabled) std::copy(config.value->data(), config.value->data() + 4, config.disabled_value);

		if (ImGui::ColorEdit4(item.info.internalLabel.c_str(), item.config.enabled ? config.value->data() : &config.disabled_value[0], config.flags))
		{
			mvColor color = mvColor((*config.value)[0], (*config.value)[1], (*config.value)[2], (*config.value)[3]);

			if (item.config.alias.empty())
				mvSubmitCallback([&item, color]() { mvAddCallback(item.getCallback(false), item.uuid, ToPyColor(color), item.config.user_data); });
			else
				mvSubmitCallback([&item, color]() { mvAddCallback(item.getCallback(false), item.config.alias, ToPyColor(color), item.config.user_data); });
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(item.state);

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (item.config.indent > 0.0f)
		ImGui::Unindent(item.config.indent);

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// handle drag & drop if used
	apply_drag_drop(&item);
}

void
DearPyGui::draw_color_map(ImDrawList* drawlist, mvAppItem& item, mvColorMapConfig& config)
{
	ScopedID id(item.uuid);

	ImPlot::ColormapButton(item.info.internalLabel.c_str(), ImVec2(-1.0f, 0.0f), config.colorMap);
}

void
DearPyGui::draw_color_map_button(ImDrawList* drawlist, mvAppItem& item, mvColorMapButtonConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!item.config.show)
		return;

	// focusing
	if (item.info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		item.info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (item.config.width != 0)
		ImGui::SetNextItemWidth((float)item.config.width);

	// set indent
	if (item.config.indent > 0.0f)
		ImGui::Indent(item.config.indent);

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{
		ScopedID id(item.uuid);
		if (ImPlot::ColormapButton(item.info.internalLabel.c_str(), ImVec2((float)item.config.width, (float)item.config.height), config.colorMap))
		{
			if (item.config.alias.empty())
				mvAddCallback(item.getCallback(false), item.uuid, nullptr, item.config.user_data);
			else
				mvAddCallback(item.getCallback(false), item.config.alias, nullptr, item.config.user_data);
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(item.state);

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (item.config.indent > 0.0f)
		ImGui::Unindent(item.config.indent);

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// handle drag & drop if used
	apply_drag_drop(&item);
}

void
DearPyGui::draw_color_map_scale(ImDrawList* drawlist, mvAppItem& item, mvColorMapScaleConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!item.config.show)
		return;

	// focusing
	if (item.info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		item.info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (item.config.width != 0)
		ImGui::SetNextItemWidth((float)item.config.width);

	// set indent
	if (item.config.indent > 0.0f)
		ImGui::Indent(item.config.indent);

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{
		ScopedID id(item.uuid);

		ImPlot::ColormapScale(item.info.internalLabel.c_str(), config.scale_min, config.scale_max, ImVec2((float)item.config.width, (float)item.config.height), config.format.c_str(), config.flags, config.colorMap);
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(item.state);

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (item.config.indent > 0.0f)
		ImGui::Unindent(item.config.indent);

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// handle drag & drop if used
	apply_drag_drop(&item);
}

void
DearPyGui::draw_color_picker(ImDrawList* drawlist, mvAppItem& item, mvColorPickerConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!item.config.show)
		return;

	// focusing
	if (item.info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		item.info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (item.config.width != 0)
		ImGui::SetNextItemWidth((float)item.config.width);

	// set indent
	if (item.config.indent > 0.0f)
		ImGui::Indent(item.config.indent);

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{
		ScopedID id(item.uuid);

		if (!item.config.enabled) std::copy(config.value->data(), config.value->data() + 4, config.disabled_value);

		if (ImGui::ColorPicker4(item.info.internalLabel.c_str(), item.config.enabled ? config.value->data() : &config.disabled_value[0], config.flags))
		{
			mvColor color = mvColor((*config.value)[0], (*config.value)[1], (*config.value)[2], (*config.value)[3]);

			if (item.config.alias.empty())
				mvSubmitCallback([&item, color]() { mvAddCallback(item.getCallback(false), item.uuid, ToPyColor(color), item.config.user_data); });
			else
				mvSubmitCallback([&item, color]() { mvAddCallback(item.getCallback(false), item.config.alias, ToPyColor(color), item.config.user_data); });
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(item.state);

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (item.config.indent > 0.0f)
		ImGui::Unindent(item.config.indent);

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// handle drag & drop if used
	apply_drag_drop(&item);
}

void
DearPyGui::draw_color_map_slider(ImDrawList* drawlist, mvAppItem& item, mvColorMapSliderConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!item.config.show)
		return;

	// focusing
	if (item.info.focusNextFrame)
	{
		ImGui::SetKeyboardFocusHere();
		item.info.focusNextFrame = false;
	}

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// set item width
	if (item.config.width != 0)
		ImGui::SetNextItemWidth((float)item.config.width);

	// set indent
	if (item.config.indent > 0.0f)
		ImGui::Indent(item.config.indent);

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{
		ScopedID id(item.uuid);

		if (ImPlot::ColormapSlider(item.info.internalLabel.c_str(), config.value.get(), &config.color, "", config.colorMap))
		{
			auto value = *config.value;

			if (item.config.alias.empty())
				mvSubmitCallback([&item, value]() { mvAddCallback(item.getCallback(false), item.uuid, ToPyFloat(value), item.config.user_data); });
			else
				mvSubmitCallback([&item, value]() { mvAddCallback(item.getCallback(false), item.config.alias, ToPyFloat(value), item.config.user_data); });
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//-----------------------------------------------------------------------------
	UpdateAppItemState(item.state);

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	if (item.config.indent > 0.0f)
		ImGui::Unindent(item.config.indent);

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// handle drag & drop if used
	apply_drag_drop(&item);
}

void
DearPyGui::draw_color_map_registry(ImDrawList* drawlist, mvAppItem& item)
{

	if (!item.config.show)
		return;

	ImGui::PushID(&item);

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(item.info.internalLabel.c_str(), &item.config.show))
	{
		ImGui::Text("Builtin:");
		for (int i = 0; i < 16; i++)
			ImPlot::ColormapButton(ImPlot::GetColormapName(i), ImVec2(-1.0f, 0.0f), i);

		ImGui::Text("User:");

		for (auto& item : item.childslots[1])
			item->draw(drawlist, 0.0f, 0.0f);

	}

	ImGui::End();

	ImGui::PopID();
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvColorButtonConfig& outConfig)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvColorButton)], inDict))
		return;

	for (int i = 0; i < PyTuple_Size(inDict); i++)
	{
		PyObject* item = PyTuple_GetItem(inDict, i);
		mvColor color;
		switch (i)
		{
		case 0:
			color = ToColor(item);
			std::array<float, 4> temp_array;
			temp_array[0] = color.r;
			temp_array[1] = color.g;
			temp_array[2] = color.b;
			temp_array[3] = color.a;
			if (outConfig.value)
				*outConfig.value = temp_array;
			else
				outConfig.value = std::make_shared<std::array<float, 4>>(temp_array);
			break;

		default:
			break;
		}
	}
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvColorEditConfig& outConfig)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvColorEdit)], inDict))
		return;

	for (int i = 0; i < PyTuple_Size(inDict); i++)
	{
		PyObject* item = PyTuple_GetItem(inDict, i);
		mvColor color;
		switch (i)
		{
		case 0:
			color = ToColor(item);
			std::array<float, 4> temp_array;
			temp_array[0] = color.r;
			temp_array[1] = color.g;
			temp_array[2] = color.b;
			temp_array[3] = color.a;
			if (outConfig.value)
				*outConfig.value = temp_array;
			else
				outConfig.value = std::make_shared<std::array<float, 4>>(temp_array);
			break;

		default:
			break;
		}
	}
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvColorPickerConfig& outConfig)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvColorPicker)], inDict))
		return;

	for (int i = 0; i < PyTuple_Size(inDict); i++)
	{
		PyObject* item = PyTuple_GetItem(inDict, i);
		mvColor color;
		switch (i)
		{
		case 0:
			color = ToColor(item);
			std::array<float, 4> temp_array;
			temp_array[0] = color.r;
			temp_array[1] = color.g;
			temp_array[2] = color.b;
			temp_array[3] = color.a;
			if (outConfig.value)
				*outConfig.value = temp_array;
			else
				outConfig.value = std::make_shared<std::array<float, 4>>(temp_array);
			break;

		default:
			break;
		}
	}

}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvColorMapConfig& outConfig, mvAppItem& item)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvColorMap)], inDict))
		return;

	for (int i = 0; i < PyTuple_Size(inDict); i++)
	{
		PyObject* arg = PyTuple_GetItem(inDict, i);
		std::vector<std::vector<int>> rawColors;
		switch (i)
		{
		case 0:
			rawColors = ToVectVectInt(arg);
			for (const auto& color : rawColors)
			{

				std::vector<float> c;

				for (int j = 0; j < color.size(); j++)
					c.push_back((float)color[j] / 255.0f);

				while (c.size() < 4)
					c.push_back(1.0f);

				outConfig.colors.push_back(ImVec4(c[0], c[1], c[2], c[3]));

			}
			break;
		case 1:
			outConfig.qualitative = ToBool(arg);
			break;
		default:
			break;
		}
	}
	outConfig.colorMap = ImPlot::AddColormap(item.info.internalLabel.c_str(), outConfig.colors.data(), (int)outConfig.colors.size(), outConfig.qualitative);
}

PyObject*
DearPyGui::set_data_source(mvColorButtonConfig& outConfig)
{
	// nasty hack
	int r = (int)(outConfig.value->data()[0] * 255.0f * 255.0f);
	int g = (int)(outConfig.value->data()[1] * 255.0f * 255.0f);
	int b = (int)(outConfig.value->data()[2] * 255.0f * 255.0f);
	int a = (int)(outConfig.value->data()[3] * 255.0f * 255.0f);

	auto color = mvColor(r, g, b, a);
	return ToPyColor(color);
}

PyObject*
DearPyGui::set_data_source(mvColorEditConfig& outConfig)
{
	// nasty hack
	int r = (int)(outConfig.value->data()[0] * 255.0f * 255.0f);
	int g = (int)(outConfig.value->data()[1] * 255.0f * 255.0f);
	int b = (int)(outConfig.value->data()[2] * 255.0f * 255.0f);
	int a = (int)(outConfig.value->data()[3] * 255.0f * 255.0f);

	auto color = mvColor(r, g, b, a);
	return ToPyColor(color);
}

PyObject*
DearPyGui::set_data_source(mvColorPickerConfig& outConfig)
{
	// nasty hack
	int r = (int)(outConfig.value->data()[0] * 255.0f * 255.0f);
	int g = (int)(outConfig.value->data()[1] * 255.0f * 255.0f);
	int b = (int)(outConfig.value->data()[2] * 255.0f * 255.0f);
	int a = (int)(outConfig.value->data()[3] * 255.0f * 255.0f);

	auto color = mvColor(r, g, b, a);
	return ToPyColor(color);
}

PyObject*
DearPyGui::set_data_source(mvColorMapSliderConfig& outConfig)
{
	return ToPyFloat(*outConfig.value);
}

void
DearPyGui::set_py_value(PyObject* inDict, mvColorButtonConfig& outConfig)
{
	mvColor color = ToColor(inDict);
	std::array<float, 4> temp_array;
	temp_array[0] = color.r;
	temp_array[1] = color.g;
	temp_array[2] = color.b;
	temp_array[3] = color.a;
	if (outConfig.value)
		*outConfig.value = temp_array;
	else
		outConfig.value = std::make_shared<std::array<float, 4>>(temp_array);
}

void
DearPyGui::set_py_value(PyObject* inDict, mvColorMapSliderConfig& outConfig)
{
	*outConfig.value = ToFloat(inDict);
}

void
DearPyGui::set_py_value(PyObject* inDict, mvColorEditConfig& outConfig)
{
	mvColor color = ToColor(inDict);
	std::array<float, 4> temp_array;
	temp_array[0] = color.r;
	temp_array[1] = color.g;
	temp_array[2] = color.b;
	temp_array[3] = color.a;
	if (outConfig.value)
		*outConfig.value = temp_array;
	else
		outConfig.value = std::make_shared<std::array<float, 4>>(temp_array);
}

void
DearPyGui::set_py_value(PyObject* inDict, mvColorPickerConfig& outConfig)
{
	mvColor color = ToColor(inDict);
	std::array<float, 4> temp_array;
	temp_array[0] = color.r;
	temp_array[1] = color.g;
	temp_array[2] = color.b;
	temp_array[3] = color.a;
	if (outConfig.value)
		*outConfig.value = temp_array;
	else
		outConfig.value = std::make_shared<std::array<float, 4>>(temp_array);
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvColorButtonConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvColorEditConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvColorPickerConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(srcItem->getValue());

}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvColorMapSliderConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<float>*>(srcItem->getValue());

}

void
DearPyGui::set_configuration(PyObject* inDict, mvColorButtonConfig& outConfig)
{
	if (inDict == nullptr)
		return;


	// helpers for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};
	flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, outConfig.flags);
	flagop("no_border", ImGuiColorEditFlags_NoBorder, outConfig.flags);
	flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvColorEditConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	// helpers for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, outConfig.flags);
	flagop("no_picker", ImGuiColorEditFlags_NoPicker, outConfig.flags);
	flagop("no_options", ImGuiColorEditFlags_NoOptions, outConfig.flags);
	flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, outConfig.flags);
	flagop("no_inputs", ImGuiColorEditFlags_NoInputs, outConfig.flags);
	flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, outConfig.flags);
	flagop("no_label", ImGuiColorEditFlags_NoLabel, outConfig.flags);
	flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, outConfig.flags);
	flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, outConfig.flags);
	flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, outConfig.flags);

	if (PyObject* item = PyDict_GetItemString(inDict, "alpha_preview"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_AlphaPreview;
		outConfig.flags &= ~ImGuiColorEditFlags_AlphaPreviewHalf;

		switch (mode)
		{
		case ImGuiColorEditFlags_AlphaPreview:
			outConfig.flags |= ImGuiColorEditFlags_AlphaPreview;
			break;
		case ImGuiColorEditFlags_AlphaPreviewHalf:
			outConfig.flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
			break;
		default:
			break;
		}
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "display_mode"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_DisplayRGB;
		outConfig.flags &= ~ImGuiColorEditFlags_DisplayHSV;
		outConfig.flags &= ~ImGuiColorEditFlags_DisplayHex;

		switch (mode)
		{
		case ImGuiColorEditFlags_DisplayHex:
			outConfig.flags |= ImGuiColorEditFlags_DisplayHex;
			break;
		case ImGuiColorEditFlags_DisplayHSV:
			outConfig.flags |= ImGuiColorEditFlags_DisplayHSV;
			break;
		default:
			outConfig.flags |= ImGuiColorEditFlags_DisplayRGB;
			break;
		}
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "display_type"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_Uint8;
		outConfig.flags &= ~ImGuiColorEditFlags_Float;

		switch (mode)
		{
		case ImGuiColorEditFlags_Float:
			outConfig.flags |= ImGuiColorEditFlags_Float;
			break;
		default:
			outConfig.flags |= ImGuiColorEditFlags_Uint8;
			break;
		}
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "input_mode"))
	{
		long mode = (long)ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_InputRGB;
		outConfig.flags &= ~ImGuiColorEditFlags_InputHSV;

		switch (mode)
		{
		case ImGuiColorEditFlags_InputHSV:
			outConfig.flags |= ImGuiColorEditFlags_InputHSV;
			break;
		default:
			outConfig.flags |= ImGuiColorEditFlags_InputRGB;
			break;
		}
	}
}

void
DearPyGui::set_configuration(PyObject* inDict, mvColorPickerConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	// helpers for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};


	flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, outConfig.flags);
	flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, outConfig.flags);
	flagop("no_inputs", ImGuiColorEditFlags_NoInputs, outConfig.flags);
	flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, outConfig.flags);
	flagop("no_label", ImGuiColorEditFlags_NoLabel, outConfig.flags);
	flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, outConfig.flags);
	flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, outConfig.flags);
	flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, outConfig.flags);
	flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, outConfig.flags);
	flagop("display_hex", ImGuiColorEditFlags_DisplayHex, outConfig.flags);

	if (PyObject* item = PyDict_GetItemString(inDict, "picker_mode"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_PickerHueBar;
		outConfig.flags &= ~ImGuiColorEditFlags_PickerHueWheel;

		switch (mode)
		{
		case ImGuiColorEditFlags_PickerHueWheel:
			outConfig.flags |= ImGuiColorEditFlags_PickerHueWheel;
			break;
		default:
			outConfig.flags |= ImGuiColorEditFlags_PickerHueBar;
			break;
		}
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "alpha_preview"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_AlphaPreview;
		outConfig.flags &= ~ImGuiColorEditFlags_AlphaPreviewHalf;

		switch (mode)
		{
		case ImGuiColorEditFlags_AlphaPreview:
			outConfig.flags |= ImGuiColorEditFlags_AlphaPreview;
			break;
		case ImGuiColorEditFlags_AlphaPreviewHalf:
			outConfig.flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
			break;
		default:
			break;
		}
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "display_type"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_Uint8;
		outConfig.flags &= ~ImGuiColorEditFlags_Float;

		switch (mode)
		{
		case ImGuiColorEditFlags_Float:
			outConfig.flags |= ImGuiColorEditFlags_Float;
			break;
		default:
			outConfig.flags |= ImGuiColorEditFlags_Uint8;
			break;
		}
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "input_mode"))
	{
		long mode = ToUUID(item);

		// reset target flags
		outConfig.flags &= ~ImGuiColorEditFlags_InputRGB;
		outConfig.flags &= ~ImGuiColorEditFlags_InputHSV;

		switch (mode)
		{
		case ImGuiColorEditFlags_InputHSV:
			outConfig.flags |= ImGuiColorEditFlags_InputHSV;
			break;
		default:
			outConfig.flags |= ImGuiColorEditFlags_InputRGB;
			break;
		}
	}
}

void
DearPyGui::set_configuration(PyObject* inDict, mvColorMapScaleConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "min_scale")) outConfig.scale_min = (double)ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "max_scale")) outConfig.scale_max = (double)ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "format")) outConfig.format = ToString(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "colormap"))
	{
		outConfig.colorMap = (ImPlotColormap)GetIDFromPyObject(item);
		if (outConfig.colorMap > 10)
		{
			auto asource = GetItem(*GContext->itemRegistry, outConfig.colorMap);
			if (asource == nullptr)
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_colormap",
					"Source Item not found: " + std::to_string(outConfig.colorMap), nullptr);
				outConfig.colorMap = 0;
			}

			else if (asource->type == mvAppItemType::mvColorMap)
			{
				mvColorMap* colormap = static_cast<mvColorMap*>(asource);
				outConfig.colorMap = colormap->configData.colorMap;
			}
		}
	}

	// helpers for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};


	flagop("invert", ImPlotColormapScaleFlags_Invert, outConfig.flags);
	flagop("no_label", ImPlotColormapScaleFlags_NoLabel, outConfig.flags);
	flagop("opposite", ImPlotColormapScaleFlags_Opposite, outConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvColorButtonConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, inConfig.flags);
	checkbitset("no_border", ImGuiColorEditFlags_NoBorder, inConfig.flags);
	checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvColorEditConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, inConfig.flags);
	checkbitset("no_picker", ImGuiColorEditFlags_NoPicker, inConfig.flags);
	checkbitset("no_options", ImGuiColorEditFlags_NoOptions, inConfig.flags);
	checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, inConfig.flags);
	checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, inConfig.flags);
	checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, inConfig.flags);
	checkbitset("no_label", ImGuiColorEditFlags_NoLabel, inConfig.flags);
	checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, inConfig.flags);
	checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, inConfig.flags);

	// input_mode
	if (inConfig.flags & ImGuiColorEditFlags_InputRGB)
		PyDict_SetItemString(outDict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputRGB)));
	else if (inConfig.flags & ImGuiColorEditFlags_InputHSV)
		PyDict_SetItemString(outDict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputHSV)));

	// alpha_preview
	if (inConfig.flags & ImGuiColorEditFlags_AlphaPreview)
		PyDict_SetItemString(outDict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreview)));
	else if (inConfig.flags & ImGuiColorEditFlags_AlphaPreviewHalf)
		PyDict_SetItemString(outDict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreviewHalf)));
	else
		PyDict_SetItemString(outDict, "alpha_preview", mvPyObject(ToPyLong(0l)));

	// display_mode
	if (inConfig.flags & ImGuiColorEditFlags_DisplayHSV)
		PyDict_SetItemString(outDict, "display_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_DisplayHSV)));
	else if (inConfig.flags & ImGuiColorEditFlags_DisplayHex)
		PyDict_SetItemString(outDict, "display_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_DisplayHex)));
	else
		PyDict_SetItemString(outDict, "display_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_DisplayRGB)));

	// display_type
	if (inConfig.flags & ImGuiColorEditFlags_Uint8)
		PyDict_SetItemString(outDict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Uint8)));
	else if (inConfig.flags & ImGuiColorEditFlags_Float)
		PyDict_SetItemString(outDict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Float)));

}

void
DearPyGui::fill_configuration_dict(const mvColorPickerConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, inConfig.flags);
	checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, inConfig.flags);
	checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, inConfig.flags);
	checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, inConfig.flags);
	checkbitset("no_label", ImGuiColorEditFlags_NoLabel, inConfig.flags);
	checkbitset("no_side_preview", ImGuiColorEditFlags_NoSidePreview, inConfig.flags);
	checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, inConfig.flags);
	checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, inConfig.flags);
	checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, inConfig.flags);
	checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, inConfig.flags);

	// input_mode
	if (inConfig.flags & ImGuiColorEditFlags_InputRGB)
		PyDict_SetItemString(outDict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputRGB)));
	else if (inConfig.flags & ImGuiColorEditFlags_InputHSV)
		PyDict_SetItemString(outDict, "input_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_InputHSV)));

	// alpha_preview
	if (inConfig.flags & ImGuiColorEditFlags_AlphaPreview)
		PyDict_SetItemString(outDict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreview)));
	else if (inConfig.flags & ImGuiColorEditFlags_AlphaPreviewHalf)
		PyDict_SetItemString(outDict, "alpha_preview", mvPyObject(ToPyLong(ImGuiColorEditFlags_AlphaPreviewHalf)));
	else
		PyDict_SetItemString(outDict, "alpha_preview", mvPyObject(ToPyLong(0)));

	// display_type
	if (inConfig.flags & ImGuiColorEditFlags_Uint8)
		PyDict_SetItemString(outDict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Uint8)));
	else if (inConfig.flags & ImGuiColorEditFlags_Float)
		PyDict_SetItemString(outDict, "display_type", mvPyObject(ToPyLong(ImGuiColorEditFlags_Float)));

	// picker_mode
	if (inConfig.flags & ImGuiColorEditFlags_PickerHueWheel)
		PyDict_SetItemString(outDict, "picker_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_PickerHueWheel)));
	else if (inConfig.flags & ImGuiColorEditFlags_PickerHueBar)
		PyDict_SetItemString(outDict, "picker_mode", mvPyObject(ToPyLong(ImGuiColorEditFlags_PickerHueBar)));
}

void
DearPyGui::fill_configuration_dict(const mvColorMapScaleConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "min_scale", mvPyObject(ToPyFloat((float)inConfig.scale_min)));
	PyDict_SetItemString(outDict, "max_scale", mvPyObject(ToPyFloat((float)inConfig.scale_max)));
	PyDict_SetItemString(outDict, "format", mvPyObject(ToPyString(inConfig.format)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	checkbitset("invert", ImPlotColormapScaleFlags_Invert, inConfig.flags);
	checkbitset("no_label", ImPlotColormapScaleFlags_NoLabel, inConfig.flags);
	checkbitset("opposite", ImPlotColormapScaleFlags_Opposite, inConfig.flags);
}