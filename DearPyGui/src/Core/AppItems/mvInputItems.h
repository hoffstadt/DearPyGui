#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvPythonTranslator.h"
#include <utility>
#include "mvAppItem.h"

#include "Core/mvValueStorage.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputInt
//     * mvInputInt2
//     * mvInputInt3
//     * mvInputInt4
//     * mvInputFloat
//     * mvInputFloat2
//     * mvInputFloat3
//     * mvInputFloat4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvInputInt
	//-----------------------------------------------------------------------------
	class mvInputInt : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt, "add_input_int")

		mvInputInt(const std::string& name, int default_value, const std::string& dataSource)
			: mvIntPtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::InputInt(m_label.c_str(), m_value, 1, 100, m_flags))
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
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
		}

	private:

		ImGuiInputTextFlags m_flags = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInputInt2
	//-----------------------------------------------------------------------------
	class mvInputInt2 : public mvInt2PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt2, "add_input_int")

		mvInputInt2(const std::string& name, int* default_value, const std::string& dataSource)
			: mvInt2PtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::InputInt2(m_label.c_str(), m_value, m_flags))
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
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
		}

	private:

		ImGuiInputTextFlags m_flags = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInputInt3
	//-----------------------------------------------------------------------------
	class mvInputInt3 : public mvInt3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt3, "add_input_int3")

		mvInputInt3(const std::string& name, int* default_value, const std::string& dataSource)
			: mvInt3PtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::InputInt3(m_label.c_str(), m_value, m_flags))
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
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
		}

	private:

		ImGuiInputTextFlags m_flags = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInputInt4
	//-----------------------------------------------------------------------------
	class mvInputInt4 : public mvInt4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt4, "add_input_int4")

		mvInputInt4(const std::string& name, int* default_value, const std::string& dataSource)
			: mvInt4PtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{

			pushColorStyles();

			if (ImGui::InputInt4(m_label.c_str(), m_value, m_flags))
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
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
		}

	private:

		ImGuiInputTextFlags m_flags = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInputFloat
	//-----------------------------------------------------------------------------
	class mvInputFloat : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat, "add_input_float")

		mvInputFloat(const std::string& name, float default_value, const std::string& dataSource)
			: mvFloatPtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImGui::InputFloat(m_label.c_str(), m_value, 0.0f, 0.0f, m_format.c_str(), m_flags))
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
			if (dict == nullptr)
				return;
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
			if (dict == nullptr)
				return;
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

		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloat2
	//-----------------------------------------------------------------------------
	class mvInputFloat2: public mvFloat2PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat2, "add_input_float2")

		mvInputFloat2(const std::string& name, float* default_value, const std::string& dataSource)
			: mvFloat2PtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImGui::InputFloat2(m_label.c_str(), m_value, m_format.c_str(), m_flags))
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
			if (dict == nullptr)
				return;
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
			if (dict == nullptr)
				return;
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

		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloat3
	//-----------------------------------------------------------------------------
	class mvInputFloat3 : public mvFloat3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat3, "add_input_float3")

		mvInputFloat3(const std::string& name, float* default_value, const std::string& dataSource)
			: mvFloat3PtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImGui::InputFloat3(m_label.c_str(), m_value, m_format.c_str(), m_flags))
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
			if (dict == nullptr)
				return;
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
			if (dict == nullptr)
				return;
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

		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloat4
	//-----------------------------------------------------------------------------
	class mvInputFloat4 : public mvFloat4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat4, "add_input_float4")

		mvInputFloat4(const std::string& name, float* default_value, const std::string& dataSource)
			: mvFloat4PtrBase(name, default_value, dataSource)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImGui::InputFloat4(m_label.c_str(), m_value, m_format.c_str(), m_flags))
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
			if (dict == nullptr)
				return;
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
			if (dict == nullptr)
				return;
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

		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = 0;

	};

}