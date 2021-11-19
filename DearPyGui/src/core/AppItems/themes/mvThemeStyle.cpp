#include "mvThemeStyle.h"
#include "mvContext.h"
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
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		args.push_back({ mvPyDataType::Long, "target", mvArgType::POSITIONAL_ARG, "0" });
		args.push_back({ mvPyDataType::Float, "x", mvArgType::POSITIONAL_ARG, "1.0" });
		args.push_back({ mvPyDataType::Float, "y", mvArgType::POSITIONAL_ARG, "-1.0" });
		args.push_back({ mvPyDataType::Integer, "category", mvArgType::KEYWORD_ARG, "0", "Options include mvThemeCat_Core, mvThemeCat_Plots, mvThemeCat_Nodes." });

		mvPythonParserSetup setup;
		setup.about = "Adds a theme style.";
		setup.category = { "Themes" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

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


	PyObject* mvThemeStyle::getPyValue()
	{
		return ToPyFloatList(_value->data(), 2);
	}

	void mvThemeStyle::setPyValue(PyObject* value)
	{
		std::vector<float> temp = ToFloatVect(value);
		while (temp.size() < 4)
			temp.push_back(0.0f);
		std::array<float, 4> temp_array;
		for (size_t i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (_value)
			*_value = temp_array;
		else
			_value = std::make_shared<std::array<float, 4>>(temp_array);

		if (_parentPtr)
			_parentPtr->_triggerAlternativeAction = true;
	}

	void mvThemeStyle::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
	}

	void mvThemeStyle::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_libType == mvLibType::MV_IMGUI)
		{
			const mvGuiStyleVarInfo* var_info = GetStyleVarInfo(_targetStyle);
			if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
				ImGui::PushStyleVar(_targetStyle, { (*_value)[0], (*_value)[1] });
			else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
				ImGui::PushStyleVar(_targetStyle, (*_value)[0]);
		}
		else if (_libType == mvLibType::MV_IMPLOT)
		{
			const mvPlotStyleVarInfo* var_info = GetPlotStyleVarInfo(_targetStyle);
			if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
				ImPlot::PushStyleVar(_targetStyle, (*_value)[0]);
			else if (var_info->Type == ImGuiDataType_S32 && var_info->Count == 1)
				ImPlot::PushStyleVar(_targetStyle, (int)(*_value)[0]);
			else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
				ImPlot::PushStyleVar(_targetStyle, { (*_value)[0], (*_value)[1] });
		}
		else if (_libType == mvLibType::MV_IMNODES)
			imnodes::PushStyleVar((imnodes::StyleVar)_targetStyle, (*_value)[0]);
	}

	void mvThemeStyle::customAction(void* data)
	{
		if (_libType == mvLibType::MV_IMGUI)
			ImGui::PopStyleVar();
		else if (_libType == mvLibType::MV_IMPLOT)
			ImPlot::PopStyleVar();
		else if (_libType == mvLibType::MV_IMNODES)
			imnodes::PopStyleVar();
	}

	void mvThemeStyle::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				_targetStyle = ToInt(item);
				break;
			}

			case 1:
				(*_value)[0] = ToFloat(item);
				break;

			case 2:
				(*_value)[1] = ToFloat(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "category")) _libType = (mvLibType)ToInt(item);

		if (_libType == mvLibType::MV_IMGUI)
		{
			if (_targetStyle >= ImGuiStyleVar_COUNT || _targetStyle < 0)
			{
				_state.ok = false;
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}

		else if (_libType == mvLibType::MV_IMPLOT)
		{
			if (_targetStyle >= ImPlotStyleVar_COUNT || _targetStyle < 0)
			{
				_state.ok = false;
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}

		else if (_libType == mvLibType::MV_IMNODES)
		{
			if (_targetStyle >= 14 || _targetStyle < 0)
			{
				_state.ok = false;
				mvThrowPythonError(mvErrorCode::mvNone, "");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
			}
		}
	}

	void mvThemeStyle::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvThemeStyle*>(item);
		if (_source != 0) _value = titem->_value;
		_targetStyle = titem->_targetStyle;
		_libType = titem->_libType;
	}

}
