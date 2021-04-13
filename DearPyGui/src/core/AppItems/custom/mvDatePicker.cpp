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
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Dict>("default_value", mvArgType::KEYWORD_ARG, "{'month_day': 14, 'year':20, 'month':5}");
		parser.addArg<mvPyDataType::Integer>("level", mvArgType::KEYWORD_ARG, "0", "0-day, 1-month, 2-year");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDatePicker::mvDatePicker(const std::string& name)
		: mvTimePtrBase(name)
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
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
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