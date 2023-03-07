#include "mvPlotting.h"
#include <utility>
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPyUtils.h"
#include "mvFontItems.h"
#include "mvThemes.h"
#include "mvContainers.h"
#include "mvTextureItems.h"
#include "mvItemHandlers.h"

static void
draw_polygon(const mvAreaSeriesConfig& config)
{

	static const std::vector<double>* xptr;
	static const std::vector<double>* yptr;

	xptr = &(*config.value.get())[0];
	yptr = &(*config.value.get())[1];

	std::vector<ImVec2> points;
	for (unsigned i = 0; i < xptr->size(); i++)
	{
		auto p = ImPlot::PlotToPixels({ (*xptr)[i], (*yptr)[i] });
		points.push_back(p);
	}

	if (config.fill.r > 0.0f)
	{
		size_t i;
		double y;
		double miny, maxy;
		double x1, y1;
		double x2, y2;
		int ind1, ind2;
		size_t ints;
		size_t n = points.size();
		int* polyints = new int[n];


		/* Get plot Y range in pixels */
		ImPlotLimits limits = ImPlot::GetPlotLimits();
		auto upperLimitsPix = ImPlot::PlotToPixels({ limits.X.Max, limits.Y.Max });
		auto lowerLimitsPix = ImPlot::PlotToPixels({ limits.X.Min, limits.Y.Min });

		/* Determine Y range of data*/
		miny = (int)points[0].y;
		maxy = (int)points[0].y;
		for (i = 1; i < n; i++)
		{
			miny = std::min((int)miny, (int)points[i].y);
			maxy = std::max((int)maxy, (int)points[i].y);
		}

		/* Determine to clip scans based on plot bounds y or data bounds y
		when the plot data is converted the min and max y invert (due to plot to graphics coord)
		so we comapre min with max and max with min*/
		miny = std::max((int)miny, (int)upperLimitsPix.y);
		maxy = std::min((int)maxy, (int)lowerLimitsPix.y);

		/* Draw, scanning y */
		for (y = miny; y <= maxy; y++) {
			ints = 0;
			for (i = 0; (i < n); i++) {
				if (!i)
				{
					ind1 = (int)n - 1;
					ind2 = 0;
				}
				else
				{
					ind1 = (int)i - 1;
					ind2 = (int)i;
				}
				y1 = (int)points[ind1].y;
				y2 = (int)points[ind2].y;
				if (y1 < y2)
				{
					x1 = (int)points[ind1].x;
					x2 = (int)points[ind2].x;
				}
				else if (y1 > y2)
				{
					y2 = (int)points[ind1].y;
					y1 = (int)points[ind2].y;
					x2 = (int)points[ind1].x;
					x1 = (int)points[ind2].x;
				}
				else
					continue;

				if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
					polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;

			}

			auto compare_int = [](const void* a, const void* b)
			{
				return (*(const int*)a) - (*(const int*)b);
			};

			qsort(polyints, ints, sizeof(int), compare_int);

			for (i = 0; i < ints; i += 2)
				ImGui::GetWindowDrawList()->AddLine({ (float)polyints[i], (float)y }, { (float)polyints[i + 1], (float)y }, config.fill, 1.0f);
		}
		delete[] polyints;
	}

}

template <typename T>
int BinarySearch(const T* arr, int l, int r, T x) {
	if (r >= l) {
		int mid = l + (r - l) / 2;
		if (arr[mid] == x)
			return mid;
		if (arr[mid] > x)
			return BinarySearch(arr, l, mid - 1, x);
		return BinarySearch(arr, mid + 1, r, x);
	}
	return -1;
}

