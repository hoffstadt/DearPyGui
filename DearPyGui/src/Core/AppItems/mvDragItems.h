#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvPythonTranslator.h"
#include <utility>
#include "mvAppItem.h"

#include "Core/mvValueStorage.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragFloat2
//     * mvDragFloat3
//     * mvDragFloat4
//     * mvDragInt
//     * mvDragInt2
//     * mvDragInt3
//     * mvDragInt4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvDragFloat
	//-----------------------------------------------------------------------------
	class mvDragFloat : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragFloat, "add_drag_float")

		mvDragFloat(const std::string& name, float default_value, const std::string& dataSource)
			: mvFloatPtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				m_disabled_value = *m_value;
			}


			if (ImGui::DragFloat(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		float               m_min = 0.0f;
		float               m_max = 100.0f;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragFloat2
	//-----------------------------------------------------------------------------
	class mvDragFloat2 : public mvFloat2PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragFloat2, "add_drag_float2")

		mvDragFloat2(const std::string& name, float* default_value, const std::string& dataSource)
			: mvFloat2PtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				std::copy(m_value, m_value + 2, m_disabled_value);
			}

			if (ImGui::DragFloat2(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		float               m_min = 0.0f;
		float               m_max = 100.0f;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragFloat3
	//-----------------------------------------------------------------------------
	class mvDragFloat3 : public mvFloat3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragFloat3, "add_drag_float3")

		mvDragFloat3(const std::string& name, float* default_value, const std::string& dataSource)
			: mvFloat3PtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				std::copy(m_value, m_value + 3, m_disabled_value);
			}

			if (ImGui::DragFloat3(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		float               m_min = 0.0f;
		float               m_max = 100.0f;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragFloat4
	//-----------------------------------------------------------------------------
	class mvDragFloat4 : public mvFloat4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragFloat4, "add_drag_float4")

		mvDragFloat4(const std::string& name, float* default_value, const std::string& dataSource)
			: mvFloat4PtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				std::copy(m_value, m_value + 4, m_disabled_value);
			}

			if (ImGui::DragFloat4(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
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
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		float               m_min = 0.0f;
		float               m_max = 100.0f;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt
	//-----------------------------------------------------------------------------
	class mvDragInt : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragInt, "add_drag_int")

		mvDragInt(const std::string& name, int default_value, const std::string& dataSource)
			: mvIntPtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				m_disabled_value = *m_value;
			}

			if (ImGui::DragInt(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
			PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		int                 m_min = 0;
		int                 m_max = 100;
		std::string         m_format = "%d";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt2
	//-----------------------------------------------------------------------------
	class mvDragInt2 : public mvInt2PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragInt2, "add_drag_int2")

		mvDragInt2(const std::string& name, int* default_value, const std::string& dataSource)
			: mvInt2PtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				std::copy(m_value, m_value + 2, m_disabled_value);
			}

			if (ImGui::DragInt2(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
			PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		int                 m_min = 0;
		int                 m_max = 100;
		std::string         m_format = "%d";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt3
	//-----------------------------------------------------------------------------
	class mvDragInt3 : public mvInt3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragInt3, "add_drag_int3")

		mvDragInt3(const std::string& name, int* default_value, const std::string& dataSource)
			: mvInt3PtrBase(name, default_value, dataSource)
		{

		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				std::copy(m_value, m_value + 3, m_disabled_value);
			}

			if (ImGui::DragInt3(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
			PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		int                 m_min = 0;
		int                 m_max = 100;
		std::string         m_format = "%d";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt4
	//-----------------------------------------------------------------------------
	class mvDragInt4 : public mvInt4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragInt4, "add_drag_int4")

		mvDragInt4(const std::string& name, int* default_value, const std::string& dataSource)
			: mvInt4PtrBase(name, default_value, dataSource)
		{
			m_description.disableAllowed = true;
		}

		void setEnabled(bool value) override
		{
			if (value)
			{
				m_flags = m_stor_flags;
			}
			else
			{
				m_stor_flags = m_flags;
				m_flags |= ImGuiSliderFlags_NoInput;
			}

			m_enabled = value;
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
				ImGui::PushStyleColor(ImGuiCol_FrameBg, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, disabled_color);
				ImGui::PushStyleColor(ImGuiCol_Button, disabled_color);
				std::copy(m_value, m_value + 4, m_disabled_value);
			}

			if (ImGui::DragInt4(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
				mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::PopID();
			if (!m_enabled) ImGui::PopStyleColor(5);
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "format", ToPyString(m_format));
			PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
			PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
			PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// window flags
			checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
			checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

		}

	private:

		float               m_speed = 1.0f;
		int                 m_min = 0;
		int                 m_max = 100;
		std::string         m_format = "%d";
		ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;

	};

}