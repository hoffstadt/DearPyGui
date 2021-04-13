#include "mvCollapsingHeader.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvCollapsingHeader::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("source");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("closable", mvArgType::KEYWORD, "False");
		parser.addArg<mvPyDataType::Bool>("default_open", mvArgType::KEYWORD, "False");
		parser.addArg<mvPyDataType::Bool>("open_on_double_click", mvArgType::KEYWORD, "False", "Need double-click to open node");
		parser.addArg<mvPyDataType::Bool>("open_on_arrow", mvArgType::KEYWORD, "False", "Only open when clicking on the arrow part.");
		parser.addArg<mvPyDataType::Bool>("leaf", mvArgType::KEYWORD, "False", "No collapsing, no arrow (use as a convenience for leaf nodes).");
		parser.addArg<mvPyDataType::Bool>("bullet", mvArgType::KEYWORD, "False", "Display a bullet instead of arrow");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCollapsingHeader::mvCollapsingHeader(const std::string& name)
		: mvBoolPtrBase(name)
	{
	}

	void mvCollapsingHeader::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		bool* toggle = nullptr;
		if (m_closable)
			toggle = &m_show;
		*m_value = ImGui::CollapsingHeader(m_label.c_str(), toggle, m_flags);

		//we do this so that the children dont get the theme
		scope.cleanup();

		if (*m_value)
		{
			for (auto& item : m_children[1])
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
		}
	}

	void mvCollapsingHeader::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);


	}

	void mvCollapsingHeader::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);

	}

}