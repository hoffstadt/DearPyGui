#include "mvSimplePlot.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvSimplePlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD, "()");
		parser.addArg<mvPyDataType::String>("overlay", mvArgType::KEYWORD, "''", "overlays text (similar to a plot title)");

		parser.addArg<mvPyDataType::Bool>("histogram", mvArgType::KEYWORD, "False");
		
		parser.addArg<mvPyDataType::Float>("min_scale", mvArgType::KEYWORD, "0.0");
		parser.addArg<mvPyDataType::Float>("max_scale", mvArgType::KEYWORD, "0.0");

		parser.finalize();

		parsers->insert({ s_command, parser });
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

	void mvSimplePlot::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "minscale")) m_min = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "maxscale")) m_max = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "histogram")) m_histogram = ToBool(item);
	}

	void mvSimplePlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
		PyDict_SetItemString(dict, "minscale", ToPyFloat(m_min));
		PyDict_SetItemString(dict, "maxscale", ToPyFloat(m_max));
		PyDict_SetItemString(dict, "histogram", ToPyBool(m_histogram));
	}

}