static void
PlotCandlestick(const char* label_id, const double* xs, const double* opens,
	const double* closes, const double* lows, const double* highs, int count,
	bool tooltip, float width_percent, const ImVec4& bullCol, const ImVec4& bearCol, int time_unit)
{

	ImDrawList* draw_list = ImPlot::GetPlotDrawList();
	// calc real value width
	float half_width = count > 1 ? ((float)xs[1] - (float)xs[0]) * width_percent : width_percent;

	// custom tool
	if (ImPlot::IsPlotHovered() && tooltip) {
		ImPlotPoint mouse = ImPlot::GetPlotMousePos();
		mouse.x = ImPlot::RoundTime(ImPlotTime::FromDouble(mouse.x), time_unit).ToDouble();
		float  tool_l = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
		float  tool_r = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
		float  tool_t = ImPlot::GetPlotPos().y;
		float  tool_b = tool_t + ImPlot::GetPlotSize().y;
		ImPlot::PushPlotClipRect();
		draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128, 128, 128, 64));
		ImPlot::PopPlotClipRect();

		// find mouse location index
		int idx = BinarySearch(xs, 0, count - 1, mouse.x);

		if (idx != -1)
		{
			ImGui::BeginTooltip();
			if (time_unit == ImPlotTimeUnit_Day)
			{
				char buff[32];
				ImPlot::FormatDate(ImPlotTime::FromDouble(xs[idx]), buff, 32, ImPlotDateFmt_DayMoYr, ImPlot::GetStyle().UseISO8601);
				ImGui::Text("Day:   %s", buff);
			}
			else if (time_unit == ImPlotTimeUnit_Us)
			{
				ImGui::Text("Microsecond: %f", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Ms)
			{
				ImGui::Text("Millisecond: %f", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_S)
			{
				ImGui::Text("Second: %f", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Min)
			{
				ImGui::Text("Minute: %f", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Hr)
			{
				ImGui::Text("Hour: %f", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Mo)
			{
				ImGui::Text("Month: %f", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Yr)
			{
				ImGui::Text("Year: %f", xs[idx]);
			}
			ImGui::Text("Open:  $%.2f", opens[idx]);
			ImGui::Text("Close: $%.2f", closes[idx]);
			ImGui::Text("Low:   $%.2f", lows[idx]);
			ImGui::Text("High:  $%.2f", highs[idx]);
			ImGui::EndTooltip();
		}
	}

	// begin plot item
	if (ImPlot::BeginItem(label_id)) {
		// override legend icon color
		ImPlot::GetCurrentItem()->Color = ImGui::ColorConvertFloat4ToU32({ 0.25f, 0.25f, 0.25f, 1.0f });
		// fit data if requested
		if (ImPlot::FitThisFrame()) {
			for (int i = 0; i < count; ++i) {
				ImPlot::FitPoint(ImPlotPoint(xs[i], lows[i]));
				ImPlot::FitPoint(ImPlotPoint(xs[i], highs[i]));
			}
		}
		// render data
		for (int i = 0; i < count; ++i) {
			ImVec2 open_pos = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
			ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
			ImVec2 low_pos = ImPlot::PlotToPixels(xs[i], lows[i]);
			ImVec2 high_pos = ImPlot::PlotToPixels(xs[i], highs[i]);
			ImU32 color = ImGui::GetColorU32(opens[i] > closes[i] ? bearCol : bullCol);
			draw_list->AddLine(low_pos, high_pos, color);
			draw_list->AddRectFilled(open_pos, close_pos, color);
		}

		// end plot item
		ImPlot::EndItem();
	}
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvAnnotationConfig& outConfig)
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
	outConfig.value = *static_cast<std::shared_ptr<std::array<double, 4>>*>(srcItem->getValue());
}

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
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, std::shared_ptr<std::vector<std::vector<double>>>& outValue)
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
	outValue = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(srcItem->getValue());
}

void
DearPyGui::draw_plot(ImDrawList* drawlist, mvAppItem& item, mvPlotConfig& config)
{
	if (!item.config.show)
		return;

	// cache old cursor position
	ImVec2 previousCursorPos = ImGui::GetCursorPos();

	// set cursor position if user set
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(item.state.pos);

	// update widget's position state
	item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

	// push font if a font object is attached
	if (item.font)
	{
		ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(&item);

	// Must do this because these items are not avalable as style items
	// these are here because they need to be applied every plot
		ImPlot::GetStyle().UseLocalTime = config.localTime;
		ImPlot::GetStyle().UseISO8601 = config.iSO8601;
		ImPlot::GetStyle().Use24HourClock = config.clock24Hour;

	if (config._newColorMap)
	{
		ImPlot::BustColorCache(item.info.internalLabel.c_str());
		config._newColorMap = false;
	}

	if (config._useColorMap)
		ImPlot::PushColormap(config._colormap);

	// custom input mapping
	ImPlot::GetInputMap().PanButton = config.pan_button;
	ImPlot::GetInputMap().FitButton = config.fit_button;
	ImPlot::GetInputMap().ContextMenuButton = config.context_menu_button;
	ImPlot::GetInputMap().BoxSelectButton = config.box_select_button;
	ImPlot::GetInputMap().BoxSelectCancelButton = config.box_select_cancel_button;
	ImPlot::GetInputMap().QueryButton = config.query_button;
	ImPlot::GetInputMap().QueryToggleMod = config.query_toggle_mod;
	ImPlot::GetInputMap().HorizontalMod = config.horizontal_mod;
	ImPlot::GetInputMap().VerticalMod = config.vertical_mod;
	if (config.pan_mod != -1) ImPlot::GetInputMap().PanMod = config.pan_mod;
	if (config.box_select_mod != -1) ImPlot::GetInputMap().BoxSelectMod = config.box_select_mod;
	if (config.query_mod != -1) ImPlot::GetInputMap().QueryMod = config.query_mod;

	// gives axes change to make changes to ticks, limits, etc.
	for (auto& child : item.childslots[1])
	{
		// skip item if it's not shown
		if (!child->config.show)
			continue;

		if (child->type == mvAppItemType::mvPlotAxis)
		{
			auto axis = static_cast<mvPlotAxis*>(child.get());
			if (axis->configData.setLimits || axis->configData._dirty)
			{
				switch (axis->info.location)
				{
				case(0): ImPlot::SetNextPlotLimitsX(axis->configData.limits.x, axis->configData.limits.y, ImGuiCond_Always); break;
				case(1): ImPlot::SetNextPlotLimitsY(axis->configData.limits.x, axis->configData.limits.y, ImGuiCond_Always); break;
				case(2): ImPlot::SetNextPlotLimitsY(axis->configData.limits.x, axis->configData.limits.y, ImGuiCond_Always, ImPlotYAxis_2); break;
				case(3): ImPlot::SetNextPlotLimitsY(axis->configData.limits.x, axis->configData.limits.y, ImGuiCond_Always, ImPlotYAxis_3); break;
				default: ImPlot::SetNextPlotLimitsY(axis->configData.limits.x, axis->configData.limits.y, ImGuiCond_Always); break;
				}
				axis->configData._dirty = false;
			}

			if (!axis->configData.labels.empty())
			{
				// TODO: Checks
				if (axis->info.location == 0)
					ImPlot::SetNextPlotTicksX(axis->configData.labelLocations.data(), (int)axis->configData.labels.size(), axis->configData.clabels.data());
				else
					ImPlot::SetNextPlotTicksY(axis->configData.labelLocations.data(), (int)axis->configData.labels.size(), axis->configData.clabels.data());
			}
		}
		else
			child->customAction();
	}

	if (config._fitDirty)
	{
		ImPlot::FitNextPlotAxes(config._axisfitDirty[0], config._axisfitDirty[1], config._axisfitDirty[2], config._axisfitDirty[3]);
		config._fitDirty = false;
		config._axisfitDirty[0] = false;
		config._axisfitDirty[1] = false;
		config._axisfitDirty[2] = false;
		config._axisfitDirty[3] = false;
	}

	if (ImPlot::BeginPlot(item.info.internalLabel.c_str(),
		config.xaxisName.empty() ? nullptr : config.xaxisName.c_str(),
		config._y1axisName.empty() ? nullptr : config._y1axisName.c_str(),
		ImVec2((float)item.config.width, (float)item.config.height),
		config._flags, config._xflags, config._yflags, config._y1flags, config._y2flags,
		config._y2axisName.empty() ? nullptr : config._y2axisName.c_str(),
		config._y3axisName.empty() ? nullptr : config._y3axisName.c_str()))
	{

		auto context = ImPlot::GetCurrentContext();
		// legend, drag point and lines
		for (auto& child : item.childslots[0])
			child->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

		// axes
		for (auto& child : item.childslots[1])
			child->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

		ImPlot::PushPlotClipRect();

		ImPlot::SetPlotYAxis(ImPlotYAxis_1); // draw items should use first plot axis

		// drawings
		for (auto& child : item.childslots[2])
		{
			// skip item if it's not shown
			if (!child->config.show)
				continue;

			//item->draw(ImPlot::GetPlotDrawList(), ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
			child->draw(ImPlot::GetPlotDrawList(), 0.0f, 0.0f);

			UpdateAppItemState(child->state);
		}

		ImPlot::PopPlotClipRect();

		if (config._useColorMap)
			ImPlot::PopColormap();

		config._queried = ImPlot::IsPlotQueried();

		if (config._queried)
		{
			ImPlotLimits area = ImPlot::GetPlotQuery();
			config._queryArea[0] = area.X.Min;
			config._queryArea[1] = area.X.Max;
			config._queryArea[2] = area.Y.Min;
			config._queryArea[3] = area.Y.Max;
		}

		if (item.config.callback != nullptr && config._queried)
		{

			if (item.config.alias.empty())
				mvSubmitCallback([=, &item]() {
				PyObject* area = PyTuple_New(4);
				PyTuple_SetItem(area, 0, PyFloat_FromDouble(config._queryArea[0]));
				PyTuple_SetItem(area, 1, PyFloat_FromDouble(config._queryArea[1]));
				PyTuple_SetItem(area, 2, PyFloat_FromDouble(config._queryArea[2]));
				PyTuple_SetItem(area, 3, PyFloat_FromDouble(config._queryArea[3]));
				mvAddCallback(item.config.callback, item.uuid, area, item.config.user_data);
					});
			else
				mvSubmitCallback([=, &item]() {
				PyObject* area = PyTuple_New(4);
				PyTuple_SetItem(area, 0, PyFloat_FromDouble(config._queryArea[0]));
				PyTuple_SetItem(area, 1, PyFloat_FromDouble(config._queryArea[1]));
				PyTuple_SetItem(area, 2, PyFloat_FromDouble(config._queryArea[2]));
				PyTuple_SetItem(area, 3, PyFloat_FromDouble(config._queryArea[3]));
				mvAddCallback(item.config.callback, item.config.alias, area, item.config.user_data);
					});
		}

		if (ImPlot::IsPlotHovered())
		{
			GContext->input.mousePlotPos.x = ImPlot::GetPlotMousePos().x;
			GContext->input.mousePlotPos.y = ImPlot::GetPlotMousePos().y;
		}

		// todo: resolve clipping
		if (item.config.dropCallback)
		{
			ScopedID id(item.uuid);
			if (ImPlot::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item.config.payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if (item.config.alias.empty())
						mvAddCallback(item.config.dropCallback, item.uuid, payloadActual->configData.dragData, nullptr);
					else
						mvAddCallback(item.config.dropCallback, item.config.alias, payloadActual->configData.dragData, nullptr);
				}

				ImPlot::EndDragDropTarget();
			}
		}

		// update state

		config._flags = context->CurrentPlot->Flags;

		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		{

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y;
			GContext->input.mousePos.x = (int)x;
			GContext->input.mousePos.y = (int)y;


			if (GContext->itemRegistry->activeWindow != item.uuid)
				GContext->itemRegistry->activeWindow = item.uuid;

		}

		// TODO: find a better way to handle this
		for (auto& child : item.childslots[0])
		{
			if (child->type == mvAppItemType::mvPlotLegend)
			{
				auto legend = static_cast<mvPlotLegend*>(child.get());
				legend->configData.legendLocation = context->CurrentPlot->Items.Legend.Location;
				legend->configData.horizontal = context->CurrentPlot->Items.Legend.Orientation == ImPlotOrientation_Horizontal;
				legend->configData.outside = context->CurrentPlot->Items.Legend.Outside;
				break;
			}
		}


		ImPlot::EndPlot();

	}

	// set cursor position to cached position
	if (item.info.dirtyPos)
		ImGui::SetCursorPos(previousCursorPos);

	ImPlot::GetInputMap() = config._originalMap;

	UpdateAppItemState(item.state);

	if (item.font)
	{
		ImGui::PopFont();
	}

	if (item.theme)
	{
		item.theme->pop_theme_components();
	}

	if (item.handlerRegistry)
		item.handlerRegistry->checkEvents(&item.state);

	// drag drop
	for (auto& child : item.childslots[3])
		child->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
}

void
DearPyGui::draw_plot_axis(ImDrawList* drawlist, mvAppItem& item, mvPlotAxisConfig& config)
{
	if (!item.config.show)
		return;

	// todo: add check
	if (config.axis != 0)
		ImPlot::SetPlotYAxis(item.info.location - 1);

	for (auto& item : item.childslots[1])
		item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

	// x axis
	if (config.axis == 0)
	{
		config.limits_actual.x = (float)ImPlot::GetPlotLimits(item.info.location).X.Min;
		config.limits_actual.y = (float)ImPlot::GetPlotLimits(item.info.location).X.Max;
		auto context = ImPlot::GetCurrentContext();
		config.flags = context->CurrentPlot->XAxis.Flags;
	}

	// y axis
	else
	{
		config.limits_actual.x = (float)ImPlot::GetPlotLimits(item.info.location - 1).Y.Min;
		config.limits_actual.y = (float)ImPlot::GetPlotLimits(item.info.location - 1).Y.Max;
		auto context = ImPlot::GetCurrentContext();
		config.flags = context->CurrentPlot->YAxis[item.info.location - 1].Flags;
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
		ScopedID id(item.uuid);
		if (item.info.location == 0 && ImPlot::BeginDragDropTargetX())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item.config.payloadType.c_str()))
			{
				auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
				mvAddCallback(item.config.dropCallback, item.uuid, payloadActual->configData.dragData, nullptr);
			}

			ImPlot::EndDragDropTarget();
		}
		else if (ImPlot::BeginDragDropTargetY(item.info.location - 1))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item.config.payloadType.c_str()))
			{
				auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
				mvAddCallback(item.config.dropCallback, item.uuid, payloadActual->configData.dragData, nullptr);
			}

			ImPlot::EndDragDropTarget();
		}
	}
}

