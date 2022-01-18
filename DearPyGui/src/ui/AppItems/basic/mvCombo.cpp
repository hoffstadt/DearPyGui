#include <utility>
#include "mvCombo.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

	mvCombo::mvCombo(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	PyObject* mvCombo::getPyValue()
	{
		return ToPyString(*value);
	}

	void mvCombo::setPyValue(PyObject* value)
	{
		*this->value = ToString(value);
	}

	void mvCombo::setDataSource(mvUUID dataSource)
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
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}

	void mvCombo::draw(ImDrawList* drawlist, float x, float y)
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
			ScopedID id(uuid);

			static std::vector<std::string> disabled_items{};

			// The second parameter is the label previewed before opening the combo.
			bool activated = ImGui::BeginCombo(info.internalLabel.c_str(), value->c_str(), configData.flags);
			UpdateAppItemState(state);

			if(activated)
			{

				for (const auto& name : config.enabled ? configData.items : disabled_items)
				{
					bool is_selected = (*value == name);
					if (ImGui::Selectable((name).c_str(), is_selected))
					{
						if (config.enabled) { *value = name; }

						auto value = *this->value;

						if(config.alias.empty())
							mvSubmitCallback([=]() {
								mvAddCallback(getCallback(false), uuid, ToPyString(value), config.user_data);
								});
						else
							mvSubmitCallback([=]() {
								mvAddCallback(getCallback(false), config.alias, ToPyString(value), config.user_data);
									});


					}

					state.edited = ImGui::IsItemEdited();
					state.deactivated = ImGui::IsItemDeactivated();
					state.deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();

					// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
		}

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

	void mvCombo::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvCombo*>(item);
		if (config.source != 0) value = titem->value;
		disabled_value = titem->disabled_value;
		configData.flags = titem->configData.flags;
		configData.items = titem->configData.items;
		configData.popup_align_left = titem->configData.popup_align_left;
		configData.no_preview = titem->configData.no_preview;
	}

	void mvCombo::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				configData.items = ToStringVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvCombo::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "items")) configData.items = ToStringVect(item);

		if (PyObject* item = PyDict_GetItemString(dict, "height_mode"))
		{
			long height_mode = (long)ToUUID(item);

			if (height_mode == (long)mvComboHeightMode::mvComboHeight_Small)
				configData.flags = ImGuiComboFlags_HeightSmall;
			else if (height_mode == (long)mvComboHeightMode::mvComboHeight_Regular)
				configData.flags = ImGuiComboFlags_HeightRegular;
			else if (height_mode == (long)mvComboHeightMode::mvComboHeight_Large)
				configData.flags = ImGuiComboFlags_HeightLarge;
			else
				configData.flags = ImGuiComboFlags_HeightLargest;
		}

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, configData.flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, configData.flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, configData.flags);

	}

	void mvCombo::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			mvPyObject py_value = ToPyBool(flags & flag);
			PyDict_SetItemString(dict, keyword, py_value);
		};
		checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, configData.flags);
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, configData.flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, configData.flags);

		mvUUID mode;
		if (configData.flags & ImGuiComboFlags_HeightSmall)
			mode = (long)mvComboHeightMode::mvComboHeight_Small;
		else if (configData.flags & ImGuiComboFlags_HeightRegular)
			mode = (long)mvComboHeightMode::mvComboHeight_Regular;
		else if (configData.flags & ImGuiComboFlags_HeightLarge)
			mode = (long)mvComboHeightMode::mvComboHeight_Large;
		else
			mode = (long)mvComboHeightMode::mvComboHeight_Largest;

		mvPyObject py_height_mode = ToPyLong(mode);
		mvPyObject py_items = ToPyList(configData.items);
		PyDict_SetItemString(dict, "height_mode", py_height_mode);
		PyDict_SetItemString(dict, "items", py_items);
	}

}