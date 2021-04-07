#include "mvSimplePlot.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvSimplePlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "value", "Tuple of float values", "()"},
			{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)", "''"},
			{mvPythonDataType::Float, "minscale", "used if autoscale is false", "0.0"},
			{mvPythonDataType::Float, "maxscale", "used if autoscale is false", "0.0"},
			{mvPythonDataType::Bool, "histogram", "create a histogram", "False"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::String, "source","", "''"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "A simple plot for visualization of a set of values", "None", "Adding Widgets") });
	}

	mvSimplePlot::mvSimplePlot(const std::string& name)
		: mvFloatVectPtrBase(name)
	{
	}

	void mvSimplePlot::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::PushID(this);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_histogram)
			ImGui::PlotHistogram(m_label.c_str(), m_value->data(), (int)m_value->size(), 0, m_overlay.c_str(),
				m_min, m_max, ImVec2((float)m_width, (float)m_height));
		else
			ImGui::PlotLines(m_label.c_str(), m_value->data(), (int)m_value->size(), 0, m_overlay.c_str(),
				m_min, m_max, ImVec2((float)m_width, (float)m_height));

		ImGui::PopID();
	}

	void mvSimplePlot::setValue(const std::vector<float>& value)
	{
		*m_value = value;

		if (!value.empty())
		{
			m_max = m_value->data()[0];
			m_min = m_value->data()[0];

			for (auto& item : *m_value)
			{
				if (item > m_max)
					m_max = item;
				if (item < m_min)
					m_min = item;
			}
		}
	}

	void mvSimplePlot::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "minscale")) m_min = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "maxscale")) m_max = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "histogram")) m_histogram = ToBool(item);
	}

	void mvSimplePlot::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
		PyDict_SetItemString(dict, "minscale", ToPyFloat(m_min));
		PyDict_SetItemString(dict, "maxscale", ToPyFloat(m_max));
		PyDict_SetItemString(dict, "histogram", ToPyBool(m_histogram));
	}

}