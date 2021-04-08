#include "mvColorButton.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvColorButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");

		parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::OPTIONAL, "(0, 0, 0, 255)");

		parser.addArg<mvPyDataType::Bool>("no_alpha", mvArgType::KEYWORD, "False", "ignore Alpha component");
		parser.addArg<mvPyDataType::Bool>("no_border", mvArgType::KEYWORD, "False", "disable border");
		parser.addArg<mvPyDataType::Bool>("no_drag_drop", mvArgType::KEYWORD, "False", "disable display of inline text label");

		parser.finalize();

		parsers->insert({ s_command, parser });
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