#pragma once
#include "mvTypeBases.h"
#include "mvMarvel.h"
#include "mvPythonParser.h"

namespace Marvel {

	PyObject* add_debug_window(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDebugWindow : public mvBaseWindowAppitem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::DebugWindow, "add_debug_window")

	public:

		mvDebugWindow(const std::string& name)
			: mvBaseWindowAppitem(name)
		{
			m_width = 700;
			m_height = 500;
			m_description.deleteAllowed = false;
			m_commands = GetAllCommands();
		}

		void draw() override;

		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem;
		ImGuiID     m_selectedID;
		std::vector<std::pair<std::string, std::string>> m_commands;
	};

}