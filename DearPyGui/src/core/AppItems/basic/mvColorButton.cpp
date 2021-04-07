#include "mvColorButton.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvColorButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},		
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 255)"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Bool, "show", "Attemp to render", "True"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component", "False"},
			{mvPythonDataType::Bool, "no_border", "disable border (which is enforced by default)", "False"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label", "False"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a color button.", "None", "Adding Widgets") });
	}

	mvColorButton::mvColorButton(const std::string& name)
		: 
		mvColorPtrBase(name)
	{
	}

	void mvColorButton::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		ImVec4 col = { (*m_value)[0], (*m_value)[1], (*m_value)[2], (*m_value)[3] };

		if (ImGui::ColorButton(m_label.c_str(), col, m_flags, ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

	}

	void mvColorButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};
		flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		flagop("no_border", ImGuiColorEditFlags_NoBorder, m_flags);
		flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
	}

	void mvColorButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
		checkbitset("no_border", ImGuiColorEditFlags_NoBorder, m_flags);
		checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
	}

}