#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvPythonTranslator.h"
#include <utility>
#include "mvAppItem.h"

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
//     * mvInputInt
//     * mvInputIntMulti
//     * mvInputFloat
//     * mvInputFloatMulti
//     * mvDragFloat
//     * mvDragInt
//     * mvSliderFloat
//     * mvSliderInt
//     * mvSliderFloatMulti
//     * mvSliderIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Helper typedefs
	//-----------------------------------------------------------------------------
	template<typename T>
	using ImGuiSlideCommand = bool(*)(const char*, T*, T, T, const char*, ImGuiSliderFlags);
	template<typename T>
	using ImGuiDragCommand = bool(*)(const char*, T*, float, T, T, const char*, ImGuiSliderFlags);
	using ImGuiIntInputCommand = bool(*)(const char*, int*, ImGuiInputTextFlags);
	using ImGuiFloatInputCommand = bool(*)(const char*, float*, const char*, ImGuiInputTextFlags);
	using ImGuiIntCommand = bool(*)(const char*, int*, ImGuiInputTextFlags);
	using ImGuiFloatCommand = bool(*)(const char*, float*, const char*, ImGuiInputTextFlags);

	//-----------------------------------------------------------------------------
	// mvSelectable
	//-----------------------------------------------------------------------------
	class mvSelectable : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Selectable)

		mvSelectable(const std::string& name, bool default_value, ImGuiSelectableFlags flags)
			: mvBoolItemBase(name, default_value), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if(ImGui::Selectable(m_label.c_str(), &m_value, m_flags))
			{

				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// window flags
			flagop("disabled", ImGuiSelectableFlags_Disabled, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("disabled", ImGuiSelectableFlags_Disabled, m_flags);
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

		MV_APPITEM_TYPE(mvAppItemType::Button)

		mvButton(const std::string& name, bool small, 
			bool arrow, ImGuiDir direction)
			: mvAppItem(name), m_small(small), m_arrow(arrow), m_direction(direction)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (m_small)
			{
				if (ImGui::SmallButton(m_label.c_str()))
				{
					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				return;
			}

			if (m_arrow)
			{
				if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				{
					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				return;
			}

			if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "small")) m_small = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "arrow")) m_arrow = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "direction")) m_direction = ToInt(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "small", ToPyBool(m_small));
			PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
			PyDict_SetItemString(dict, "direction", ToPyInt(m_direction));
		}

	private:

		bool     m_small;
		bool     m_arrow;
		ImGuiDir m_direction = ImGuiDir_Up;

	};

	//-----------------------------------------------------------------------------
	// mvCheckbox
	//-----------------------------------------------------------------------------
	class mvCheckbox : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Checkbox)

		mvCheckbox(const std::string& name, bool default_value)
			: mvBoolItemBase(name, default_value)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImGui::Checkbox(m_label.c_str(), &m_value))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

	};

	//-----------------------------------------------------------------------------
	// mvCombo
	//-----------------------------------------------------------------------------
	class mvCombo : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Combo)

		mvCombo(const std::string& name, std::vector<std::string> itemnames, const std::string& default_value)
			: mvStringItemBase(name, default_value), m_items(std::move(itemnames))
		{}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::BeginCombo(m_label.c_str(), m_value.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (const auto& name : m_items)
				{
					bool is_selected = (m_value == name);
					if (ImGui::Selectable((name + "##" + m_name).c_str(), is_selected))
					{
						if (!m_dataSource.empty())
							mvDataStorage::AddData(m_dataSource, getPyValue());

						m_value = name;
						mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());

				ImGui::EndCombo();
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "items")) m_items = ToStringVect(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "items", ToPyList(m_items));
		}

	private:

		std::vector<std::string> m_items;
	};

	//-----------------------------------------------------------------------------
	// mvListbox
	//-----------------------------------------------------------------------------
	class mvListbox : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox)

		std::string getParserCommand() const override { return "add_listbox"; }

		mvListbox(const std::string& name, std::vector<std::string> itemnames, int default_value, int height)
			: mvIntItemBase(name, 1, default_value), m_names(std::move(itemnames)), m_itemsHeight(height)
		{
			for (const std::string& item : m_names)
				m_charNames.emplace_back(item.c_str());
		}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::ListBox(m_label.c_str(), &m_value[0], m_charNames.data(), m_names.size(), m_itemsHeight))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
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
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "items", ToPyList(m_names));
		}

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight; // number of items to show (default -1)
		std::vector<const char*> m_charNames;

	};

	//-----------------------------------------------------------------------------
	// mvRadioButton
	//-----------------------------------------------------------------------------
	class mvRadioButton : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::RadioButtons)

		mvRadioButton(const std::string& name, std::vector<std::string> itemnames, int default_value, bool horizontal)
			: mvIntItemBase(name, 1, default_value), m_itemnames(std::move(itemnames)), m_horizontal(horizontal)
		{
		}

		void draw() override
		{
			pushColorStyles();

			for (size_t i = 0; i < m_itemnames.size(); i++)
			{
				if (m_horizontal && i != 0)
					ImGui::SameLine();

				if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), &m_value[0], i))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

			popColorStyles();

		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "items")) m_itemnames = ToStringVect(item);
			if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "items", ToPyList(m_itemnames));
			PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		}

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal;

	};

	//-----------------------------------------------------------------------------
	// mvProgressBar
	//-----------------------------------------------------------------------------
	class mvProgressBar : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ProgressBar)

		mvProgressBar(const std::string& name, float default_value = 0.0f, std::string overlay = "")
			: mvFloatItemBase(name, 1, default_value), m_overlay(std::move(overlay))
		{
		}

		void draw() override
		{

			pushColorStyles();

			ImGui::ProgressBar(m_value[0], ImVec2((float)m_width, (float)m_height), m_overlay.c_str());

			// Context Menu
			if (!getPopup().empty())
				ImGui::OpenPopup(getPopup().c_str());

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();

		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
		}

	private:

		std::string m_overlay;

	};

	//-----------------------------------------------------------------------------
	// mvInputInt
	//-----------------------------------------------------------------------------
	class mvInputInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

		mvInputInt(const std::string& name, int default_value, ImGuiInputTextFlags flags)
			: mvIntItemBase(name, 1, default_value), m_flags(flags) {}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::InputInt(m_label.c_str(), m_value.data(), 1, 100, m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
		}

	private:

		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputIntMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiIntInputCommand imguicommand>
	class mvInputIntMulti : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvInputIntMulti(const std::string& name, int default_value[2], ImGuiInputTextFlags flags)
			: mvIntItemBase(name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_flags(flags){}

		void draw() override
		{

			pushColorStyles();

			if (imguicommand(m_label.c_str(), m_value.data(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
		}

	private:

		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloat
	//-----------------------------------------------------------------------------
	class mvInputFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

			mvInputFloat(const std::string& name, float default_value, std::string format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(name, 1, default_value), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImGui::InputFloat(m_label.c_str(), m_value.data(), 0.0f, 0.0f, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloatMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiFloatInputCommand imguicommand>
	class mvInputFloatMulti : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

			mvInputFloatMulti(const std::string& name, float* default_value, std::string  format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(name, num, default_value[0], default_value[1], default_value[2], default_value[3]), m_format(std::move(format)),
				m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (imguicommand(m_label.c_str(), m_value.data(), m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvDragFloat
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiDragCommand<float> imguicommand>
	class mvDragFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvDragFloat(const std::string& name, float* default_value, float speed,
				float minv, float maxv, std::string format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_speed(speed), m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (imguicommand(m_label.c_str(), m_value.data(), m_speed, m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
			PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		float               m_speed;
		float               m_min;
		float               m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiDragCommand<int> imguicommand >
	class mvDragInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvDragInt(const std::string& name, int* default_value, float speed,
				int minv, float maxv, std::string format, ImGuiInputTextFlags flags)
			: mvIntItemBase(name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_speed(speed), m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (imguicommand(m_label.c_str(), m_value.data(), m_speed, m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
			PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		float               m_speed;
		int                 m_min;
		int                 m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvSliderFloat
	//-----------------------------------------------------------------------------
	class mvSliderFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderFloat)

			mvSliderFloat(const std::string& name, float default_value = 0.0f, float minv = 0.0f,
				float maxv = 1.0f, std::string  format = "%.3f", bool vertical = false, ImGuiInputTextFlags flags = 0)
			: mvFloatItemBase(name, 1, default_value), m_min(minv), m_max(maxv), m_format(std::move(format)),
			m_vertical(vertical), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (m_vertical)
			{
				if ((float)m_height < 1.0f)
					m_height = 100.f;
				if ((float)m_width < 1.0f)
					m_width = 20.f;

				if (ImGui::VSliderFloat(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_value.data(), m_min, m_max, m_format.c_str()))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}
			else
			{
				if (ImGui::SliderFloat(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "vertical", ToPyBool(m_vertical));
			PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		float               m_min;
		float               m_max;
		std::string         m_format;
		bool                m_vertical;
		ImGuiInputTextFlags m_flags;

	};

	//-----------------------------------------------------------------------------
	// mvSliderInt
	//-----------------------------------------------------------------------------
	class mvSliderInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderInt)

			mvSliderInt(const std::string& name, int default_value = 0, int minv = 0,
				int maxv = 100, std::string  format = "%d", bool vertical = false, ImGuiInputTextFlags flags = 0)
			: mvIntItemBase(name, 1, default_value), m_min(minv), m_max(maxv), m_format(std::move(format)), 
			m_vertical(vertical), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (m_vertical)
			{
				if ((float)m_height < 1.0f)
					m_height = 100.f;
				if ((float)m_width < 1.0f)
					m_width = 20.f;

				if (ImGui::VSliderInt(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_value.data(), m_min, m_max, m_format.c_str()))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}
			else
			{
				if (ImGui::SliderInt(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "vertical", ToPyBool(m_vertical));
			PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		int                 m_min;
		int                 m_max;
		std::string         m_format;
		bool                m_vertical;
		ImGuiInputTextFlags m_flags;

	};

	//-----------------------------------------------------------------------------
	// mvSliderFloatMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiSlideCommand<float> imguicommand, typename T>
	class mvSliderFloatMulti : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

			mvSliderFloatMulti(const std::string& name, float* default_value, float minv, float maxv, std::string  format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (imguicommand(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		float               m_min;
		float               m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvSliderIntMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiSlideCommand<int> imguicommand, typename T>
	class mvSliderIntMulti : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

			mvSliderIntMulti(const std::string& name, int* default_value, int minv, int maxv, std::string  format, ImGuiInputTextFlags flags)
			: mvIntItemBase(name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (imguicommand(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);

		}

	private:

		int                 m_min;
		int                 m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};
}