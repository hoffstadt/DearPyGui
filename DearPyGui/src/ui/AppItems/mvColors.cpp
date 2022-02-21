#include "mvColors.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/mvItemHandlers.h"

void 
DearPyGui::apply_template(const mvColorButtonConfig& sourceConfig, mvColorButtonConfig& dstConfig)
{

	dstConfig.value = sourceConfig.value;
	dstConfig.flags = sourceConfig.flags;
	dstConfig.no_border = sourceConfig.no_border;
	dstConfig.disabled_value[0] = sourceConfig.disabled_value[0];
	dstConfig.disabled_value[1] = sourceConfig.disabled_value[1];
	dstConfig.disabled_value[2] = sourceConfig.disabled_value[2];
	dstConfig.disabled_value[3] = sourceConfig.disabled_value[3];
}

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
DearPyGui::set_positional_configuration(PyObject* inDict, mvColorButtonConfig& outConfig)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvColorButton)], inDict))
		return;

	for (int i = 0; i < PyTuple_Size(inDict); i++)
	{
		PyObject* item = PyTuple_GetItem(inDict, i);
		mvColor color = ToColor(item);
		switch (i)
		{
		case 0:
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

PyObject* mvColorButton::getPyValue()
{
	// nasty hack
	int r = (int)(configData.value->data()[0] * 255.0f * 255.0f);
	int g = (int)(configData.value->data()[1] * 255.0f * 255.0f);
	int b = (int)(configData.value->data()[2] * 255.0f * 255.0f);
	int a = (int)(configData.value->data()[3] * 255.0f * 255.0f);

	auto color = mvColor(r, g, b, a);
	return ToPyColor(color);
}

void mvColorButton::setPyValue(PyObject* value)
{
	mvColor color = ToColor(value);
	std::array<float, 4> temp_array;
	temp_array[0] = color.r;
	temp_array[1] = color.g;
	temp_array[2] = color.b;
	temp_array[3] = color.a;
	if (configData.value)
		*configData.value = temp_array;
	else
		configData.value = std::make_shared<std::array<float, 4>>(temp_array);
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