void
DearPyGui::draw_subplots(ImDrawList* drawlist, mvAppItem& item, mvSubPlotsConfig& config)
{
	ScopedID id(item.uuid);

	if (ImPlot::BeginSubplots(item.info.internalLabel.c_str(), config.rows, config.cols, ImVec2((float)item.config.width, (float)item.config.height),
		config.flags, config.row_ratios.empty() ? nullptr : config.row_ratios.data(), config.col_ratios.empty() ? nullptr : config.col_ratios.data()))
	{

		// plots
		for (auto& item : item.childslots[1])
			item->draw(drawlist, 0.0f, 0.0f);

		ImPlot::EndSubplots();
	}
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
				mvAddCallback(item.config.dropCallback, item.uuid, payloadActual->configData.dragData, nullptr);
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
DearPyGui::draw_pie_series(ImDrawList* drawlist, mvAppItem& item, const mvPieSeriesConfig& config)
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

		ImPlot::PlotPieChart(config.clabels.data(), xptr->data(), (int)config.labels.size(),
			config.x, config.y, config.radius, config.normalize, config.format.c_str(), config.angle);

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
DearPyGui::draw_label_series(ImDrawList* drawlist, mvAppItem& item, const mvLabelSeriesConfig& config)
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

		ImPlot::PlotText(item.info.internalLabel.c_str(), (*xptr)[0], (*yptr)[0], config.vertical,
			ImVec2((float)config.xoffset, (float)config.yoffset));

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
DearPyGui::draw_image_series(ImDrawList* drawlist, mvAppItem& item, mvImageSeriesConfig& config)
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

		if (config._texture)
		{
			if (config._internalTexture)
				config._texture->draw(drawlist, 0.0f, 0.0f);

			if (!config._texture->state.ok)
				return;

			void* texture = nullptr;

			if (config._texture->type == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(config._texture.get())->_texture;
			else if (config._texture->type == mvAppItemType::mvRawTexture)
				texture = static_cast<mvRawTexture*>(config._texture.get())->_texture;
			else
				texture = static_cast<mvDynamicTexture*>(config._texture.get())->_texture;

			ImPlot::PlotImage(item.info.internalLabel.c_str(), texture, config.bounds_min, config.bounds_max, config.uv_min, config.uv_max, config.tintColor);

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
DearPyGui::draw_area_series(ImDrawList* drawlist, mvAppItem& item, const mvAreaSeriesConfig& config)
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

		ImPlot::PushPlotClipRect();
		ImPlot::RegisterOrGetItem(item.info.internalLabel.c_str());
		draw_polygon(config);
		ImPlot::PopPlotClipRect();

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
DearPyGui::draw_candle_series(ImDrawList* drawlist, mvAppItem& item, const mvCandleSeriesConfig& config)
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

		static const std::vector<double>* datesptr;
		static const std::vector<double>* openptr;
		static const std::vector<double>* closeptr;
		static const std::vector<double>* lowptr;
		static const std::vector<double>* highptr;

		datesptr = &(*config.value.get())[0];
		openptr = &(*config.value.get())[1];
		closeptr = &(*config.value.get())[2];
		lowptr = &(*config.value.get())[3];
		highptr = &(*config.value.get())[4];

		PlotCandlestick(item.info.internalLabel.c_str(), datesptr->data(), openptr->data(), closeptr->data(),
			lowptr->data(), highptr->data(), (int)datesptr->size(), config.tooltip, config.weight, config.bullColor,
			config.bearColor, config.timeunit);

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
DearPyGui::draw_custom_series(ImDrawList* drawlist, mvAppItem& item, mvCustomSeriesConfig& config)
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

		static std::vector<double>* xptr;
		static std::vector<double>* yptr;
		static std::vector<double>* y1ptr;
		static std::vector<double>* y2ptr;
		static std::vector<double>* y3ptr;

		xptr = &(*config.value.get())[0];
		yptr = &(*config.value.get())[1];
		y1ptr = &(*config.value.get())[2];
		y2ptr = &(*config.value.get())[3];
		y3ptr = &(*config.value.get())[4];

		ImDrawList* draw_list = ImPlot::GetPlotDrawList();

		if (ImPlot::IsPlotHovered() && !item.childslots[1].empty() && config.tooltip)
		{
			ImGui::BeginTooltip();
			for (auto& item : item.childslots[1])
				item->draw(draw_list, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			ImGui::EndTooltip();
		}

		// begin plot item
		if (ImPlot::BeginItem(item.info.internalLabel.c_str()))
		{
			// override legend icon color
			ImPlot::GetCurrentItem()->Color = ImGui::ColorConvertFloat4ToU32({ 0.25f, 0.25f, 0.25f, 1.0f });

			// fit data if requested
			if (ImPlot::FitThisFrame())
			{
				for (int i = 0; i < xptr->size(); ++i)
				{
					// TODO: make this configurable
					ImPlot::FitPoint(ImPlotPoint((*xptr)[i], (*yptr)[i]));
				}
			}

			// render data
			if (config.channelCount == 2)
			{
				for (int i = 0; i < xptr->size(); ++i)
				{
					ImVec2 y_pos = ImPlot::PlotToPixels((*xptr)[i], (*yptr)[i]);
					config._transformedValues[0][i] = y_pos.x;
					config._transformedValues[1][i] = y_pos.y;
				}
			}
			else if (config.channelCount == 3)
			{
				for (int i = 0; i < xptr->size(); ++i)
				{

					ImVec2 y_pos = ImPlot::PlotToPixels((*xptr)[i], (*yptr)[i]);
					ImVec2 y1_pos = ImPlot::PlotToPixels((*xptr)[i], (*y1ptr)[i]);
					config._transformedValues[0][i] = y_pos.x;
					config._transformedValues[1][i] = y_pos.y;
					config._transformedValues[2][i] = y1_pos.y;
				}
			}
			else if (config.channelCount == 4)
			{
				for (int i = 0; i < xptr->size(); ++i)
				{

					ImVec2 y_pos = ImPlot::PlotToPixels((*xptr)[i], (*yptr)[i]);
					ImVec2 y1_pos = ImPlot::PlotToPixels((*xptr)[i], (*y1ptr)[i]);
					ImVec2 y2_pos = ImPlot::PlotToPixels((*xptr)[i], (*y2ptr)[i]);
					config._transformedValues[0][i] = y_pos.x;
					config._transformedValues[1][i] = y_pos.y;
					config._transformedValues[2][i] = y1_pos.y;
					config._transformedValues[3][i] = y2_pos.y;
				}
			}
			else if (config.channelCount == 5)
			{
				for (int i = 0; i < xptr->size(); ++i)
				{

					ImVec2 y_pos = ImPlot::PlotToPixels((*xptr)[i], (*yptr)[i]);
					ImVec2 y1_pos = ImPlot::PlotToPixels((*xptr)[i], (*y1ptr)[i]);
					ImVec2 y2_pos = ImPlot::PlotToPixels((*xptr)[i], (*y2ptr)[i]);
					ImVec2 y3_pos = ImPlot::PlotToPixels((*xptr)[i], (*y3ptr)[i]);
					config._transformedValues[0][i] = y_pos.x;
					config._transformedValues[1][i] = y_pos.y;
					config._transformedValues[2][i] = y1_pos.y;
					config._transformedValues[3][i] = y2_pos.y;
					config._transformedValues[4][i] = y3_pos.y;
				}
			}
			ImPlotPoint mouse = ImPlot::GetPlotMousePos();
			ImVec2 mouse2 = ImPlot::PlotToPixels(mouse.x, mouse.y);
			static int extras = 4;
			mvSubmitCallback([&, mouse, mouse2]() {
				PyObject* helperData = PyDict_New();
				PyDict_SetItemString(helperData, "MouseX_PlotSpace", ToPyFloat(mouse.x));
				PyDict_SetItemString(helperData, "MouseY_PlotSpace", ToPyFloat(mouse.y));
				PyDict_SetItemString(helperData, "MouseX_PixelSpace", ToPyFloat(mouse2.x));
				PyDict_SetItemString(helperData, "MouseY_PixelSpace", ToPyFloat(mouse2.y));
				PyObject* appData = PyTuple_New(config.channelCount + extras);
				PyTuple_SetItem(appData, 0, helperData);
				for (int i = 1; i < config.channelCount + 1; i++)
					PyTuple_SetItem(appData, i, ToPyList(config._transformedValues[i-1]));
				mvAddCallback(item.config.callback, item.uuid, appData, item.config.user_data);
				});

			// drawings
			ImPlotPlot* currentPlot = ImPlot::GetCurrentContext()->CurrentPlot;
			ImPlot::GetCurrentContext()->CurrentPlot = nullptr;
			for (auto& child : item.childslots[2])
			{
				// skip item if it's not shown
				if (!child->config.show)
					continue;
				//child->draw(draw_list, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
				child->draw(ImPlot::GetPlotDrawList(), 0.0f, 0.0f);
				UpdateAppItemState(child->state);
			}
			ImPlot::GetCurrentContext()->CurrentPlot = currentPlot;

			// end plot item
			ImPlot::EndItem();
		}

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
DearPyGui::draw_plot_annotation(ImDrawList* drawlist, mvAppItem& item, mvAnnotationConfig& config)
{
	if (!item.config.show)
		return;

	ScopedID id(item.uuid);

	if (config.clamped)
		ImPlot::AnnotateClamped((*config.value.get())[0], (*config.value.get())[1], config.pixOffset, config.color.toVec4(), "%s", item.config.specifiedLabel.c_str());
	else
		ImPlot::Annotate((*config.value.get())[0], (*config.value.get())[1], config.pixOffset, config.color.toVec4(), "%s", item.config.specifiedLabel.c_str());

}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvBarSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvBasicSeriesConfig& outConfig)
{
	//if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvLineSeries)], inDict))
	//	return;

	for(int i = 0; i < PyTuple_Size(inDict); i++)
		(*outConfig.value)[i] = ToDoubleVect(PyTuple_GetItem(inDict, i));

	if (outConfig.type == mvAppItemType::mvShadeSeries)
	{
		(*outConfig.value)[2] = (*outConfig.value)[1];
		for (auto& item : (*outConfig.value)[2])
			item = 0.0;
	}
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mv2dHistogramSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mv2dHistogramSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvErrorSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
	(*outConfig.value)[2] = ToDoubleVect(PyTuple_GetItem(inDict, 2));
	(*outConfig.value)[3] = ToDoubleVect(PyTuple_GetItem(inDict, 3));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvHeatSeriesConfig& outConfig)
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
DearPyGui::set_positional_configuration(PyObject* inDict, mvHistogramSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvHistogramSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvPieSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvPieSeries)], inDict))
		return;

	outConfig.x = ToDouble(PyTuple_GetItem(inDict, 0));
	outConfig.y = ToDouble(PyTuple_GetItem(inDict, 1));
	outConfig.radius = ToDouble(PyTuple_GetItem(inDict, 2));
	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 3));
	outConfig.labels = ToStringVect(PyTuple_GetItem(inDict, 4));
	outConfig.clabels.clear();
	for (const auto& label : outConfig.labels)
		outConfig.clabels.push_back(label.c_str());
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvLabelSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvPieSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvImageSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvImageSeries)], inDict))
		return;

	outConfig.textureUUID = GetIDFromPyObject(PyTuple_GetItem(inDict, 0));
	auto resultmin = ToPoint(PyTuple_GetItem(inDict, 1));
	auto resultmax = ToPoint(PyTuple_GetItem(inDict, 2));
	outConfig.bounds_min.x = resultmin.x;
	outConfig.bounds_min.y = resultmin.y;
	outConfig.bounds_max.x = resultmax.x;
	outConfig.bounds_max.y = resultmax.y;
	outConfig._texture = GetRefItem(*GContext->itemRegistry, outConfig.textureUUID);
	if (outConfig._texture)
	{
	}
	else if (outConfig.textureUUID == MV_ATLAS_UUID)
	{
		outConfig._texture = std::make_shared<mvStaticTexture>(outConfig.textureUUID);
		outConfig._internalTexture = true;
	}
	else
		mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(mvAppItemType::mvImageSeries), "Texture not found.", nullptr);
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvSubPlotsConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvSubPlots)], inDict))
		return;

	outConfig.rows = ToInt(PyTuple_GetItem(inDict, 0));
	outConfig.cols = ToInt(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvAreaSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvAreaSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvCandleSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvCandleSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
	(*outConfig.value)[2] = ToDoubleVect(PyTuple_GetItem(inDict, 2));
	(*outConfig.value)[3] = ToDoubleVect(PyTuple_GetItem(inDict, 3));
	(*outConfig.value)[4] = ToDoubleVect(PyTuple_GetItem(inDict, 4));
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvCustomSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvCustomSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
	outConfig.channelCount = ToInt(PyTuple_GetItem(inDict, 2));

	for (int i = 0; i < outConfig.channelCount; i++)
	{
		outConfig._transformedValues.push_back({});
		outConfig._transformedValues.back().resize((*outConfig.value)[i].size());
		memcpy(outConfig._transformedValues.back().data(), (*outConfig.value)[i].data(), sizeof(double) * (*outConfig.value)[i].size());
	}
}

