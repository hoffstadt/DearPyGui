#include "mvTimePicker.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTimePicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a time picker.", { "Widgets" });
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

		parser.addArg<mvPyDataType::Dict>("default_value", mvArgType::KEYWORD_ARG, "{'hour': 14, 'min': 32, 'sec': 23}");
		parser.addArg<mvPyDataType::Bool>("hour24", mvArgType::KEYWORD_ARG, "False", "Show 24 hour clock instead of 12 hour.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTimePicker::mvTimePicker(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvTimePicker::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		ImPlot::GetStyle().Use24HourClock = _hour24;

		if (ImPlot::ShowTimePicker(_internalLabel.c_str(), _imvalue.get()))
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

	void mvTimePicker::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "hour24")) _hour24 = ToBool(item);
	}

	void mvTimePicker::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "hour24", mvPyObject(ToPyBool(_hour24)));
	}

	PyObject* mvTimePicker::getPyValue()
	{
		return ToPyTime(*_value);
	}

	void mvTimePicker::setPyValue(PyObject* value)
	{

		if (value)
			*_value = ToTime(value);
		else
			_value = {};

		*_imvalue = ImPlot::MkGmtTime(_value.get());
	}
}