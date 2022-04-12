#include "mvPlotting.h"

#include <utility>
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"
#include "AppItems/plots/mvPlot.h"
#include "AppItems/plots/mvSubPlots.h"

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvDragLineConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<double>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvDragPointConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::array<double, 4>>*>(item.getValue());
}

void 
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvBarSeriesConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvBasicSeriesConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mv2dHistogramSeriesConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvErrorSeriesConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvHeatSeriesConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvHistogramSeriesConfig& outConfig)
{
	if (dataSource == item.config.source) return;
	item.config.source = dataSource;

	mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
	if (!srcItem)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), &item);
		return;
	}
	if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), &item);
		return;
	}
	outConfig.value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::draw_plot_legend(ImDrawList* drawlist, mvAppItem& item, mvPlotLegendConfig& config)
{
	if (!item.config.show)
		return;

	if (config.dirty)
	{
		ImPlot::SetLegendLocation(config.legendLocation, config.horizontal ? ImPlotOrientation_Horizontal : ImPlotOrientation_Vertical, config.outside);
		config.dirty = false;
	}

	UpdateAppItemState(item.state);

	if (item.font)
	{
		ImGui::PopFont();
	}

	if (item.theme)
	{
		static_cast<mvTheme*>(item.theme.get())->customAction();
	}

	if (item.config.dropCallback)
	{
		if (ImPlot::BeginDragDropTargetLegend())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item.config.payloadType.c_str()))
			{
				auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
				mvAddCallback(item.config.dropCallback, item.uuid, payloadActual->getDragData(), nullptr);
			}

			ImPlot::EndDragDropTarget();
		}
	}
}

void
DearPyGui::draw_drag_line(ImDrawList* drawlist, mvAppItem& item, mvDragLineConfig& config)
{
	if (!item.config.show)
		return;

	ScopedID id(item.uuid);

	if (config.vertical)
	{
		if (ImPlot::DragLineX(item.config.specifiedLabel.c_str(), config.value.get(), config.show_label, config.color, config.thickness))
		{
			mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
		}
	}
	else
	{
		if (ImPlot::DragLineY(item.config.specifiedLabel.c_str(), config.value.get(), config.show_label, config.color, config.thickness))
		{
			mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
		}
	}
}

void
DearPyGui::draw_drag_point(ImDrawList* drawlist, mvAppItem& item, mvDragPointConfig& config)
{
	if (!item.config.show)
		return;

	ScopedID id(item.uuid);

	static double dummyx = (*config.value.get())[0];
	static double dummyy = (*config.value.get())[1];
	dummyx = (*config.value.get())[0];
	dummyy = (*config.value.get())[1];

	if (ImPlot::DragPoint(item.config.specifiedLabel.c_str(), &dummyx, &dummyy, config.show_label, config.color, config.radius))
	{
		(*config.value.get())[0] = dummyx;
		(*config.value.get())[1] = dummyy;
		mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
	}
}