void
DearPyGui::set_required_configuration(PyObject* inDict, mvPlotAxisConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvPlotAxis)], inDict))
		return;

	outConfig.axis = ToInt(PyTuple_GetItem(inDict, 0));
	if (outConfig.axis > 1)
		outConfig.axis = 1;
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
DearPyGui::set_configuration(PyObject* inDict, mvPlotConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x_axis_name")) outConfig.xaxisName = ToString(item);

	// custom input mapping
	if (PyObject* item = PyDict_GetItemString(inDict, "pan_button")) outConfig.pan_button = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "pad_mod")) outConfig.pan_mod = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "fit_button")) outConfig.fit_button = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "context_menu_button")) outConfig.context_menu_button = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "box_select_button")) outConfig.box_select_button = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "box_select_mod")) outConfig.box_select_mod = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "box_select_cancel_button")) outConfig.box_select_cancel_button = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "query_button")) outConfig.query_button = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "query_mod")) outConfig.query_mod = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "query_toggle_mod")) outConfig.query_toggle_mod = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "horizontal_mod")) outConfig.horizontal_mod = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "vertical_mod")) outConfig.vertical_mod = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "use_local_time")) outConfig.localTime = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "use_ISO8601")) outConfig.iSO8601 = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "use_24hour_clock")) outConfig.clock24Hour = ToBool(item);

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// plot flags
	flagop("no_title", ImPlotFlags_NoTitle, outConfig._flags);
	flagop("no_menus", ImPlotFlags_NoMenus, outConfig._flags);
	flagop("no_box_select", ImPlotFlags_NoBoxSelect, outConfig._flags);
	flagop("no_mouse_pos", ImPlotFlags_NoMousePos, outConfig._flags);
	flagop("no_highlight", ImPlotFlags_NoHighlight, outConfig._flags);
	flagop("no_child", ImPlotFlags_NoChild, outConfig._flags);
	flagop("query", ImPlotFlags_Query, outConfig._flags);
	flagop("crosshairs", ImPlotFlags_Crosshairs, outConfig._flags);
	flagop("anti_aliased", ImPlotFlags_AntiAliased, outConfig._flags);
	flagop("equal_aspects", ImPlotFlags_Equal, outConfig._flags);
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
			plot->configData._flags &= ~ImPlotFlags_NoLegend;
		else if (auto plot = static_cast<mvSubPlots*>(item.info.parentPtr))
			plot->removeFlag(ImPlotSubplotFlags_NoLegend);
		item.config.show = true;
	}

	if (item.info.hiddenLastFrame)
	{
		item.info.hiddenLastFrame = false;
		if (auto plot = static_cast<mvPlot*>(item.info.parentPtr))
			plot->configData._flags |= ImPlotFlags_NoLegend;
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

	bool valueChanged = false;
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { valueChanged = true; (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { valueChanged = true; (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y1")) { valueChanged = true; (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y2")) { valueChanged = true; (*outConfig.value)[2] = ToDoubleVect(item); }

	if (valueChanged && outConfig.type == mvAppItemType::mvShadeSeries)
	{
		if ((*outConfig.value)[1].size() != (*outConfig.value)[2].size())
		{
			(*outConfig.value)[2].clear();
			for (size_t i = 0; i < (*outConfig.value)[1].size(); i++)
				(*outConfig.value)[2].push_back(0.0);

		}
	}
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
DearPyGui::set_configuration(PyObject* inDict, mvPieSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "format")) outConfig.format = ToString(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) outConfig.x = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) outConfig.y = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "radius")) outConfig.radius = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "angle")) outConfig.angle = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "normalize")) outConfig.normalize = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "labels"))
	{
		outConfig.labels = ToStringVect(item);
		outConfig.clabels.clear();
		for (const auto& label : outConfig.labels)
			outConfig.clabels.push_back(label.c_str());
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "values")) { (*outConfig.value)[0] = ToDoubleVect(item); }

}

