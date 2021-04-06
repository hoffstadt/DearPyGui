#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvHeatSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::Integer, "rows", "", "1"},
			{mvPythonDataType::Integer, "cols", "", "1"},
			{mvPythonDataType::Double, "scale_min", "", "0.0"},
			{mvPythonDataType::Double, "scale_max", "", "1.0"},
			{mvPythonDataType::String, "format", "", "'%0.1f'"},
			{mvPythonDataType::FloatList, "bounds_min", "", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "bounds_max", "", "(1.0, 1.0)"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "contribute_to_bounds", "", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvHeatSeries::mvHeatSeries(const std::string& name, const std::vector<std::vector<float>>& default_value)
		: mvSeriesBase(name, default_value)
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

	}

	void mvHeatSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	PyObject* mvHeatSeries::add_heat_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();

		PyObject* x = PyTuple_New(2);
		PyTuple_SetItem(x, 0, PyLong_FromLong(0));
		PyTuple_SetItem(x, 1, PyLong_FromLong(1));

		int rows = 1;
		int cols = 1;
		double scale_min = 0.0;
		double scale_max = 1.0;

		const char* format = "%0.1f";
		PyObject* bounds_min = PyTuple_New(2);
		PyTuple_SetItem(bounds_min, 0, PyLong_FromLong(0));
		PyTuple_SetItem(bounds_min, 1, PyLong_FromLong(0));
		PyObject* bounds_max = PyTuple_New(2);
		PyTuple_SetItem(bounds_max, 0, PyLong_FromLong(1));
		PyTuple_SetItem(bounds_max, 1, PyLong_FromLong(1));

		const char* label = "";
		const char* source = "";
		const char* parent = "";
		const char* before = "";
		int show = true;
		int contribute_to_bounds = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__,
			&name, &x, &rows, &cols, &scale_min, &scale_max, &format, &bounds_min,
			&bounds_max, &label, &source, &parent, &before, &show, &contribute_to_bounds))
			return GetPyNone();

		auto mvalues = ToFloatVect(x);
		auto mbounds_min = ToVec2(bounds_min);
		auto mbounds_max = ToVec2(bounds_max);

		if (mvalues.size() == 0)
			return GetPyNone();

		if (mvalues.size() % cols != 0)
		{
			ThrowPythonException(std::string(name) + " series dimensions are wrong.");
			return GetPyNone();
		}

		if (rows * cols != mvalues.size())
		{
			ThrowPythonException(std::string(name) + " series dimensions do not match data sizes.");
			return GetPyNone();
		}

		auto item = CreateRef<mvHeatSeries>(name, std::vector<std::vector<float>>{mvalues});

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}