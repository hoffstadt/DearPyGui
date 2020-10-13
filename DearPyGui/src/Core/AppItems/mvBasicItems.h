#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvPythonTranslator.h"
#include <utility>
#include "mvAppItem.h"

#include "Core/mvValueStorage.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSelectable
//     * mvButton
//     * mvCheckbox
//     * mvCombo
//     * mvListbox
//     * mvRadioButton
//     * mvProgressBar
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvSelectable
	//-----------------------------------------------------------------------------
	class mvSelectable : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Selectable, "add_selectable")

		mvSelectable(const std::string& name, bool default_value, const std::string& dataSource)
			: mvBoolPtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();
			ImGui::PushID(this);

			if(ImGui::Selectable(m_label.c_str(), m_value, m_flags))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags, bool flip)
			{
				if (flip)
				{
					if (PyObject* item = PyDict_GetItemString(dict, keyword)) !ToBool(item) ? flags |= flag : flags &= ~flag;
				}
				else
				{
					if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
				}
			};

			// window flags
			flagop("enabled", ImGuiSelectableFlags_Disabled, m_flags, true);
			flagop("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags, bool flip)
			{
				if(flip)
					PyDict_SetItemString(dict, keyword, ToPyBool(!(flags & flag)));
				else
					PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("enabled", ImGuiSelectableFlags_Disabled, m_flags, true);
			checkbitset("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);
		}
	private:
		ImGuiSelectableFlags m_flags = ImGuiSelectableFlags_None;
	};

	//-----------------------------------------------------------------------------
	// mvButton
	//-----------------------------------------------------------------------------
	class mvButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Button, "add_button")

		mvButton(const std::string& name)
			: mvAppItem(name)
		{
		}

		void draw() override
		{
			pushColorStyles();
			ImGui::PushID(this);

			if (!m_enabled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
				ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
				disabled_color.w = 0.392f;
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabled_color);
			}

			if (m_small)
			{
				if (ImGui::SmallButton(m_label.c_str()))
				{
					if(m_enabled)
						mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				}

				ImGui::PopID();
				if (!m_enabled) ImGui::PopStyleColor(4);
				popColorStyles();
				return;
			}

			if (m_arrow)
			{
				if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				{
					if (m_enabled)
						mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				}

				ImGui::PopID();
				if (!m_enabled) ImGui::PopStyleColor(4);
				popColorStyles();
				return;
			}

			if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			{

				if (m_enabled)
					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(4);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			if (PyObject* item = PyDict_GetItemString(dict, "small")) m_small = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "arrow")) m_arrow = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "direction")) m_direction = ToInt(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "small", ToPyBool(m_small));
			PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
			PyDict_SetItemString(dict, "direction", ToPyInt(m_direction));
		}

	private:

		bool     m_small = false;
		bool     m_arrow = false;
		ImGuiDir m_direction = ImGuiDir_Up;

	};

	//-----------------------------------------------------------------------------
	// mvCheckbox
	//-----------------------------------------------------------------------------
	class mvCheckbox : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Checkbox, "add_checkbox")

		mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource)
			: mvBoolPtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();
			ImGui::PushID(this);

			if (ImGui::Checkbox(m_label.c_str(), m_value))
				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);


			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			popColorStyles();
		}

	};

	//-----------------------------------------------------------------------------
	// mvCombo
	//-----------------------------------------------------------------------------
	class mvCombo : public mvStringPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Combo, "add_combo")

			mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource)
			: mvStringPtrBase(name, default_value, dataSource)
		{}

		void draw() override
		{

			pushColorStyles();
			ImGui::PushID(this);

			if (ImGui::BeginCombo(m_label.c_str(), m_value->c_str(), m_flags)) // The second parameter is the label previewed before opening the combo.
			{
				for (const auto& name : m_items)
				{
					bool is_selected = (*m_value == name);
					if (ImGui::Selectable((name).c_str(), is_selected))
					{
						*m_value = name;
						mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				ImGui::EndCombo();
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "items")) m_items = ToStringVect(item);

			// helpers for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
			{

				for (size_t i = 0; i < keywords.size(); i++)
				{
					if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
					{
						//turning all conflicting flags false
						for (const auto& flag : flags) mflags &= ~flag;
						//writing only the first conflicting flag
						ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
						break;
					}
				}

			};

			flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
			flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
			flagop("no_preview", ImGuiComboFlags_NoPreview, m_flags);

			std::vector<std::string> HeightKeywords{
				"height_small",
				"height_regular",
				"height_large",
				"height_largest" };
			std::vector<int> HeightFlags{
				ImGuiComboFlags_HeightSmall,
				ImGuiComboFlags_HeightRegular,
				ImGuiComboFlags_HeightLarge,
				ImGuiComboFlags_HeightLargest };

			conflictingflagop(HeightKeywords, HeightFlags, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "items", ToPyList(m_items));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};
			checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
			checkbitset("height_small", ImGuiComboFlags_HeightSmall, m_flags);
			checkbitset("height_regular", ImGuiComboFlags_HeightRegular, m_flags);
			checkbitset("height_large", ImGuiComboFlags_HeightLarge, m_flags);
			checkbitset("height_largest", ImGuiComboFlags_HeightLargest, m_flags);
			checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
			checkbitset("no_preview", ImGuiComboFlags_NoPreview, m_flags);
		}

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;
		std::vector<std::string> m_items;
	};

	//-----------------------------------------------------------------------------
	// mvListbox
	//-----------------------------------------------------------------------------
	class mvListbox : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox, "add_listbox")

		mvListbox(const std::string& name, int default_value, const std::string& dataSource)
			: mvIntPtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();
			ImGui::PushID(this);

			if (ImGui::ListBox(m_label.c_str(), m_value, m_charNames.data(), m_names.size(), m_itemsHeight))
				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);


			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "num_items")) m_itemsHeight = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(dict, "items"))
			{
				m_names = ToStringVect(item);
				m_charNames.clear();
				for (const std::string& item : m_names)
					m_charNames.emplace_back(item.c_str());
			}
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "items", ToPyList(m_names));
			PyDict_SetItemString(dict, "num_items", ToPyInt(m_itemsHeight));
		}

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight = 3; // number of items to show (default -1)
		std::vector<const char*> m_charNames;

	};

	//-----------------------------------------------------------------------------
	// mvRadioButton
	//-----------------------------------------------------------------------------
	class mvRadioButton : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::RadioButtons, "add_radio_button")

		mvRadioButton(const std::string& name, int default_value, const std::string& dataSource)
			: mvIntPtrBase(name, default_value, dataSource)
		{

		}

		void draw() override
		{

			ImGui::BeginGroup();

			pushColorStyles();
			ImGui::PushID(this);

			for (size_t i = 0; i < m_itemnames.size(); i++)
			{
				if (m_horizontal && i != 0)
					ImGui::SameLine();

				if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), m_value, i))
					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

			ImGui::PopID();
			popColorStyles();

			ImGui::EndGroup();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "items")) m_itemnames = ToStringVect(item);
			if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "items", ToPyList(m_itemnames));
			PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		}

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal = false;

	};

	//-----------------------------------------------------------------------------
	// mvProgressBar
	//-----------------------------------------------------------------------------
	class mvProgressBar : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ProgressBar, "add_progress_bar")

		mvProgressBar(const std::string& name, float default_value, const std::string& dataSource)
			: mvFloatPtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();
			ImGui::PushID(this);

			ImGui::ProgressBar(*m_value, ImVec2((float)m_width, (float)m_height), m_overlay.c_str());

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
		}

	private:

		std::string m_overlay;

	};

}