void
DearPyGui::set_configuration(PyObject* inDict, mvLabelSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "vertical")) outConfig.vertical = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "x_offset")) outConfig.xoffset = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "y_offset")) outConfig.yoffset = ToInt(item);

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

}

void
DearPyGui::set_configuration(PyObject* inDict, mvImageSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "uv_min")) outConfig.uv_min = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "uv_max")) outConfig.uv_max = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "tint_color")) outConfig.tintColor = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "bounds_min"))
	{
		auto result = ToPoint(item);
		outConfig.bounds_min.x = result.x;
		outConfig.bounds_min.y = result.y;
	}
	if (PyObject* item = PyDict_GetItemString(inDict, "bounds_max"))
	{
		auto result = ToPoint(item);
		outConfig.bounds_max.x = result.x;
		outConfig.bounds_max.y = result.y;
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "texture_tag"))
	{
		outConfig.textureUUID = GetIDFromPyObject(item);
		outConfig._texture = GetRefItem(*GContext->itemRegistry, outConfig.textureUUID);
		if (outConfig.textureUUID == MV_ATLAS_UUID)
		{
			outConfig._texture = std::make_shared<mvStaticTexture>(outConfig.textureUUID);
			outConfig._internalTexture = true;
		}
		else if (outConfig._texture)
		{
			outConfig._internalTexture = false;
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(mvAppItemType::mvImageSeries), "Texture not found.", nullptr);
		}
	}
}

