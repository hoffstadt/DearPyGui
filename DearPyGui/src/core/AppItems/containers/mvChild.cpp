#include "mvChild.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvChild::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Bool, "border", "", "True"},
			{mvPythonDataType::Bool, "autosize_x", "Autosize the window to fit it's items in the x.", "False"},
			{mvPythonDataType::Bool, "autosize_y", "Autosize the window to fit it's items in the y.", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "menubar", "", "False"},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end.",
		"None", "Containers") });
	}

	mvChild::mvChild(const std::string& name)
		: mvBoolPtrBase(name, false)
	{
	}

	void mvChild::addFlag(ImGuiWindowFlags flag)
	{
		m_windowflags |= flag;
	}

	void mvChild::removeFlag(ImGuiWindowFlags flag)
	{
		m_windowflags &= ~flag;
	}

	void mvChild::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		ImGui::BeginChild(m_label.c_str(), ImVec2(m_autosize_x ? 0 : (float)m_width, m_autosize_y ? 0 : (float)m_height), m_border, m_windowflags);

		//we do this so that the children dont get the theme
		scope.cleanup();

		for (auto item : m_children1)
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->getState().update();
		}

		// allows this item to have a render callback
		registerWindowFocusing();

		m_width = (int)ImGui::GetWindowWidth();
		m_height = (int)ImGui::GetWindowHeight();

		ImGui::EndChild();
	}

	void mvChild::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "border")) m_border = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_x")) m_autosize_x = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_y")) m_autosize_y = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
	}

	void mvChild::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		PyDict_SetItemString(dict, "autosize_x", ToPyBool(m_autosize_x));
		PyDict_SetItemString(dict, "autosize_y", ToPyBool(m_autosize_y));

		// helper for bit flipping
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
	}

	PyObject* mvChild::add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int show = true;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int border = true;
		int autosize_x = false;
		int autosize_y = false;
		int no_scrollbar = false;
		int horizontal_scrollbar = false;
		int menubar = false;

		if (!(mvApp::GetApp()->getParsers())["add_child"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &parent, &before, &width, &height, &border, &autosize_x,
			&autosize_y, &no_scrollbar, &horizontal_scrollbar, &menubar))
			return ToPyBool(false);

		auto item = CreateRef<mvChild>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return ToPyString(name);

	}

}