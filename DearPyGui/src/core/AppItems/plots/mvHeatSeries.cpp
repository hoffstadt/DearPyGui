#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvHeatSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("x");
		parser.addArg<mvPyDataType::Integer>("rows");
		parser.addArg<mvPyDataType::Integer>("cols");


		parser.addArg<mvPyDataType::Double>("scale_min", mvArgType::KEYWORD, "0.0");
		parser.addArg<mvPyDataType::Double>("scale_max", mvArgType::KEYWORD, "1.0");

		parser.addArg<mvPyDataType::FloatList>("bounds_min", mvArgType::KEYWORD, "(0.0, 0.0)");
		parser.addArg<mvPyDataType::FloatList>("bounds_max", mvArgType::KEYWORD, "(1.0, 1.0)");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD, "'%0.1f'");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHeatSeries::mvHeatSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvHeatSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		static const std::vector<float>* xptr;

		xptr = &(*m_value.get())[0];


		ImPlot::PlotHeatmap(m_name.c_str(), xptr->data(), m_rows, m_cols, m_scale_min, m_scale_max,
			m_format.c_str(), { m_bounds_min.x, m_bounds_min.y }, { m_bounds_max.x, m_bounds_max.y });

	}

	void mvHeatSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "rows")) m_rows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "cols")) m_cols = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_min")) m_bounds_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_max")) m_bounds_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_min")) m_scale_min = (double)ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_max")) m_scale_max = (double)ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvHeatSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}