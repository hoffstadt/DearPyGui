#include "mvThemes.h"
#include "mvPyUtils.h"
#include <implot.h>
#include <imgui_internal.h>

void
apply_local_theming(mvAppItem* item)
{
	if (item->config.enabled)
	{
		if (auto classTheme = DearPyGui::GetClassThemeComponent(item->type))
			classTheme->push_theme_items();
	}
	else
	{
		if (auto classTheme = DearPyGui::GetDisabledClassThemeComponent(item->type))
			classTheme->push_theme_items();
	}

	if (item->theme)
	{
		item->theme->setSpecificEnabled(item->config.enabled);
		item->theme->setSpecificType((int)item->type);
		item->theme->push_theme_components();
	}
}

void
cleanup_local_theming(mvAppItem* item)
{
	if (item->config.enabled)
	{
		if (auto classTheme = DearPyGui::GetClassThemeComponent(item->type))
			classTheme->pop_theme_items();
	}
	else
	{
		if (auto classTheme = DearPyGui::GetDisabledClassThemeComponent(item->type))
			classTheme->pop_theme_items();
	}

	if (item->theme)
	{
		item->theme->setSpecificEnabled(item->config.enabled);
		item->theme->setSpecificType((int)item->type);
		item->theme->pop_theme_components();
	}
}

mvTheme::mvTheme(mvUUID uuid)
	:
	mvAppItem(uuid)
{
	config.show = false;
}

void mvTheme::push_theme_components()
{
	for (auto& child : childslots[1])
	{
		auto comp = static_cast<mvThemeComponent*>(child.get());
		if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
		{
			if (_specificEnabled == comp->_specificEnabled)
			{
				comp->push_theme_items();
			}

		}
		if (comp->_specificType != _specificType)
		{
			if (_specificEnabled == comp->_specificEnabled)
			{
				comp->_oldComponent = *comp->_specificComponentPtr;
				*comp->_specificComponentPtr = *(std::shared_ptr<mvThemeComponent>*) & child;
			}
			else
			{
				comp->_oldComponent = *comp->_specificDisabledComponentPtr;
				*comp->_specificDisabledComponentPtr = *(std::shared_ptr<mvThemeComponent>*) & child;
			}
		}
	}
}

void mvTheme::pop_theme_components()
{

	for (auto& child : childslots[1])
	{
		auto comp = static_cast<mvThemeComponent*>(child.get());
		if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
		{
			if (_specificEnabled == comp->_specificEnabled)
			{
				comp->pop_theme_items();
			}
		}
		if (comp->_specificType != _specificType)
		{
			if (_specificEnabled == comp->_specificEnabled)
			{
				*comp->_specificComponentPtr = comp->_oldComponent;
			}
			else
			{
				*comp->_specificDisabledComponentPtr = comp->_oldComponent;
			}

		}
	}
}

mvThemeColor::mvThemeColor(mvUUID uuid)
	:
	mvAppItem(uuid)
{
}

void mvThemeColor::push_theme_color()
{
	ImVec4 color = ImVec4((*_value)[0], (*_value)[1], (*_value)[2], (*_value)[3]);

	if (_libType == mvLibType::MV_IMGUI)
	{
		ImGui::PushStyleColor(_targetColor, color);
	}
	else if (_libType == mvLibType::OT)
		ImPlot::PushStyleColor(_targetColor, color);
	else if (_libType == mvLibType::MV_IMNODES)
		ImNodes::PushColorStyle(_targetColor, ImGui::ColorConvertFloat4ToU32(color));
}

void mvThemeColor::pop_theme_color()
{
	if (_libType == mvLibType::MV_IMGUI)
		ImGui::PopStyleColor();
	else if (_libType == mvLibType::OT)
		ImPlot::PopStyleColor();
	else if (_libType == mvLibType::MV_IMNODES)
		ImNodes::PopColorStyle();
}

void mvThemeColor::handleSpecificPositionalArgs(PyObject* dict)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	for (int i = 0; i < PyTuple_Size(dict); i++)
	{
		PyObject* item = PyTuple_GetItem(dict, i);
		switch (i)
		{
		case 0:
			_targetColor = ToInt(item);
			break;

		case 1:
			setPyValue(item);
			break;

		default:
			break;
		}
	}
}

void mvThemeColor::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "category")) _libType = (mvLibType)ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "value")) setPyValue(item);

	if (_libType == mvLibType::MV_IMGUI)
	{

		if (_targetColor >= ImGuiCol_COUNT || _targetColor < 0)
		{
			state.ok = false;
			mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
			assert(false);
		}
	}

	else if (_libType == mvLibType::OT)
	{
		if (_targetColor >= ImPlotCol_COUNT || _targetColor < 0)
		{
			state.ok = false;
			mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
			assert(false);
		}
	}

	else if (_libType == mvLibType::MV_IMNODES)
	{
		if (_targetColor >= ImNodesCol_COUNT || _targetColor < 0)
		{
			state.ok = false;
			mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
			assert(false);
		}
	}
}

