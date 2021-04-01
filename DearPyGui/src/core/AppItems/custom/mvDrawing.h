#pragma once

#include "mvTypeBases.h"
#include "mvDrawList.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawing);
	class mvDrawing : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvDrawing, add_drawing)

		MV_CREATE_EXTRA_COMMAND(bring_draw_command_forward);
		MV_CREATE_EXTRA_COMMAND(bring_draw_command_to_front);
		MV_CREATE_EXTRA_COMMAND(send_draw_command_back);
		MV_CREATE_EXTRA_COMMAND(send_draw_command_to_back);
		MV_CREATE_EXTRA_COMMAND(modify_draw_command);
		MV_CREATE_EXTRA_COMMAND(get_draw_command);
		MV_CREATE_EXTRA_COMMAND(clear_drawing);
		MV_CREATE_EXTRA_COMMAND(delete_draw_command);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(bring_draw_command_forward);
			MV_ADD_EXTRA_COMMAND(bring_draw_command_to_front);
			MV_ADD_EXTRA_COMMAND(send_draw_command_back);
			MV_ADD_EXTRA_COMMAND(send_draw_command_to_back);
			MV_ADD_EXTRA_COMMAND(modify_draw_command);
			MV_ADD_EXTRA_COMMAND(get_draw_command);

			MV_ADD_EXTRA_COMMAND(clear_drawing);
			MV_ADD_EXTRA_COMMAND(delete_draw_command);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvDrawing(const std::string& name);

		void draw() override;

		mvRef<mvDrawList> getDrawList();


	private:

		mvRef<mvDrawList> m_drawList;
		float             m_startx = 0.0f;
		float             m_starty = 0.0f;

	};
}