void 
DearPyGui::draw_bar_series(ImDrawList* drawlist, mvAppItem& item, const mvBarSeriesConfig& config)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];

		if (config.horizontal)
			ImPlot::PlotBarsH(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.weight);
		else
			ImPlot::PlotBars(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.weight);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_line_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];

		ImPlot::PlotLine(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_scatter_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];

		ImPlot::PlotScatter(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------
	// 
	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_stair_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];

		ImPlot::PlotStairs(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------
	// 
	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_stem_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];

		ImPlot::PlotStems(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------
	// 
	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_shade_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* y1ptr;
		static const std::vector<double>* y2ptr;

		xptr = &(*config.value.get())[0];
		y1ptr = &(*config.value.get())[1];
		y2ptr = &(*config.value.get())[2];

		ImPlot::PlotShaded(item.info.internalLabel.c_str(), xptr->data(), y1ptr->data(),
			y2ptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------
	// 
	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_hline_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;

		xptr = &(*config.value.get())[0];

		ImPlot::PlotHLines(item.info.internalLabel.c_str(), xptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_vline_series(ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;

		xptr = &(*config.value.get())[0];

		ImPlot::PlotVLines(item.info.internalLabel.c_str(), xptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_2dhistogram_series(ImDrawList* drawlist, mvAppItem& item, const mv2dHistogramSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];

		ImPlot::PlotHistogram2D(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(),
			config.xbins, config.ybins, config.density, ImPlotLimits(config.xmin, config.xmax, config.ymin, config.ymax), config.outliers);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_error_series(ImDrawList* drawlist, mvAppItem& item, const mvErrorSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;
		static const std::vector<double>* zptr;
		static const std::vector<double>* wptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];
		zptr = &(*config.value.get())[2];
		wptr = &(*config.value.get())[3];

		if (config.horizontal)
			ImPlot::PlotErrorBarsH(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());
		else
			ImPlot::PlotErrorBars(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_heat_series(ImDrawList* drawlist, mvAppItem& item, const mvHeatSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;

		xptr = &(*config.value.get())[0];


		ImPlot::PlotHeatmap(item.info.internalLabel.c_str(), xptr->data(), config.rows, config.cols, config.scale_min, config.scale_max,
			config.format.c_str(), { config.bounds_min.x, config.bounds_min.y }, { config.bounds_max.x, config.bounds_max.y });

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void
DearPyGui::draw_histogram_series(ImDrawList* drawlist, mvAppItem& item, const mvHistogramSeriesConfig& config)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!item.config.show)
		return;

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;

		xptr = &(*config.value.get())[0];

		ImPlot::PlotHistogram(item.info.internalLabel.c_str(), xptr->data(), (int)xptr->size(), config.bins,
			config.cumlative, config.density, ImPlotRange(config.min, config.max), config.outliers, (double)config.barScale);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), 1))
		{
			for (auto& childset : item.childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}


	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (item.font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(&item);
}

void 
DearPyGui::set_required_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvBarSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvBasicSeriesConfig& outConfig)
{
	//if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvLineSeries)], inDict))
	//	return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	if(PyTuple_Size(inDict) > 1)
		(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mv2dHistogramSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mv2dHistogramSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvErrorSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
	(*outConfig.value)[2] = ToDoubleVect(PyTuple_GetItem(inDict, 2));
	(*outConfig.value)[3] = ToDoubleVect(PyTuple_GetItem(inDict, 3));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvHeatSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvHeatSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	outConfig.rows = ToInt(PyTuple_GetItem(inDict, 1));
	outConfig.cols = ToInt(PyTuple_GetItem(inDict, 2));
	(*outConfig.value)[1].push_back(outConfig.bounds_min.y);
	(*outConfig.value)[1].push_back(outConfig.bounds_max.y);
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvHistogramSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvHistogramSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
}

void
DearPyGui::set_configuration(PyObject* inDict, mvDragLineConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "color")) outConfig.color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "thickness")) outConfig.thickness = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "show_label")) outConfig.show_label = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "vertical")) outConfig.vertical = ToBool(item);

}

void
DearPyGui::set_configuration(PyObject* inDict, mvDragPointConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "color")) outConfig.color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "radius")) outConfig.radius = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "show_label")) outConfig.show_label = ToBool(item);

}

