#pragma once

#include "mvTypeBases.h"
#include <vector>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTableColumn
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvTableColumn);
	class mvTableColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvTableColumn, add_table_column)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvTableColumn(const std::string& name, float init_width_or_weight);
		~mvTableColumn();

		void draw() override;
		bool isParentCompatible(mvAppItemType type) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTableColumnFlags m_flags = 0;
		float m_init_width_or_weight = 0.0f;

	};

}