void
DearPyGui::set_configuration(PyObject* inDict, mvAreaSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "fill")) outConfig.fill = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }
}

void
DearPyGui::set_configuration(PyObject* inDict, mvCandleSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;
	if (PyObject* item = PyDict_GetItemString(inDict, "bull_color")) outConfig.bullColor = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "bear_color")) outConfig.bearColor = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "weight")) outConfig.weight = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "tooltip")) outConfig.tooltip = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "dates")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "opens")) { (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "closes")) { (*outConfig.value)[2] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "lows")) { (*outConfig.value)[3] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "highs")) { (*outConfig.value)[4] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "time_unit")) { outConfig.timeunit = ToUUID(item); }
}

void
DearPyGui::set_configuration(PyObject* inDict, mvCustomSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y1")) { (*outConfig.value)[2] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y2")) { (*outConfig.value)[3] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y3")) { (*outConfig.value)[4] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "tooltip")) { outConfig.tooltip = ToBool(item); }
}

void
DearPyGui::set_configuration(PyObject* inDict, mvAnnotationConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "color")) outConfig.color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "clamped")) outConfig.clamped = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "offset")) outConfig.pixOffset = ToVec2(item);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvSubPlotsConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "rows")) outConfig.rows = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "columns")) outConfig.cols = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "row_ratios")) outConfig.row_ratios = ToFloatVect(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "column_ratios")) outConfig.col_ratios = ToFloatVect(item);

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// subplot flags
	flagop("no_title", ImPlotSubplotFlags_NoTitle, outConfig.flags);
	flagop("no_menus", ImPlotSubplotFlags_NoMenus, outConfig.flags);
	flagop("no_resize", ImPlotSubplotFlags_NoResize, outConfig.flags);
	flagop("no_align", ImPlotSubplotFlags_NoAlign, outConfig.flags);
	flagop("link_rows", ImPlotSubplotFlags_LinkRows, outConfig.flags);
	flagop("link_columns", ImPlotSubplotFlags_LinkCols, outConfig.flags);
	flagop("link_all_x", ImPlotSubplotFlags_LinkAllX, outConfig.flags);
	flagop("link_all_y", ImPlotSubplotFlags_LinkAllY, outConfig.flags);
	flagop("column_major", ImPlotSubplotFlags_ColMajor, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvPlotAxisConfig& outConfig, mvAppItem& item)
{
	if (inDict == nullptr)
		return;

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// axis flags
	flagop("no_gridlines", ImPlotAxisFlags_NoGridLines, outConfig.flags);
	flagop("no_tick_marks", ImPlotAxisFlags_NoTickMarks, outConfig.flags);
	flagop("no_tick_labels", ImPlotAxisFlags_NoTickLabels, outConfig.flags);
	flagop("log_scale", ImPlotAxisFlags_LogScale, outConfig.flags);
	flagop("invert", ImPlotAxisFlags_Invert, outConfig.flags);
	flagop("lock_min", ImPlotAxisFlags_LockMin, outConfig.flags);
	flagop("lock_max", ImPlotAxisFlags_LockMax, outConfig.flags);
	flagop("time", ImPlotAxisFlags_Time, outConfig.flags);

	if (item.info.parentPtr)
	{
		static_cast<mvPlot*>(item.info.parentPtr)->updateFlags();
		static_cast<mvPlot*>(item.info.parentPtr)->updateAxesNames();
	}

	if (item.info.shownLastFrame)
	{
		item.info.shownLastFrame = false;
		if (auto plot = static_cast<mvPlot*>(item.info.parentPtr))
			plot->configData._flags &= ~ImPlotFlags_NoLegend;
		item.config.show = true;
	}

	if (item.info.hiddenLastFrame)
	{
		item.info.hiddenLastFrame = false;
		if (auto plot = static_cast<mvPlot*>(item.info.parentPtr))
			plot->configData._flags |= ImPlotFlags_NoLegend;
		item.config.show = false;
	}
}