void
DearPyGui::set_configuration(PyObject* inDict, mvPlotLegendConfig& outConfig, mvAppItem& item)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "location")) { outConfig.legendLocation = ToInt(item); outConfig.dirty = true; }
	if (PyObject* item = PyDict_GetItemString(inDict, "horizontal")) { outConfig.horizontal = ToBool(item); outConfig.dirty = true; }
	if (PyObject* item = PyDict_GetItemString(inDict, "outside")) { outConfig.outside = ToBool(item); outConfig.dirty = true; }

	if (item.info.shownLastFrame)
	{
		item.info.shownLastFrame = false;
		if (auto plot = static_cast<mvPlot*>(item.info.parentPtr))
			plot->removeFlag(ImPlotFlags_NoLegend);
		else if (auto plot = static_cast<mvSubPlots*>(item.info.parentPtr))
			plot->removeFlag(ImPlotSubplotFlags_NoLegend);
		item.config.show = true;
	}

	if (item.info.hiddenLastFrame)
	{
		item.info.hiddenLastFrame = false;
		if (auto plot = static_cast<mvPlot*>(item.info.parentPtr))
			plot->addFlag(ImPlotFlags_NoLegend);
		else if (auto plot = static_cast<mvSubPlots*>(item.info.parentPtr))
			plot->addFlag(ImPlotSubplotFlags_NoLegend);
		item.config.show = false;
	}
}

void
DearPyGui::set_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "horizontal")) outConfig.horizontal = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "weight")) outConfig.weight = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

}

void 
DearPyGui::set_configuration(PyObject* inDict, mvBasicSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

}

void
DearPyGui::set_configuration(PyObject* inDict, mv2dHistogramSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "xbins")) { outConfig.xbins = ToInt(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "ybins")) { outConfig.ybins = ToInt(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "xmin_range")) { outConfig.xmin = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "xmax_range")) { outConfig.xmax = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "ymin_range")) { outConfig.ymin = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "ymax_range")) { outConfig.ymax = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "density")) { outConfig.density = ToBool(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "outliers")) { outConfig.outliers = ToBool(item); }
}

void
DearPyGui::set_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "horizontal")) outConfig.horizontal = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "negative")) { (*outConfig.value)[2] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "positive")) { (*outConfig.value)[3] = ToDoubleVect(item); }
}

void
DearPyGui::set_configuration(PyObject* inDict, mvHeatSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "format")) outConfig.format = ToString(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "rows")) outConfig.rows = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "cols")) outConfig.cols = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "bounds_min")) outConfig.bounds_min = ToPoint(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "bounds_max")) outConfig.bounds_max = ToPoint(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "scale_min")) outConfig.scale_min = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "scale_max")) outConfig.scale_max = ToDouble(item);

	bool valueChanged = false;
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { valueChanged = true; (*outConfig.value)[0] = ToDoubleVect(item); }

	if (valueChanged)
	{
		(*outConfig.value)[1].push_back(outConfig.bounds_min.y);
		(*outConfig.value)[1].push_back(outConfig.bounds_max.y);
	}
}

void
DearPyGui::set_configuration(PyObject* inDict, mvHistogramSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "bins")) { outConfig.bins = ToInt(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "bar_scale")) { outConfig.barScale = ToFloat(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "min_range")) { outConfig.min = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "max_range")) { outConfig.max = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "cumlative")) { outConfig.cumlative = ToBool(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "density")) { outConfig.density = ToBool(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "outliers")) { outConfig.outliers = ToBool(item); }
}

void
DearPyGui::fill_configuration_dict(const mvDragLineConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	mvPyObject py_color = ToPyColor(inConfig.color);
	mvPyObject py_thickness = ToPyFloat(inConfig.thickness);
	mvPyObject py_show_label = ToPyBool(inConfig.show_label);
	mvPyObject py_vertical = ToPyBool(inConfig.vertical);

	PyDict_SetItemString(outDict, "color", py_color);
	PyDict_SetItemString(outDict, "thickness", py_thickness);
	PyDict_SetItemString(outDict, "show_label", py_show_label);
	PyDict_SetItemString(outDict, "vertical", py_vertical);
}

void
DearPyGui::fill_configuration_dict(const mvDragPointConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	mvPyObject py_color = ToPyColor(inConfig.color);
	mvPyObject py_radius = ToPyFloat(inConfig.radius);
	mvPyObject py_show_label = ToPyBool(inConfig.show_label);

	PyDict_SetItemString(outDict, "color", py_color);
	PyDict_SetItemString(outDict, "radius", py_radius);
	PyDict_SetItemString(outDict, "show_label", py_show_label);
}

