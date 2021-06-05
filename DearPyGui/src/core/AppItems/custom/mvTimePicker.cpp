#include "mvTimePicker.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTimePicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Dict>("default_value", mvArgType::KEYWORD_ARG, "{'hour': 14, 'min': 32, 'sec': 23}");
		parser.addArg<mvPyDataType::Bool>("hour24", mvArgType::KEYWORD_ARG, "False", "show 24 hour clock");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTimePicker::mvTimePicker(const std::string& name)
		: 
		mvTimePtrBase(name)
	{
	}

	void mvTimePicker::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		ImPlot::GetStyle().Use24HourClock = m_hour24;

		if (ImPlot::ShowTimePicker(m_name.c_str(), m_imvalue.get()))
		{
			ImPlot::GetGmtTime(*m_imvalue, m_value.get());
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr, m_user_data);
		}

	}

	void mvTimePicker::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "hour24")) m_hour24 = ToBool(item);
	}

	void mvTimePicker::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "hour24", ToPyBool(m_hour24));
	}

}