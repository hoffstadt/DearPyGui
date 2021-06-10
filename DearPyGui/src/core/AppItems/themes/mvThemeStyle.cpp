#include "mvThemeStyle.h"
#include "mvApp.h"
#include <array>
#include <implot.h>
#include <imnodes.h>
#include "mvItemRegistry.h"
//#include "mvImGuiThemeScope.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvThemeStyle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_LABEL)
		);

		parser.addArg<mvPyDataType::Integer>("target", mvArgType::POSITIONAL_ARG, "0");
		parser.addArg<mvPyDataType::Float>("x", mvArgType::POSITIONAL_ARG, "1.0");
		parser.addArg<mvPyDataType::Float>("y", mvArgType::POSITIONAL_ARG, "-1.0");
		parser.addArg<mvPyDataType::Integer>("category", mvArgType::KEYWORD_ARG, "0");

		parser.finalize();

		parsers->insert({ s_command, parser });

	}

	mvThemeStyle::mvThemeStyle(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvThemeStyle::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTheme) return true;
		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvThemeStyle::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_libType == mvLibType::MV_IMGUI)
		{
			if(m_y < 0.0f)
				ImGui::PushStyleVar(m_targetStyle, m_x);
			else
				ImGui::PushStyleVar(m_targetStyle, { m_x, m_y });
		}
		else if (m_libType == mvLibType::MV_IMPLOT)
		{
			if (m_y < 0.0f)
				ImPlot::PushStyleVar(m_targetStyle, m_x);
			else
				ImPlot::PushStyleVar(m_targetStyle, { m_x, m_y });
		}
		else if (m_libType == mvLibType::MV_IMNODES)
			imnodes::PushStyleVar((imnodes::StyleVar)m_targetStyle, m_x);
	}

	void mvThemeStyle::customAction()
	{
		if (m_libType == mvLibType::MV_IMGUI)
			ImGui::PopStyleVar();
		else if (m_libType == mvLibType::MV_IMPLOT)
			ImPlot::PopStyleVar();
		else if (m_libType == mvLibType::MV_IMNODES)
			imnodes::PopStyleVar();
	}

	void mvThemeStyle::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_targetStyle = ToInt(item);
				break;

			case 1:
				m_x = ToFloat(item);
				break;

			case 2:
				m_y = ToFloat(item);
				break;

			default:
				break;
			}
		}
	}

	void mvThemeStyle::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "category")) m_libType = (mvLibType)ToInt(item);
	}

}
