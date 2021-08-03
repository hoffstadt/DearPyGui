#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTimePicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Time, 1);
	class mvTimePicker : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTimePicker, add_time_picker)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTimePicker(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y)override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
		mvRef<tm>         _value = CreateRef<tm>();
		mvRef<ImPlotTime> _imvalue = CreateRef<ImPlotTime>();
		bool              _hour24 = false;

	};

}