void
DearPyGui::fill_configuration_dict(const mvPlotLegendConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "location", mvPyObject(ToPyInt(inConfig.legendLocation)));
	PyDict_SetItemString(outDict, "horizontal", mvPyObject(ToPyBool(inConfig.horizontal)));
	PyDict_SetItemString(outDict, "outside", mvPyObject(ToPyBool(inConfig.outside)));

}

void 
DearPyGui::fill_configuration_dict(const mvBarSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	mvPyObject py_horizontal = ToPyBool(inConfig.horizontal);
	mvPyObject py_weight = ToPyFloat(inConfig.weight);

	PyDict_SetItemString(outDict, "horizontal", py_horizontal);
	PyDict_SetItemString(outDict, "weight", py_weight);
}

void
DearPyGui::fill_configuration_dict(const mvBasicSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
}

void
DearPyGui::fill_configuration_dict(const mv2dHistogramSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "xbins", mvPyObject(ToPyBool(inConfig.xbins)));
	PyDict_SetItemString(outDict, "ybins", mvPyObject(ToPyBool(inConfig.ybins)));
	PyDict_SetItemString(outDict, "xmin_range", mvPyObject(ToPyBool(inConfig.xmin)));
	PyDict_SetItemString(outDict, "xmax_range", mvPyObject(ToPyBool(inConfig.xmax)));
	PyDict_SetItemString(outDict, "ymin_range", mvPyObject(ToPyBool(inConfig.ymin)));
	PyDict_SetItemString(outDict, "ymax_range", mvPyObject(ToPyBool(inConfig.ymax)));
	PyDict_SetItemString(outDict, "density", mvPyObject(ToPyBool(inConfig.density)));
	PyDict_SetItemString(outDict, "outliers", mvPyObject(ToPyBool(inConfig.outliers)));
}

void
DearPyGui::fill_configuration_dict(const mvErrorSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "horizontal", mvPyObject(ToPyBool(inConfig.horizontal)));
}

void
DearPyGui::fill_configuration_dict(const mvHeatSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "format", mvPyObject(ToPyString(inConfig.format)));
	PyDict_SetItemString(outDict, "rows", mvPyObject(ToPyInt(inConfig.rows)));
	PyDict_SetItemString(outDict, "cols", mvPyObject(ToPyInt(inConfig.cols)));
	PyDict_SetItemString(outDict, "bounds_min", mvPyObject(ToPyPair(inConfig.bounds_min.x, inConfig.bounds_min.y)));
	PyDict_SetItemString(outDict, "bounds_max", mvPyObject(ToPyPair(inConfig.bounds_max.x, inConfig.bounds_max.y)));
	PyDict_SetItemString(outDict, "scale_min", mvPyObject(ToPyDouble(inConfig.scale_min)));
	PyDict_SetItemString(outDict, "scale_max", mvPyObject(ToPyDouble(inConfig.scale_max)));
}

void
DearPyGui::fill_configuration_dict(const mvHistogramSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "bins", mvPyObject(ToPyBool(inConfig.bins)));
	PyDict_SetItemString(outDict, "bar_scale", mvPyObject(ToPyBool(inConfig.barScale)));
	PyDict_SetItemString(outDict, "min_range", mvPyObject(ToPyBool(inConfig.min)));
	PyDict_SetItemString(outDict, "max_range", mvPyObject(ToPyBool(inConfig.max)));
	PyDict_SetItemString(outDict, "cumlative", mvPyObject(ToPyBool(inConfig.cumlative)));
	PyDict_SetItemString(outDict, "density", mvPyObject(ToPyBool(inConfig.density)));
	PyDict_SetItemString(outDict, "outliers", mvPyObject(ToPyBool(inConfig.outliers)));
}

