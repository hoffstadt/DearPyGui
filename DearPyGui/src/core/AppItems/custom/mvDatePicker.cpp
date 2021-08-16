#include "mvDatePicker.h"
#include "mvApp.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDatePicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Creates a date picker.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Dict>("default_value", mvArgType::KEYWORD_ARG, "{'month_day': 14, 'year':20, 'month':5}");
		parser.addArg<mvPyDataType::Integer>("level", mvArgType::KEYWORD_ARG, "0", "Use avaliable constants. mvDatePickerLevel_Day, mvDatePickerLevel_Month, mvDatePickerLevel_Year");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDatePicker::mvDatePicker(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDatePicker::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDatePicker*>(item);
		if (_source != 0)
                { 
                    _value = titem->_value;
		    _imvalue = titem->_imvalue;
                }
		_level = titem->_level;
	}

	void mvDatePicker::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (ImPlot::ShowDatePicker(_internalLabel.c_str(), &_level, _imvalue.get(), _imvalue.get()))
		{
			ImPlot::GetGmtTime(*_imvalue, _value.get());
			{
				auto value = *_value;
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyTime(value), _user_data);
					});
			}
		}

	}

	void mvDatePicker::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "level")) {
			_level = ToUUID(item);
			if (_level > 2) _level = 0;
		}
	}

	void mvDatePicker::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "level", mvPyObject(ToPyUUID((long)_level)));
	}

	PyObject* mvDatePicker::getPyValue()
	{
		return ToPyTime(*_value);
	}

	void mvDatePicker::setPyValue(PyObject* value)
	{

		if (value)
			*_value = ToTime(value);
		else
			_value = {};

		*_imvalue = ImPlot::MkGmtTime(_value.get());
	}
}