void
DearPyGui::fill_configuration_dict(const mvPlotConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "x_axis_name", mvPyObject(ToPyString(inConfig.xaxisName)));
	PyDict_SetItemString(outDict, "pan_button", mvPyObject(ToPyInt(inConfig.pan_button)));
	PyDict_SetItemString(outDict, "pan_mod", mvPyObject(ToPyInt(inConfig.pan_mod)));
	PyDict_SetItemString(outDict, "fit_button", mvPyObject(ToPyInt(inConfig.fit_button)));
	PyDict_SetItemString(outDict, "context_menu_button", mvPyObject(ToPyInt(inConfig.context_menu_button)));
	PyDict_SetItemString(outDict, "box_select_button", mvPyObject(ToPyInt(inConfig.box_select_button)));
	PyDict_SetItemString(outDict, "box_select_mod", mvPyObject(ToPyInt(inConfig.box_select_mod)));
	PyDict_SetItemString(outDict, "box_select_cancel_button", mvPyObject(ToPyInt(inConfig.box_select_cancel_button)));
	PyDict_SetItemString(outDict, "query_button", mvPyObject(ToPyInt(inConfig.query_button)));
	PyDict_SetItemString(outDict, "query_mod", mvPyObject(ToPyInt(inConfig.query_mod)));
	PyDict_SetItemString(outDict, "query_toggle_mod", mvPyObject(ToPyInt(inConfig.query_toggle_mod)));
	PyDict_SetItemString(outDict, "horizontal_mod", mvPyObject(ToPyInt(inConfig.horizontal_mod)));
	PyDict_SetItemString(outDict, "vertical_mod", mvPyObject(ToPyInt(inConfig.vertical_mod)));
	PyDict_SetItemString(outDict, "use_local_time", mvPyObject(ToPyBool(inConfig.localTime)));
	PyDict_SetItemString(outDict, "use_ISO8601", mvPyObject(ToPyBool(inConfig.iSO8601)));
	PyDict_SetItemString(outDict, "use_24hour_clock", mvPyObject(ToPyBool(inConfig.clock24Hour)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// plot flags
	checkbitset("no_title", ImPlotFlags_NoTitle, inConfig._flags);
	checkbitset("no_menus", ImPlotFlags_NoMenus, inConfig._flags);
	checkbitset("no_box_select", ImPlotFlags_NoBoxSelect, inConfig._flags);
	checkbitset("no_mouse_pos", ImPlotFlags_NoMousePos, inConfig._flags);
	checkbitset("no_highlight", ImPlotFlags_NoHighlight, inConfig._flags);
	checkbitset("no_child", ImPlotFlags_NoChild, inConfig._flags);
	checkbitset("query", ImPlotFlags_Query, inConfig._flags);
	checkbitset("crosshairs", ImPlotFlags_Crosshairs, inConfig._flags);
	checkbitset("anti_aliased", ImPlotFlags_AntiAliased, inConfig._flags);
	checkbitset("equal_aspects", ImPlotFlags_Equal, inConfig._flags);
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
DearPyGui::fill_configuration_dict(const mvPieSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "format", mvPyObject(ToPyString(inConfig.format)));
	PyDict_SetItemString(outDict, "x", mvPyObject(ToPyDouble(inConfig.x)));
	PyDict_SetItemString(outDict, "y", mvPyObject(ToPyDouble(inConfig.y)));
	PyDict_SetItemString(outDict, "radius", mvPyObject(ToPyDouble(inConfig.radius)));
	PyDict_SetItemString(outDict, "angle", mvPyObject(ToPyDouble(inConfig.angle)));
	PyDict_SetItemString(outDict, "normalize", mvPyObject(ToPyBool(inConfig.normalize)));
	PyDict_SetItemString(outDict, "labels", mvPyObject(ToPyList(inConfig.labels)));
}

void
DearPyGui::fill_configuration_dict(const mvLabelSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "vertical", mvPyObject(ToPyBool(inConfig.vertical)));
	PyDict_SetItemString(outDict, "x_offset", mvPyObject(ToPyInt(inConfig.xoffset)));
	PyDict_SetItemString(outDict, "y_offset", mvPyObject(ToPyInt(inConfig.yoffset)));
}

