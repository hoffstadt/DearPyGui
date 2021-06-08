#include "mvDatePicker.h"
#include "mvApp.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvDatePicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

		parser.addArg<mvPyDataType::Dict>("default_value", mvArgType::KEYWORD_ARG, "{'month_day': 14, 'year':20, 'month':5}");
		parser.addArg<mvPyDataType::Integer>("level", mvArgType::KEYWORD_ARG, "0", "0-day, 1-month, 2-year");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDatePicker::mvDatePicker(mvUUID uuid)
		: mvTimePtrBase(uuid)
	{
	}

	void mvDatePicker::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImPlot::ShowDatePicker(m_name.c_str(), &m_level, m_imvalue.get(), m_imvalue.get()))
		{
			ImPlot::GetGmtTime(*m_imvalue, m_value.get());
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr, m_user_data);
		}

	}

	void mvDatePicker::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "level")) m_level = ToInt(item);
	}

	void mvDatePicker::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "level", ToPyInt(m_level));
	}

}