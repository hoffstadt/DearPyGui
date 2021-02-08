#pragma once
#include "mvTypeBases.h"
#include "mvMarvel.h"

namespace Marvel {

	PyObject* add_debug_window(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDebugWindow : public mvBaseWindowAppitem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::DebugWindow, "add_debug_window")

	public:

		mvDebugWindow(const std::string& name);

		void draw() override;

		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem;
		ImGuiID     m_selectedID = 0u;
		std::vector<std::pair<std::string, std::string>> m_commands;
	};

}