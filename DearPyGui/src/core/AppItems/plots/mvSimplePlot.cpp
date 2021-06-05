#include "mvSimplePlot.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvSimplePlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD_ARG, "()");
		parser.addArg<mvPyDataType::String>("overlay", mvArgType::KEYWORD_ARG, "''", "overlays text (similar to a plot title)");

		parser.addArg<mvPyDataType::Bool>("histogram", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "True");
		
		parser.addArg<mvPyDataType::Float>("min_scale", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Float>("max_scale", mvArgType::KEYWORD_ARG, "0.0");

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

	void mvSimplePlot::setPyValue(PyObject* value)
	{
		*m_value = ToFloatVect(value);

		if (!m_autosize)
			return;
		if (!m_value->empty())
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

	void mvSimplePlot::setValue(const std::vector<float>& value)
	{
		*m_value = value;

		if (!m_autosize)
			return;
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
		if (PyObject* item = PyDict_GetItemString(dict, "autosize")) m_autosize = ToBool(item);
	}

	void mvSimplePlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
		PyDict_SetItemString(dict, "minscale", ToPyFloat(m_min));
		PyDict_SetItemString(dict, "maxscale", ToPyFloat(m_max));
		PyDict_SetItemString(dict, "histogram", ToPyBool(m_histogram));
		PyDict_SetItemString(dict, "autosize", ToPyBool(m_autosize));
	}

}