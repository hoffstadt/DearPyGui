#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawLine, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawLine : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawLine, draw_line)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDrawLine(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:

		mvVec2  m_p1 = { 0.0f, 0.0f };
		mvVec2  m_p2 = { 0.0f, 0.0f };
		mvColor m_color = {1.0f, 1.0f, 1.0f, 1.0f};
		float   m_thickness = 1.0f;

	};

}
