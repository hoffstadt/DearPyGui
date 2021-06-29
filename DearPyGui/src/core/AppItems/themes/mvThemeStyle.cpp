#include "mvThemeStyle.h"
#include "mvApp.h"
#include <array>
#include <implot.h>
#include <imnodes.h>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvThemeStyle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Themes" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		parser.addArg<mvPyDataType::Long>("target", mvArgType::POSITIONAL_ARG, "0");
		parser.addArg<mvPyDataType::Float>("x", mvArgType::POSITIONAL_ARG, "1.0");
		parser.addArg<mvPyDataType::Float>("y", mvArgType::POSITIONAL_ARG, "-1.0");
		parser.addArg<mvPyDataType::Integer>("category", mvArgType::KEYWORD_ARG, "0", "Options include mvThemeCat_Core, mvThemeCat_Plots, mvThemeCat_Nodes.");

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

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: mvTheme", this);

		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvThemeStyle::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_libType == mvLibType::MV_IMGUI)
		{
			if(!m_twoComponent)
				ImGui::PushStyleVar(m_targetStyle, m_x);
			else
				ImGui::PushStyleVar(m_targetStyle, { m_x, m_y });
		}
		else if (m_libType == mvLibType::MV_IMPLOT)
		{
			if (!m_twoComponent)
			{
				if(m_targetStyle == ImPlotStyleVar_Marker)
					ImPlot::PushStyleVar(m_targetStyle, (int)m_x);
				else
					ImPlot::PushStyleVar(m_targetStyle, m_x);
			}
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
			{
				m_targetStyle = ToInt(item);
				if (m_libType == mvLibType::MV_IMGUI)
				{
					switch (m_targetStyle)
					{
					case(ImGuiStyleVar_WindowPadding): m_twoComponent = true; break;
					case(ImGuiStyleVar_WindowMinSize): m_twoComponent = true; break;
					case(ImGuiStyleVar_WindowTitleAlign): m_twoComponent = true; break;
					case(ImGuiStyleVar_FramePadding): m_twoComponent = true; break;
					case(ImGuiStyleVar_ItemSpacing): m_twoComponent = true; break;
					case(ImGuiStyleVar_ItemInnerSpacing): m_twoComponent = true; break;
					case(ImGuiStyleVar_CellPadding): m_twoComponent = true; break;
					case(ImGuiStyleVar_ButtonTextAlign): m_twoComponent = true; break;
					case(ImGuiStyleVar_SelectableTextAlign): m_twoComponent = true; break;
					default: break;
					}

					if (m_targetStyle >= ImGuiStyleVar_COUNT || m_targetStyle < 0)
					{
						m_state.setOk(false);
						mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
						MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
					}
				}

				else if (m_libType == mvLibType::MV_IMPLOT)
				{
					switch (m_targetStyle)
					{
					case(ImPlotStyleVar_MajorTickLen): m_twoComponent = true; break;
					case(ImPlotStyleVar_MinorTickLen): m_twoComponent = true; break;
					case(ImPlotStyleVar_MajorTickSize): m_twoComponent = true; break;
					case(ImPlotStyleVar_MinorTickSize): m_twoComponent = true; break;
					case(ImPlotStyleVar_MajorGridSize): m_twoComponent = true; break;
					case(ImPlotStyleVar_MinorGridSize): m_twoComponent = true; break;
					case(ImPlotStyleVar_PlotPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_LabelPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_LegendPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_LegendInnerPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_LegendSpacing): m_twoComponent = true; break;
					case(ImPlotStyleVar_MousePosPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_AnnotationPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_FitPadding): m_twoComponent = true; break;
					case(ImPlotStyleVar_PlotDefaultSize): m_twoComponent = true; break;
					case(ImPlotStyleVar_PlotMinSize): m_twoComponent = true; break;
					default: 
						m_twoComponent = false;
						break;
					}

					if (m_targetStyle >= ImPlotStyleVar_COUNT || m_targetStyle < 0)
					{
						m_state.setOk(false);
						mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
						MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
					}
				}

				else if (m_libType == mvLibType::MV_IMNODES)
				{
					if (m_targetStyle >= 14 || m_targetStyle < 0)
					{
						m_state.setOk(false);
						mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
						MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
					}
				}
				break;
			}

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

		if (m_libType == mvLibType::MV_IMGUI)
		{
			switch (m_targetStyle)
			{
			case(ImGuiStyleVar_WindowPadding): m_twoComponent = true; break;
			case(ImGuiStyleVar_WindowMinSize): m_twoComponent = true; break;
			case(ImGuiStyleVar_WindowTitleAlign): m_twoComponent = true; break;
			case(ImGuiStyleVar_FramePadding): m_twoComponent = true; break;
			case(ImGuiStyleVar_ItemSpacing): m_twoComponent = true; break;
			case(ImGuiStyleVar_ItemInnerSpacing): m_twoComponent = true; break;
			case(ImGuiStyleVar_CellPadding): m_twoComponent = true; break;
			case(ImGuiStyleVar_ButtonTextAlign): m_twoComponent = true; break;
			case(ImGuiStyleVar_SelectableTextAlign): m_twoComponent = true; break;
			default: break;
			}

			if (m_targetStyle >= ImGuiStyleVar_COUNT || m_targetStyle < 0)
			{
				m_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}

		else if (m_libType == mvLibType::MV_IMPLOT)
		{
			switch (m_targetStyle)
			{
			case(ImPlotStyleVar_MajorTickLen): m_twoComponent = true; break;
			case(ImPlotStyleVar_MinorTickLen): m_twoComponent = true; break;
			case(ImPlotStyleVar_MajorTickSize): m_twoComponent = true; break;
			case(ImPlotStyleVar_MinorTickSize): m_twoComponent = true; break;
			case(ImPlotStyleVar_MajorGridSize): m_twoComponent = true; break;
			case(ImPlotStyleVar_MinorGridSize): m_twoComponent = true; break;
			case(ImPlotStyleVar_PlotPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_LabelPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_LegendPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_LegendInnerPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_LegendSpacing): m_twoComponent = true; break;
			case(ImPlotStyleVar_MousePosPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_AnnotationPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_FitPadding): m_twoComponent = true; break;
			case(ImPlotStyleVar_PlotDefaultSize): m_twoComponent = true; break;
			case(ImPlotStyleVar_PlotMinSize): m_twoComponent = true; break;
			default:
				m_twoComponent = false;
				break;
			}

			if (m_targetStyle >= ImPlotStyleVar_COUNT || m_targetStyle < 0)
			{
				m_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}

		else if (m_libType == mvLibType::MV_IMNODES)
		{
			if (m_targetStyle >= 14 || m_targetStyle < 0)
			{
				m_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}
	}

}