void mvThemeColor::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_target = ToPyInt(_targetColor);
	mvPyObject py_cat = ToPyInt((int)_libType);

	PyDict_SetItemString(dict, "target", py_target);
	PyDict_SetItemString(dict, "category", py_cat);
}

PyObject* mvThemeColor::getPyValue()
{
	// nasty hack
	int r = (int)(_value->data()[0] * 255.0f * 255.0f);
	int g = (int)(_value->data()[1] * 255.0f * 255.0f);
	int b = (int)(_value->data()[2] * 255.0f * 255.0f);
	int a = (int)(_value->data()[3] * 255.0f * 255.0f);

	auto color = mvColor(r, g, b, a);
	return ToPyColor(color);
}

void mvThemeColor::setPyValue(PyObject* value)
{
	mvColor color = ToColor(value);
	std::array<float, 4> temp_array;
	temp_array[0] = color.r;
	temp_array[1] = color.g;
	temp_array[2] = color.b;
	temp_array[3] = color.a;
	if (_value)
		*_value = temp_array;
	else
		_value = std::make_shared<std::array<float, 4>>(temp_array);

	if (info.parentPtr)
		info.parentPtr->info.triggerAlternativeAction = true;
}

void mvThemeColor::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
}

mvThemeComponent::mvThemeComponent(mvUUID uuid)
	:
	mvAppItem(uuid)
{
}

void mvThemeComponent::push_theme_items()
{

	for (auto& child : childslots[1])
	{
		if (child->type == mvAppItemType::mvThemeColor)
			((mvThemeColor*)child.get())->push_theme_color();
		else if (child->type == mvAppItemType::mvThemeStyle)
			((mvThemeStyle*)child.get())->push_theme_style();
	}
}

void mvThemeComponent::pop_theme_items()
{

	for (auto& child : childslots[1])
	{
		if (child->type == mvAppItemType::mvThemeColor)
			((mvThemeColor*)child.get())->pop_theme_color();
		else if (child->type == mvAppItemType::mvThemeStyle)
			((mvThemeStyle*)child.get())->pop_theme_style();
	}

}

void mvThemeComponent::handleSpecificPositionalArgs(PyObject* dict)
{
	static std::shared_ptr<mvThemeComponent> all_item_theme_component = nullptr;

	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	for (int i = 0; i < PyTuple_Size(dict); i++)
	{
		PyObject* item = PyTuple_GetItem(dict, i);
		switch (i)
		{
		case 0:
		{
			_specificType = ToInt(item);
			_specificComponentPtr = &DearPyGui::GetClassThemeComponent((mvAppItemType)_specificType);
			_specificDisabledComponentPtr = &DearPyGui::GetDisabledClassThemeComponent((mvAppItemType)_specificType);

			if (_specificType == (int)mvAppItemType::All)
			{
				_specificComponentPtr = &all_item_theme_component;
				_specificDisabledComponentPtr = &all_item_theme_component;
			}

			break;
		}
		default:
			break;
		}
	}
}

void mvThemeComponent::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "enabled_state")) _specificEnabled = ToBool(item);
}

void mvThemeComponent::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_target = ToPyInt(_specificType);
	mvPyObject py_cat = ToPyBool((int)_specificEnabled);

	PyDict_SetItemString(dict, "item_type", py_target);
	PyDict_SetItemString(dict, "enabled_state", py_cat);
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
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, Alpha) },               // ImGuiStyleVar_Alpha
    { ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, DisabledAlpha) },       // ImGuiStyleVar_DisabledAlpha
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, WindowPadding) },       // ImGuiStyleVar_WindowPadding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, WindowRounding) },      // ImGuiStyleVar_WindowRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, WindowBorderSize) },    // ImGuiStyleVar_WindowBorderSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, WindowMinSize) },       // ImGuiStyleVar_WindowMinSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, WindowTitleAlign) },    // ImGuiStyleVar_WindowTitleAlign
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, ChildRounding) },       // ImGuiStyleVar_ChildRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, ChildBorderSize) },     // ImGuiStyleVar_ChildBorderSize
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, PopupRounding) },       // ImGuiStyleVar_PopupRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, PopupBorderSize) },     // ImGuiStyleVar_PopupBorderSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, FramePadding) },        // ImGuiStyleVar_FramePadding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, FrameRounding) },       // ImGuiStyleVar_FrameRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, FrameBorderSize) },     // ImGuiStyleVar_FrameBorderSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, ItemSpacing) },         // ImGuiStyleVar_ItemSpacing
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, ItemInnerSpacing) },    // ImGuiStyleVar_ItemInnerSpacing
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, IndentSpacing) },       // ImGuiStyleVar_IndentSpacing
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, CellPadding) },         // ImGuiStyleVar_CellPadding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, ScrollbarSize) },       // ImGuiStyleVar_ScrollbarSize
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, ScrollbarRounding) },   // ImGuiStyleVar_ScrollbarRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, GrabMinSize) },         // ImGuiStyleVar_GrabMinSize
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, GrabRounding) },        // ImGuiStyleVar_GrabRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, TabRounding) },         // ImGuiStyleVar_TabRounding
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, TabBarBorderSize) },        // ImGuiStyleVar_TabBarBorderSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, ButtonTextAlign) },         // ImGuiStyleVar_ButtonTextAlign
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, SelectableTextAlign) },     // ImGuiStyleVar_SelectableTextAlign
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImGuiStyle, SeparatorTextBorderSize) }, // ImGuiStyleVar_SeparatorTextBorderSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, SeparatorTextAlign) },      // ImGuiStyleVar_SeparatorTextAlign
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImGuiStyle, SeparatorTextPadding) },    // ImGuiStyleVar_SeparatorTextPadding
};

