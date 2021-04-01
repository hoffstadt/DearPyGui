#pragma once
#include "mvTypeBases.h"
#include "mvApp.h"
#include <ImGuiFileDialog.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvFileDialog);
	class mvFileDialog : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvFileDialog, open_file_dialog)

		MV_CREATE_EXTRA_COMMAND(select_directory_dialog);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(select_directory_dialog);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvFileDialog();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool prerender2 ();
		void setCallback(mvCallable callback);

	private:

		mvCallable   m_callback2 = nullptr;
	};

}