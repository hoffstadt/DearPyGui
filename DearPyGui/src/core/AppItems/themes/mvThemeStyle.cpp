#include "mvThemeStyle.h"
#include "mvApp.h"
#include <array>
#include <implot.h>
#include <imgui_internal.h>
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

	struct mvGuiStyleVarInfo
	{
		ImGuiDataType   Type;
		ImU32           Count;
		ImU32           Offset;
		void* GetVarPtr(ImGuiStyle* style) const { return (void*)((unsigned char*)style + Offset); }
	};

	static const mvGuiStyleVarInfo GStyleVarInfo[] =
	{
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, Alpha) },               // ImGuiStyleVar_Alpha
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowPadding) },       // ImGuiStyleVar_WindowPadding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowRounding) },      // ImGuiStyleVar_WindowRounding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowBorderSize) },    // ImGuiStyleVar_WindowBorderSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowMinSize) },       // ImGuiStyleVar_WindowMinSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowTitleAlign) },    // ImGuiStyleVar_WindowTitleAlign
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildRounding) },       // ImGuiStyleVar_ChildRounding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildBorderSize) },     // ImGuiStyleVar_ChildBorderSize
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, PopupRounding) },       // ImGuiStyleVar_PopupRounding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, PopupBorderSize) },     // ImGuiStyleVar_PopupBorderSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, FramePadding) },        // ImGuiStyleVar_FramePadding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameRounding) },       // ImGuiStyleVar_FrameRounding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameBorderSize) },     // ImGuiStyleVar_FrameBorderSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemSpacing) },         // ImGuiStyleVar_ItemSpacing
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemInnerSpacing) },    // ImGuiStyleVar_ItemInnerSpacing
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, IndentSpacing) },       // ImGuiStyleVar_IndentSpacing
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, CellPadding) },         // ImGuiStyleVar_CellPadding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ScrollbarSize) },       // ImGuiStyleVar_ScrollbarSize
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ScrollbarRounding) },   // ImGuiStyleVar_ScrollbarRounding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabMinSize) },         // ImGuiStyleVar_GrabMinSize
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabRounding) },        // ImGuiStyleVar_GrabRounding
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, TabRounding) },         // ImGuiStyleVar_TabRounding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ButtonTextAlign) },     // ImGuiStyleVar_ButtonTextAlign
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, SelectableTextAlign) }, // ImGuiStyleVar_SelectableTextAlign
	};

	static const mvGuiStyleVarInfo* GetStyleVarInfo(ImGuiStyleVar idx)
	{
		IM_ASSERT(idx >= 0 && idx < ImGuiStyleVar_COUNT);
		IM_ASSERT(IM_ARRAYSIZE(GStyleVarInfo) == ImGuiStyleVar_COUNT);
		return &GStyleVarInfo[idx];
	}

	struct mvPlotStyleVarInfo {
		ImGuiDataType   Type;
		ImU32           Count;
		ImU32           Offset;
		void* GetVarPtr(ImPlotStyle* style) const { return (void*)((unsigned char*)style + Offset); }
	};

	static const mvPlotStyleVarInfo GPlotStyleVarInfo[] =
	{
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, LineWeight)         }, // ImPlotStyleVar_LineWeight
		{ ImGuiDataType_S32,   1, (ImU32)IM_OFFSETOF(ImPlotStyle, Marker)             }, // ImPlotStyleVar_Marker
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, MarkerSize)         }, // ImPlotStyleVar_MarkerSize
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, MarkerWeight)       }, // ImPlotStyleVar_MarkerWeight
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, FillAlpha)          }, // ImPlotStyleVar_FillAlpha
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, ErrorBarSize)       }, // ImPlotStyleVar_ErrorBarSize
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, ErrorBarWeight)     }, // ImPlotStyleVar_ErrorBarWeight
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, DigitalBitHeight)   }, // ImPlotStyleVar_DigitalBitHeight
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, DigitalBitGap)      }, // ImPlotStyleVar_DigitalBitGap

		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, PlotBorderSize)     }, // ImPlotStyleVar_PlotBorderSize
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, MinorAlpha)         }, // ImPlotStyleVar_MinorAlpha
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MajorTickLen)       }, // ImPlotStyleVar_MajorTickLen
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MinorTickLen)       }, // ImPlotStyleVar_MinorTickLen
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MajorTickSize)      }, // ImPlotStyleVar_MajorTickSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MinorTickSize)      }, // ImPlotStyleVar_MinorTickSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MajorGridSize)      }, // ImPlotStyleVar_MajorGridSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MinorGridSize)      }, // ImPlotStyleVar_MinorGridSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, PlotPadding)        }, // ImPlotStyleVar_PlotPadding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, LabelPadding)       }, // ImPlotStyleVar_LabelPaddine
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, LegendPadding)      }, // ImPlotStyleVar_LegendPadding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, LegendInnerPadding) }, // ImPlotStyleVar_LegendInnerPadding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, LegendSpacing)      }, // ImPlotStyleVar_LegendSpacing

		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, MousePosPadding)    }, // ImPlotStyleVar_MousePosPadding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, AnnotationPadding)  }, // ImPlotStyleVar_AnnotationPadding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, FitPadding)         }, // ImPlotStyleVar_FitPadding
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, PlotDefaultSize)    }, // ImPlotStyleVar_PlotDefaultSize
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImPlotStyle, PlotMinSize)        }  // ImPlotStyleVar_PlotMinSize
	};

	static const mvPlotStyleVarInfo* GetPlotStyleVarInfo(ImPlotStyleVar idx) {
		IM_ASSERT(idx >= 0 && idx < ImPlotStyleVar_COUNT);
		IM_ASSERT(IM_ARRAYSIZE(GPlotStyleVarInfo) == ImPlotStyleVar_COUNT);
		return &GPlotStyleVarInfo[idx];
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
			const mvGuiStyleVarInfo* var_info = GetStyleVarInfo(m_targetStyle);
			if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
				ImGui::PushStyleVar(m_targetStyle, { m_x, m_y });
			else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
				ImGui::PushStyleVar(m_targetStyle, m_x);
		}
		else if (m_libType == mvLibType::MV_IMPLOT)
		{
			const mvPlotStyleVarInfo* var_info = GetPlotStyleVarInfo(m_targetStyle);
			if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
				ImPlot::PushStyleVar(m_targetStyle, m_x);
			else if (var_info->Type == ImGuiDataType_S32 && var_info->Count == 1)
				ImPlot::PushStyleVar(m_targetStyle, (int)m_x);
			else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
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

	void mvThemeStyle::alternativeCustomAction()
	{
		if (m_libType == mvLibType::MV_IMGUI)
		{

			const mvGuiStyleVarInfo* var_info = GetStyleVarInfo(m_targetStyle);
			if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
			{
				ImGuiStyle& style = ImGui::GetStyle();
				ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&style);
				*pvar = ImVec2(m_x, m_y);
			}

			else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
			{
				ImGuiStyle& style = ImGui::GetStyle();
				float* pvar = (float*)var_info->GetVarPtr(&style);
				*pvar = m_x;
			}

		}
		else if (m_libType == mvLibType::MV_IMPLOT)
		{
			const mvPlotStyleVarInfo* var_info = GetPlotStyleVarInfo(m_targetStyle);
			ImPlotStyle& plotstyle = ImPlot::GetStyle();
			if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1) 
			{
				float* pvar = (float*)var_info->GetVarPtr(&plotstyle);
				*pvar = m_x;
			}
			else if (var_info->Type == ImGuiDataType_S32 && var_info->Count == 1) {
				int* pvar = (int*)var_info->GetVarPtr(&plotstyle);
				*pvar = (int)m_x;
			}
			else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
			{
				ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&plotstyle);
				*pvar = ImVec2(m_x, m_y);
			}
		}
		else if (m_libType == mvLibType::MV_IMNODES)
		{
			switch (m_targetStyle)
			{
			case imnodes::StyleVar::StyleVar_GridSpacing:
				imnodes::GetStyle().grid_spacing = m_x;
				break;
			case imnodes::StyleVar::StyleVar_NodeCornerRounding:
				imnodes::GetStyle().node_corner_rounding = m_x;
				break;
			case imnodes::StyleVar::StyleVar_NodePaddingHorizontal:
				imnodes::GetStyle().node_padding_horizontal = m_x;
				break;
			case imnodes::StyleVar::StyleVar_NodePaddingVertical:
				imnodes::GetStyle().node_padding_vertical = m_x;
				break;
			case imnodes::StyleVar::StyleVar_NodeBorderThickness:
				imnodes::GetStyle().node_border_thickness = m_x;
				break;
			case imnodes::StyleVar::StyleVar_LinkThickness:
				imnodes::GetStyle().link_thickness = m_x;
				break;
			case imnodes::StyleVar::StyleVar_LinkLineSegmentsPerLength:
				imnodes::GetStyle().link_line_segments_per_length = m_x;
				break;
			case imnodes::StyleVar::StyleVar_LinkHoverDistance:
				imnodes::GetStyle().link_hover_distance = m_x;
				break;
			case imnodes::StyleVar::StyleVar_PinCircleRadius:
				imnodes::GetStyle().pin_circle_radius = m_x;
				break;
			case imnodes::StyleVar::StyleVar_PinQuadSideLength:
				imnodes::GetStyle().pin_quad_side_length = m_x;
				break;
			case imnodes::StyleVar::StyleVar_PinTriangleSideLength:
				imnodes::GetStyle().pin_triangle_side_length = m_x;
				break;
			case imnodes::StyleVar::StyleVar_PinLineThickness:
				imnodes::GetStyle().pin_line_thickness = m_x;
				break;
			case imnodes::StyleVar::StyleVar_PinHoverRadius:
				imnodes::GetStyle().pin_hover_radius = m_x;
				break;
			case imnodes::StyleVar::StyleVar_PinOffset:
				imnodes::GetStyle().pin_offset = m_x;
				break;
			default:
				assert(!"Invalid StyleVar value!");
			}
		}
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
					if (m_targetStyle >= ImGuiStyleVar_COUNT || m_targetStyle < 0)
					{
						m_state.setOk(false);
						mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
						MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
					}
				}

				else if (m_libType == mvLibType::MV_IMPLOT)
				{

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
			if (m_targetStyle >= ImGuiStyleVar_COUNT || m_targetStyle < 0)
			{
				m_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}

		else if (m_libType == mvLibType::MV_IMPLOT)
		{
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
