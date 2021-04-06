#include <utility>
#include "mvLineSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvLineSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
			{mvPythonDataType::Bool, "contribute_to_bounds", "", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvLineSeries::mvLineSeries(const std::string& name, const std::vector<std::vector<float>>& default_value)
		: mvSeriesBase(name, default_value)
	{
	}

	void mvLineSeries::draw(ImDrawList* drawlist, float x, float y)
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

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];

		ImPlot::PlotLine(m_label.c_str(), xptr->data(), yptr->data(), xptr->size());

	}

	void mvLineSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

	}

	void mvLineSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	PyObject* mvLineSeries::add_line_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();

		PyObject* x = PyTuple_New(2);
		PyTuple_SetItem(x, 0, PyLong_FromLong(0));
		PyTuple_SetItem(x, 1, PyLong_FromLong(1));

		PyObject* y = PyTuple_New(2);
		PyTuple_SetItem(y, 0, PyLong_FromLong(0));
		PyTuple_SetItem(y, 1, PyLong_FromLong(1));


		const char* label = "";
		const char* source = "";
		const char* parent = "";
		const char* before = "";
		int show = true;
		int axis = 0;
		int contribute_to_bounds = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__,
			&name, &x, &y, &label, &source, &parent, &before, &show, &axis, &contribute_to_bounds))
			return GetPyNone();

		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);

		auto item = CreateRef<mvLineSeries>(name, std::vector<std::vector<float>>{xs, ys});

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}