void
DearPyGui::apply_template(const mvDragLineConfig& sourceConfig, mvDragLineConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
	dstConfig.disabled_value = sourceConfig.disabled_value;
	dstConfig.show_label = sourceConfig.show_label;
	dstConfig.color = sourceConfig.color;
	dstConfig.thickness = sourceConfig.thickness;
	dstConfig.vertical = sourceConfig.vertical;
}

void
DearPyGui::apply_template(const mvDragPointConfig& sourceConfig, mvDragPointConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
	dstConfig.disabled_value[0] = sourceConfig.disabled_value[0];
	dstConfig.disabled_value[1] = sourceConfig.disabled_value[1];
	dstConfig.disabled_value[2] = sourceConfig.disabled_value[2];
	dstConfig.disabled_value[3] = sourceConfig.disabled_value[3];
	dstConfig.show_label = sourceConfig.show_label;
	dstConfig.color = sourceConfig.color;
	dstConfig.radius = sourceConfig.radius;
}

void
DearPyGui::apply_template(const mvPlotLegendConfig& sourceConfig, mvPlotLegendConfig& dstConfig)
{
	dstConfig.legendLocation = sourceConfig.legendLocation;
	dstConfig.horizontal = sourceConfig.horizontal;
	dstConfig.outside = sourceConfig.outside;
}

void 
DearPyGui::apply_template(const mvBarSeriesConfig& sourceConfig, mvBarSeriesConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
	dstConfig.horizontal = sourceConfig.horizontal;
	dstConfig.weight = sourceConfig.weight;
}

void
DearPyGui::apply_template(const mvBasicSeriesConfig& sourceConfig, mvBasicSeriesConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
}

void
DearPyGui::apply_template(const mv2dHistogramSeriesConfig& sourceConfig, mv2dHistogramSeriesConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
	dstConfig.xbins = sourceConfig.xbins;
	dstConfig.ybins = sourceConfig.ybins;
	dstConfig.density = sourceConfig.density;
	dstConfig.outliers = sourceConfig.outliers;
	dstConfig.xmin = sourceConfig.xmin;
	dstConfig.xmax = sourceConfig.xmax;
	dstConfig.ymin = sourceConfig.ymin;
	dstConfig.ymax = sourceConfig.ymax;
}

void
DearPyGui::apply_template(const mvErrorSeriesConfig& sourceConfig, mvErrorSeriesConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
	dstConfig.horizontal = sourceConfig.horizontal;
}

void
DearPyGui::apply_template(const mvHeatSeriesConfig& sourceConfig, mvHeatSeriesConfig& dstConfig)
{
	dstConfig.value = sourceConfig.value;
	dstConfig.rows = sourceConfig.rows;
	dstConfig.cols = sourceConfig.cols;
	dstConfig.scale_min = sourceConfig.scale_min;
	dstConfig.scale_max = sourceConfig.scale_max;
	dstConfig.format = sourceConfig.format;
	dstConfig.bounds_min = sourceConfig.bounds_min;
	dstConfig.bounds_max = sourceConfig.bounds_max;
}

void
DearPyGui::apply_template(const mvHistogramSeriesConfig& sourceConfig, mvHistogramSeriesConfig& dstConfig)
{
	dstConfig.cumlative = sourceConfig.cumlative;
	dstConfig.value = sourceConfig.value;
	dstConfig.bins = sourceConfig.bins;
	dstConfig.density = sourceConfig.density;
	dstConfig.outliers = sourceConfig.outliers;
	dstConfig.min = sourceConfig.min;
	dstConfig.max = sourceConfig.max;
	dstConfig.barScale = sourceConfig.barScale;
}

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------

void 
mvDragPoint::setPyValue(PyObject* value)
{
	std::vector<double> temp = ToDoubleVect(value);
	while (temp.size() < 4)
		temp.push_back(0.0);
	std::array<double, 4> temp_array;
	for (size_t i = 0; i < temp_array.size(); i++)
		temp_array[i] = temp[i];
	if (configData.value)
		*configData.value = temp_array;
	else
		configData.value = std::make_shared<std::array<double, 4>>(temp_array);
}