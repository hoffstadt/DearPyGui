#include <utility>
#include "mvErrorSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvErrorSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::FloatList, "negative"},
			{mvPythonDataType::FloatList, "positive"},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
			{mvPythonDataType::Bool, "contribute_to_bounds", "", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvErrorSeries::mvErrorSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvErrorSeries::draw(ImDrawList* drawlist, float x, float y)
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
		static const std::vector<float>* yptr;
		static const std::vector<float>* zptr;
		static const std::vector<float>* wptr;

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];
		zptr = &(*m_value.get())[2];
		wptr = &(*m_value.get())[3];

		if (m_horizontal)
			ImPlot::PlotErrorBarsH(m_name.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());
		else
			ImPlot::PlotErrorBars(m_name.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());

	}

	void mvErrorSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal= ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { valueChanged = true; (*m_value)[1] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "negative")) { valueChanged = true; (*m_value)[2] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "positive")) { valueChanged = true; (*m_value)[3] = ToFloatVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvErrorSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}