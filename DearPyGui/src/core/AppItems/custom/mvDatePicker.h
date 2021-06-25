#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDatePicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Time, 1);
	class mvDatePicker : public mvTimePtrBase
	{
		enum class DatePickerLevel {
			mvDatePickerLevel_Day = 0L,
			mvDatePickerLevel_Month,
			mvDatePickerLevel_Year
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDatePicker, add_date_picker)

		MV_CREATE_CONSTANT(mvDatePickerLevel_Day, 0L);
		MV_CREATE_CONSTANT(mvDatePickerLevel_Month, 1L);
		MV_CREATE_CONSTANT(mvDatePickerLevel_Year, 2L);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvDatePickerLevel_Day),
			MV_ADD_CONSTANT(mvDatePickerLevel_Month),
			MV_ADD_CONSTANT(mvDatePickerLevel_Year)
		MV_END_CONSTANTS

	public:

		mvDatePicker(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int m_level = 0;

	};

}