static const mvGuiStyleVarInfo* GetStyleVarInfo(ImGuiStyleVar idx)
{
	IM_ASSERT(idx >= 0 && idx < ImGuiStyleVar_COUNT);
	IM_ASSERT(IM_ARRAYSIZE(GStyleVarInfo) == ImGuiStyleVar_COUNT);
	return &GStyleVarInfo[idx];
}

static const mvGuiStyleVarInfo GPlotStyleVarInfo[] =
{
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, LineWeight)         }, // ImPlotStyleVar_LineWeight
	{ ImGuiDataType_S32,   1, (ImU32)offsetof(ImPlotStyle, Marker)             }, // ImPlotStyleVar_Marker
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, MarkerSize)         }, // ImPlotStyleVar_MarkerSize
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, MarkerWeight)       }, // ImPlotStyleVar_MarkerWeight
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, FillAlpha)          }, // ImPlotStyleVar_FillAlpha
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, ErrorBarSize)       }, // ImPlotStyleVar_ErrorBarSize
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, ErrorBarWeight)     }, // ImPlotStyleVar_ErrorBarWeight
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, DigitalBitHeight)   }, // ImPlotStyleVar_DigitalBitHeight
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, DigitalBitGap)      }, // ImPlotStyleVar_DigitalBitGap

	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, PlotBorderSize)     }, // ImPlotStyleVar_PlotBorderSize
	{ ImGuiDataType_Float, 1, (ImU32)offsetof(ImPlotStyle, MinorAlpha)         }, // ImPlotStyleVar_MinorAlpha
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MajorTickLen)       }, // ImPlotStyleVar_MajorTickLen
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MinorTickLen)       }, // ImPlotStyleVar_MinorTickLen
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MajorTickSize)      }, // ImPlotStyleVar_MajorTickSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MinorTickSize)      }, // ImPlotStyleVar_MinorTickSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MajorGridSize)      }, // ImPlotStyleVar_MajorGridSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MinorGridSize)      }, // ImPlotStyleVar_MinorGridSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, PlotPadding)        }, // ImPlotStyleVar_PlotPadding
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, LabelPadding)       }, // ImPlotStyleVar_LabelPaddine
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, LegendPadding)      }, // ImPlotStyleVar_LegendPadding
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, LegendInnerPadding) }, // ImPlotStyleVar_LegendInnerPadding
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, LegendSpacing)      }, // ImPlotStyleVar_LegendSpacing

	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, MousePosPadding)    }, // ImPlotStyleVar_MousePosPadding
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, AnnotationPadding)  }, // ImPlotStyleVar_AnnotationPadding
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, FitPadding)         }, // ImPlotStyleVar_FitPadding
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, PlotDefaultSize)    }, // ImPlotStyleVar_PlotDefaultSize
	{ ImGuiDataType_Float, 2, (ImU32)offsetof(ImPlotStyle, PlotMinSize)        }  // ImPlotStyleVar_PlotMinSize
};

static const mvGuiStyleVarInfo* GetPlotStyleVarInfo(ImPlotStyleVar idx) {
	IM_ASSERT(idx >= 0 && idx < ImPlotStyleVar_COUNT);
	IM_ASSERT(IM_ARRAYSIZE(GPlotStyleVarInfo) == ImPlotStyleVar_COUNT);
	return &GPlotStyleVarInfo[idx];
}