void
DearPyGui::fill_configuration_dict(const mvImageSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
	PyDict_SetItemString(outDict, "texture_tag", mvPyObject(ToPyUUID(inConfig.textureUUID)));
	PyDict_SetItemString(outDict, "uv_min", mvPyObject(ToPyPair(inConfig.uv_min.x, inConfig.uv_min.y)));
	PyDict_SetItemString(outDict, "uv_max", mvPyObject(ToPyPair(inConfig.uv_max.x, inConfig.uv_max.y)));
	PyDict_SetItemString(outDict, "tint_color", mvPyObject(ToPyColor(inConfig.tintColor)));
	PyDict_SetItemString(outDict, "bounds_min", mvPyObject(ToPyPair(inConfig.bounds_min.x, inConfig.bounds_min.y)));
	PyDict_SetItemString(outDict, "bounds_max", mvPyObject(ToPyPair(inConfig.bounds_max.x, inConfig.bounds_max.y)));
}

void
DearPyGui::fill_configuration_dict(const mvAreaSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
	PyDict_SetItemString(outDict, "fill", mvPyObject(ToPyColor(inConfig.fill)));
}

void
DearPyGui::fill_configuration_dict(const mvCandleSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
	PyDict_SetItemString(outDict, "bull_color", mvPyObject(ToPyColor(inConfig.bullColor)));
	PyDict_SetItemString(outDict, "bear_color", mvPyObject(ToPyColor(inConfig.bearColor)));
	PyDict_SetItemString(outDict, "weight",     mvPyObject(ToPyFloat(inConfig.weight)));
	PyDict_SetItemString(outDict, "tooltip",    mvPyObject(ToPyBool(inConfig.tooltip)));
	PyDict_SetItemString(outDict, "time_unit",  mvPyObject(ToPyLong(inConfig.timeunit)));
}

void
DearPyGui::fill_configuration_dict(const mvCustomSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "channel_count", mvPyObject(ToPyInt(inConfig.channelCount)));
	PyDict_SetItemString(outDict, "tooltip", mvPyObject(ToPyBool(inConfig.tooltip)));
}

void
DearPyGui::fill_configuration_dict(const mvAnnotationConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
	PyDict_SetItemString(outDict, "color", mvPyObject(ToPyColor(inConfig.color)));
	PyDict_SetItemString(outDict, "clamped", mvPyObject(ToPyBool(inConfig.clamped)));
	PyDict_SetItemString(outDict, "offset", mvPyObject(ToPyPair(inConfig.pixOffset.x, inConfig.pixOffset.y)));
}

void
DearPyGui::fill_configuration_dict(const mvSubPlotsConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "rows", mvPyObject(ToPyInt(inConfig.rows)));
	PyDict_SetItemString(outDict, "columns", mvPyObject(ToPyInt(inConfig.cols)));
	// maybe remove this on next minor release?
	PyDict_SetItemString(outDict, "cols", mvPyObject(ToPyInt(inConfig.cols)));
	PyDict_SetItemString(outDict, "row_ratios", mvPyObject(ToPyList(inConfig.row_ratios)));
	PyDict_SetItemString(outDict, "column_ratios", mvPyObject(ToPyList(inConfig.col_ratios)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// subplot flags
	checkbitset("no_title", ImPlotSubplotFlags_NoTitle, inConfig.flags);
	checkbitset("no_menus", ImPlotSubplotFlags_NoMenus, inConfig.flags);
	checkbitset("no_resize", ImPlotSubplotFlags_NoResize, inConfig.flags);
	checkbitset("no_align", ImPlotSubplotFlags_NoAlign, inConfig.flags);
	checkbitset("link_rows", ImPlotSubplotFlags_LinkRows, inConfig.flags);
	checkbitset("link_columns", ImPlotSubplotFlags_LinkCols, inConfig.flags);
	checkbitset("link_all_x", ImPlotSubplotFlags_LinkAllX, inConfig.flags);
	checkbitset("link_all_y", ImPlotSubplotFlags_LinkAllY, inConfig.flags);
	checkbitset("column_major", ImPlotSubplotFlags_ColMajor, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvPlotAxisConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// plot flags
	checkbitset("no_gridlines", ImPlotAxisFlags_NoGridLines, inConfig.flags);
	checkbitset("no_tick_marks", ImPlotAxisFlags_NoTickMarks, inConfig.flags);
	checkbitset("no_tick_labels", ImPlotAxisFlags_NoTickLabels, inConfig.flags);
	checkbitset("log_scale", ImPlotAxisFlags_LogScale, inConfig.flags);
	checkbitset("invert", ImPlotAxisFlags_Invert, inConfig.flags);
	checkbitset("lock_min", ImPlotAxisFlags_LockMin, inConfig.flags);
	checkbitset("lock_max", ImPlotAxisFlags_LockMax, inConfig.flags);
	checkbitset("time", ImPlotAxisFlags_Time, inConfig.flags);
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

void mvAnnotation::setPyValue(PyObject* value)
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

void mvPlot::updateFlags()
{
	for (size_t i = 0; i < childslots[1].size(); i++)
	{
		auto child = static_cast<mvPlotAxis*>(childslots[1][i].get());
		switch (i)
		{
		case(0): configData._xflags = child->configData.flags; break;
		case(1): configData._yflags = child->configData.flags; break;
		case(2): configData._y1flags = child->configData.flags; if (child->config.show) configData._flags |= ImPlotFlags_YAxis2; else configData._flags &= ~ImPlotFlags_YAxis2; break;
		case(3): configData._y2flags = child->configData.flags; if (child->config.show) configData._flags |= ImPlotFlags_YAxis3; else configData._flags &= ~ImPlotFlags_YAxis3; break;
		default: configData._yflags = child->configData.flags; break;
		}
	}

}

void mvPlot::updateAxesNames()
{
	configData.xaxisName.clear();
	configData._y1axisName.clear();
	configData._y2axisName.clear();
	configData._y3axisName.clear();

	for (size_t i = 0; i < childslots[1].size(); i++)
	{
		auto axis = childslots[1][i].get();
		switch (i)
		{
		case(0): configData.xaxisName = axis->config.specifiedLabel; break;
		case(1): configData._y1axisName = axis->config.specifiedLabel; break;
		case(2): configData._y2axisName = axis->config.specifiedLabel; break;
		case(3): configData._y3axisName = axis->config.specifiedLabel; break;
		default: configData._y1axisName = axis->config.specifiedLabel; break;
		}
	}

}