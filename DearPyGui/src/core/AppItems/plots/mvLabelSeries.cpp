#include <utility>
#include "mvLabelSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvLabelSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
			{mvPythonDataType::Bool, "vertical", "", "False"},
			{mvPythonDataType::Integer, "x_offset", "", "0.0"},
			{mvPythonDataType::Integer, "y_offset", "", "0.0"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
			{mvPythonDataType::Bool, "contribute_to_bounds", "", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvLabelSeries::mvLabelSeries(const std::string& name, const std::vector<std::vector<float>>& default_value)
		: mvSeriesBase(name, default_value)
	{
	}

	void mvLabelSeries::draw(ImDrawList* drawlist, float x, float y)
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

		ImPlot::PlotText(m_label.c_str(), (*xptr)[0], (*yptr)[0], m_vertical,
			ImVec2((float)m_xoffset, (float)m_yoffset));

	}

	void mvLabelSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x_offset")) m_xoffset = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y_offset")) m_yoffset = ToInt(item);

	}

	void mvLabelSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	PyObject* mvLabelSeries::add_text_point(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();

		float x = 0.0f;
		float y = 0.0f;
		int vertical = false;
		int x_offset = 0;
		int y_offset = 0;
		const char* label = "";
		const char* source = "";
		const char* parent = "";
		const char* before = "";
		int show = true;
		int axis = 0;
		int contribute_to_bounds = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__,
			&name, &x, &y, &vertical, &x_offset, &y_offset, &label, &source, &parent, &before, &show, &axis,
			&contribute_to_bounds))
			return GetPyNone();

		std::vector<float> ax = { x };
		std::vector<float> ay = { y };

		auto item = CreateRef<mvLabelSeries>(name, std::vector<std::vector<float>>{ax, ay});

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}