static const mvGuiStyleVarInfo GNodeStyleVarInfo[] = {
	// ImNodesStyleVar_GridSpacing
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, GridSpacing)},
	// ImNodesStyleVar_NodeCornerRounding
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, NodeCornerRounding)},
	// ImNodesStyleVar_NodePadding
	{ImGuiDataType_Float, 2, (ImU32)offsetof(ImNodesStyle, NodePadding)},
	// ImNodesStyleVar_NodeBorderThickness
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, NodeBorderThickness)},
	// ImNodesStyleVar_LinkThickness
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, LinkThickness)},
	// ImNodesStyleVar_LinkLineSegmentsPerLength
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, LinkLineSegmentsPerLength)},
	// ImNodesStyleVar_LinkHoverDistance
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, LinkHoverDistance)},
	// ImNodesStyleVar_PinCircleRadius
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, PinCircleRadius)},
	// ImNodesStyleVar_PinQuadSideLength
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, PinQuadSideLength)},
	// ImNodesStyleVar_PinTriangleSideLength
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, PinTriangleSideLength)},
	// ImNodesStyleVar_PinLineThickness
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, PinLineThickness)},
	// ImNodesStyleVar_PinHoverRadius
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, PinHoverRadius)},
	// ImNodesStyleVar_PinOffset
	{ImGuiDataType_Float, 1, (ImU32)offsetof(ImNodesStyle, PinOffset)},
	// ImNodesStyleVar_MiniMapPadding
	{ImGuiDataType_Float, 2, (ImU32)offsetof(ImNodesStyle, MiniMapPadding)},
	// ImNodesStyleVar_MiniMapOffset
	{ImGuiDataType_Float, 2, (ImU32)offsetof(ImNodesStyle, MiniMapOffset)},
};

static const mvGuiStyleVarInfo* GetNodeStyleVarInfo(ImPlotStyleVar idx) {
	IM_ASSERT(idx >= 0 && idx < ImNodesStyleVar_COUNT);
	IM_ASSERT(IM_ARRAYSIZE(GNodeStyleVarInfo) == ImNodesStyleVar_COUNT);
	return &GNodeStyleVarInfo[idx];
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

	if (info.parentPtr)
		info.parentPtr->info.triggerAlternativeAction = true;
}

void mvThemeStyle::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
}

void mvThemeStyle::push_theme_style()
{
	if (_libType == mvLibType::MV_IMGUI)
	{
		const mvGuiStyleVarInfo* var_info = GetStyleVarInfo(_targetStyle);
		if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
			ImGui::PushStyleVar(_targetStyle, { (*_value)[0], (*_value)[1] });
		else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
			ImGui::PushStyleVar(_targetStyle, (*_value)[0]);
	}
	else if (_libType == mvLibType::OT)
	{
		const mvGuiStyleVarInfo* var_info = GetPlotStyleVarInfo(_targetStyle);
		if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
			ImPlot::PushStyleVar(_targetStyle, (*_value)[0]);
		else if (var_info->Type == ImGuiDataType_S32 && var_info->Count == 1)
			ImPlot::PushStyleVar(_targetStyle, (int)(*_value)[0]);
		else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
			ImPlot::PushStyleVar(_targetStyle, { (*_value)[0], (*_value)[1] });
	}
	else if (_libType == mvLibType::MV_IMNODES)
	{
		const mvGuiStyleVarInfo* var_info = GetNodeStyleVarInfo(_targetStyle);
		if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
			ImNodes::PushStyleVar(_targetStyle, { (*_value)[0], (*_value)[1] });
		else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
			ImNodes::PushStyleVar(_targetStyle, (*_value)[0]);
	}
}

void mvThemeStyle::pop_theme_style()
{
	if (_libType == mvLibType::MV_IMGUI)
		ImGui::PopStyleVar();
	else if (_libType == mvLibType::OT)
		ImPlot::PopStyleVar();
	else if (_libType == mvLibType::MV_IMNODES)
		ImNodes::PopStyleVar();
}

void mvThemeStyle::handleSpecificPositionalArgs(PyObject* dict)
{
	if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
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
			state.ok = false;
			mvThrowPythonError(mvErrorCode::mvNone, "");
		}
	}

	else if (_libType == mvLibType::OT)
	{
		if (_targetStyle >= ImPlotStyleVar_COUNT || _targetStyle < 0)
		{
			state.ok = false;
			mvThrowPythonError(mvErrorCode::mvNone, "");
		}
	}

	else if (_libType == mvLibType::MV_IMNODES)
	{
		if (_targetStyle >= 14 || _targetStyle < 0)
		{
			state.ok = false;
			mvThrowPythonError(mvErrorCode::mvNone, "");
		}
	}
}

void mvThemeStyle::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_target = ToPyInt(_targetStyle);
	mvPyObject py_cat = ToPyInt((int)_libType);

	PyDict_SetItemString(dict, "target", py_target);
	PyDict_SetItemString(dict, "category", py_cat);
}
