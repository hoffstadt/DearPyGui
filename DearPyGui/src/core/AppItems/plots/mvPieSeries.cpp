#include <utility>
#include "mvPieSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvPieSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "values"},
			{mvPythonDataType::StringList, "labels"},
			{mvPythonDataType::Double, "x"},
			{mvPythonDataType::Double, "y"},
			{mvPythonDataType::Double, "radius"},
			{mvPythonDataType::Bool, "normalize", "", "False"},
			{mvPythonDataType::Double, "angle", "", "90.0"},
			{mvPythonDataType::String, "format", "", "'%0.2f'"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvPieSeries::mvPieSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvPieSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		switch (m_axis)
		{
		case ImPlotYAxis_1:
			ImPlot::SetPlotYAxis(ImPlotYAxis_1);
			break;
		case ImPlotYAxis_2:
			ImPlot::SetPlotYAxis(ImPlotYAxis_2);
			break;
		case ImPlotYAxis_3:
			ImPlot::SetPlotYAxis(ImPlotYAxis_3);
			break;
		default:
			break;
		}

		static const std::vector<float>* xptr;

		xptr = &(*m_value.get())[0];

		ImPlot::PlotPieChart(m_clabels.data(), xptr->data(), (int)m_labels.size(),
			m_x, m_y, m_radius, m_normalize, m_format.c_str(), m_angle);

	}

	void mvPieSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x")) m_x = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y")) m_y = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "angle")) m_angle = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "normalize")) m_normalize = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "labels"))
		{
			m_labels = ToStringVect(item);
			m_clabels.clear();
			for (const auto& label : m_labels)
				m_clabels.push_back(label.c_str());
		}

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "values")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }
		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}


	}

	void mvPieSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}