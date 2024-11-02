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
		ImPlotRect limits = ImPlot::GetPlotLimits();
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
			if(ImGui::BeginTooltip()) {
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
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvAxisTagConfig& outConfig)
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
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvDragRectConfig& outConfig)
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
	outConfig.value = *static_cast<std::shared_ptr<std::array<double, 2>>*>(item.getValue());
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
	ImPlot::GetInputMap().Pan = config.pan;
	ImPlot::GetInputMap().Fit = config.fit;
	ImPlot::GetInputMap().Select = config.select;
	ImPlot::GetInputMap().SelectCancel = config.select_cancel;
	ImPlot::GetInputMap().Menu = config.menu;
	ImPlot::GetInputMap().ZoomRate = config.zoom_rate;

	if (config.pan_mod != ImPlot::GetInputMap().PanMod) ImPlot::GetInputMap().PanMod = config.pan_mod;
	if (config.select_mod != ImPlot::GetInputMap().SelectMod) ImPlot::GetInputMap().SelectMod = config.select_mod;
	if (config.zoom_mod != ImPlot::GetInputMap().ZoomMod) ImPlot::GetInputMap().ZoomMod = config.zoom_mod;
	if (config.override_mod != ImPlot::GetInputMap().OverrideMod) ImPlot::GetInputMap().OverrideMod = config.override_mod;
	if (config.select_horz_mod != ImPlot::GetInputMap().SelectHorzMod) ImPlot::GetInputMap().SelectHorzMod = config.select_horz_mod;
	if (config.select_vert_mod != ImPlot::GetInputMap().SelectVertMod) ImPlot::GetInputMap().SelectVertMod = config.select_vert_mod;

	if (config._fitDirty)
	{
		// This must be called before BeginPlot
		for(int i = 0; i < ImAxis_COUNT; i++) {
			if (config._axisfitDirty[i] == true) {
				ImPlot::SetNextAxisToFit(i);
				config._axisfitDirty[i] = false;
			}
		}
		config._fitDirty = false;
	}

	if (ImPlot::BeginPlot(item.info.internalLabel.c_str(), ImVec2((float)item.config.width, (float)item.config.height), config._flags))
	{	

		// gives axes change to make changes to ticks, limits, etc.
		ImAxis next_y_axis = ImAxis_Y1;
		for (auto& child : item.childslots[1])
		{
			// skip item if it's not shown
			if (!child->config.show)
				continue;

			if (child->type == mvAppItemType::mvPlotAxis)
			{
				mvPlotAxis* axis = static_cast<mvPlotAxis*>(child.get());
				ImAxis_ id_axis = static_cast<ImAxis_>(axis->configData.axis);

				// auto-assigning additional Y axes for compatibility with DPG 1.11 and earlier versions
				auto flags = axis->configData.flags;
				if (id_axis == ImAxis_Y1)
				{
					if (axis->configData.axis < next_y_axis)
					{
						id_axis = static_cast<ImAxis_>(next_y_axis);
						flags |= ImPlotAxisFlags_Opposite;
					}
					++next_y_axis;
				}

				ImPlot::SetupAxis(id_axis, axis->config.specifiedLabel.c_str(), flags);
				if (axis->configData.setLimits || axis->configData._dirty)
				{
					ImPlot::SetupAxisLimits(id_axis, axis->configData.limits.Min, axis->configData.limits.Max, ImGuiCond_Always);
					axis->configData._dirty = false;  // TODO: Check if this is it really useful
				}
				if (!axis->configData.formatter.empty())
					ImPlot::SetupAxisFormat(id_axis, axis->configData.formatter.c_str());

				ImPlot::SetupAxisScale(id_axis, axis->configData.scale);

				if (axis->configData.setLimitsRange)
					ImPlot::SetupAxisLimitsConstraints(id_axis, axis->configData.constraints_range.Min, axis->configData.constraints_range.Max);
				if (axis->configData.setZoomRange)
					ImPlot::SetupAxisZoomConstraints(id_axis, axis->configData.zoom_range.x, axis->configData.zoom_range.y);

				if (!axis->configData.labels.empty())
				{
					// TODO: Checks (from original dpg)
					ImPlot::SetupAxisTicks(id_axis, axis->configData.labelLocations.data(), (int)axis->configData.labels.size(), axis->configData.clabels.data());
				}
			}
			else
				child->customAction();
		}
		auto context = ImPlot::GetCurrentContext();

		ImGuiIO& IO = ImGui::GetIO();
		// Note: we can't use `config.querying` here because in the frame when
		// the query modifier gets pressed, `querying` is still false but we already
		// need to disable `OverrideMod`.
		if (ImHasFlag(IO.KeyMods, config.query_toggle_mod) &&
			(ImGui::IsMouseDown(config.select) || ImGui::IsMouseReleased(config.select)))
		{
			// Preventing ImPlot from getting stuck on selection if override modifier
			// is pressed (e.g. when the override mod is the same as query toggle mod).
			ImPlot::GetInputMap().OverrideMod = ImGuiMod_None;
		}
		else
			ImPlot::GetInputMap().OverrideMod = config.override_mod;

		bool query_dirty = false;
		if (config.query_enabled && config.querying && ImGui::IsMouseReleased(config.select))
		{
			if (config.max_query_rects != 0 && config.rects.size() >= config.max_query_rects)
				config.rects.pop_back();
			config.rects.push_back(config.query_rect);
			config.querying = false;
			// Prevent ImPlot from handling mouse release on its own. This will block
			// input handling in the current frame (later we'll reset OverrideMod).
			ImPlot::GetInputMap().OverrideMod = IO.KeyMods;
			// Note: this will lock the setup and might therefore skip changes
			// to the legend, drag points, and lines in this frame.  Nothing we
			// can do about that, really.
			ImPlot::CancelPlotSelection();
			// We've updated the list, let's report this
			query_dirty = true;
		}

		// legend, drag point and lines
		for (auto& child : item.childslots[0]) // Using "ImPlot::GetPlotPos()" here trigger an assert
			child->draw(drawlist, context->CurrentPlot->PlotRect.Min.x, context->CurrentPlot->PlotRect.Min.y);

		// axes
		for (auto& child : item.childslots[1])
			child->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

		ImPlot::PushPlotClipRect();

		ImPlot::SetAxis(ImAxis_Y1);

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

		config.querying = ImHasFlag(IO.KeyMods, config.query_toggle_mod) && ImPlot::IsPlotSelected();
		if (config.querying)
			config.query_rect = ImPlot::GetPlotSelection();

		// While rendering query rects, we'll see which of them the user asks to
		// delete (by double-clicking it).  We need to run through the entire list
		// to make sure that we pick the topmost candidate if there's more than one.
		int delete_idx = -1;
        for (int i = 0; i < config.rects.size(); ++i) {
			// TODO: Implement flags
			bool hovered = false;
			query_dirty |= ImPlot::DragRect(i,&config.rects[i].X.Min,&config.rects[i].Y.Min,&config.rects[i].X.Max,&config.rects[i].Y.Max, config.query_color, ImPlotDragToolFlags_NoFit, nullptr, &hovered);
			if (config.rects.size() > config.min_query_rects) {
				if (hovered && ImGui::IsMouseDoubleClicked(config.select_cancel))
				{
					// remember it for future deletion
					delete_idx = i;
				}
			}
        }
		// Delete rect on double click.
		// We're not interested in double-clicks that modify a query rect
		// (in particular, double-clicks on rect edges), and to filter them out,
		// we additionally check for `query_dirty` to be false.
		if (delete_idx >= 0 && !query_dirty)
		{
			config.rects.erase(config.rects.begin() + delete_idx);
			// Preventing plot auto-fit if it uses the same mouse button.
			// Kind of a dirty trick but double-click has already set
			// all `FitThisFrame` to true anyway.
			if (config.fit == config.select_cancel)
			{
				context->CurrentPlot->FitThisFrame = false;
				for (int j = 0; j < ImAxis_COUNT; ++j)
					context->CurrentPlot->Axes[j].FitThisFrame = false;
			}
			// We've updated the list, let's report this
			query_dirty = true;
		}

		if (item.config.callback != nullptr && query_dirty)
		{
			if (item.config.alias.empty()) {
				mvSubmitCallback([=, &item]() {
					PyObject* result = PyTuple_New(config.rects.size());
					for (int i = 0; i < config.rects.size(); ++i) {
						auto rectMin = config.rects[i].Min();
						auto rectMax = config.rects[i].Max();
						PyTuple_SetItem(result, i, Py_BuildValue("(dddd)", rectMin.x, rectMin.y, rectMax.x, rectMax.y));
					}
					mvAddCallback(item.config.callback, item.uuid, result, item.config.user_data);
				});
			} else {
				mvSubmitCallback([=, &item]() {
					PyObject* result = PyTuple_New(config.rects.size());
					for (int i = 0; i < config.rects.size(); ++i) {
						auto rectMin = config.rects[i].Min();
						auto rectMax = config.rects[i].Max();
						PyTuple_SetItem(result, i, Py_BuildValue("(dddd)", rectMin.x, rectMin.y, rectMax.x, rectMax.y));
					}
					mvAddCallback(item.config.callback, item.config.alias, result, item.config.user_data);
				});
			}
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
			if (ImPlot::BeginDragDropTargetPlot())
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
			ImVec2 windowPos = ImGui::GetWindowPos();
			GContext->input.mousePos.x = (int)(mousePos.x - windowPos.x);
			GContext->input.mousePos.y = (int)(mousePos.y - windowPos.y);

			GContext->activeWindow = item.uuid;

		}

		// TODO: find a better way to handle this
		// We could use std::find_if from <algorithm> but it'd require item.childslots[0] to be a std::vector
		for (auto& child : item.childslots[0])
		{
			if (child->type == mvAppItemType::mvPlotLegend)
			{
				auto legend = static_cast<mvPlotLegend*>(child.get());
				legend->configData.legendLocation = context->CurrentPlot->Items.Legend.Location;
				legend->configData.flags = context->CurrentPlot->Items.Legend.Flags;
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
	ImPlot::SetAxis(config.axis);

	for (auto& item : item.childslots[1])
		item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

	// x axis
	if (config.axis <= ImAxis_X3)
		config.limits_actual = ImPlot::GetPlotLimits(config.axis, IMPLOT_AUTO).X;

	// y axis
	else
		config.limits_actual = ImPlot::GetPlotLimits(config.axis, IMPLOT_AUTO).Y;

	config.flags = ImPlot::GetCurrentContext()->CurrentPlot->Axes[config.axis].Flags;

	UpdateAppItemState(item.state);

	if (item.font)
		ImGui::PopFont();

	if (item.theme)
		static_cast<mvTheme*>(item.theme.get())->customAction();

	if (item.config.dropCallback)
	{
		ScopedID id(item.uuid);
		if (ImPlot::BeginDragDropTargetAxis(config.axis))
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
		ImPlot::SetupLegend(config.legendLocation, config.flags);
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

	bool hovered = false;
	bool held = false;

	if (config.vertical)
	{
		if (ImPlot::DragLineX(item.uuid, config.value.get(), config.color, config.thickness, config.flags, nullptr, &hovered, &held))
		{
			mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
		}
		if (config.show_label && !item.config.specifiedLabel.empty() && (hovered || held)) {
            char buff[IMPLOT_LABEL_MAX_SIZE];
			ImPlotContext& gp = *GImPlot;
			ImPlotAxis& axis = gp.CurrentPlot->Axes[gp.CurrentPlot->CurrentX];
			auto pos = *config.value.get();
    		ImPlot::LabelAxisValue(axis, pos, buff, sizeof(buff), true);
			ImVec4 color = ImPlot::IsColorAuto(config.color.toVec4()) ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : config.color;
			ImPlot::Annotation(pos, ImPlot::GetPlotLimits().Min().y, color, ImVec2(0, 0), true, "%s = %s", item.config.specifiedLabel.c_str(), buff);
		}
	}
	else
	{
		if (ImPlot::DragLineY(item.uuid, config.value.get(), config.color, config.thickness, config.flags, nullptr, &hovered, &held))
		{
			mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
		}
		if (config.show_label && !item.config.specifiedLabel.empty() && (hovered || held)) {
            char buff[IMPLOT_LABEL_MAX_SIZE];
			ImPlotContext& gp = *GImPlot;
			ImPlotAxis& axis = gp.CurrentPlot->Axes[gp.CurrentPlot->CurrentY];
			auto label_pos = *config.value.get();
    		ImPlot::LabelAxisValue(axis, label_pos, buff, sizeof(buff), true);
			ImVec4 color = ImPlot::IsColorAuto(config.color.toVec4()) ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : config.color;
			ImPlot::Annotation(ImPlot::GetPlotLimits().Min().x, label_pos, color, ImVec2(0, 0), true, "%s = %s", item.config.specifiedLabel.c_str(), buff);
		}
	}
}

void
DearPyGui::draw_plot_tag(ImDrawList* drawlist, mvAppItem& item, mvAxisTagConfig& config)
{
	if (!item.config.show)
		return;

	auto parent = (mvPlotAxis*)item.info.parentPtr;
	auto axis_id = parent->configData.axis;
	const bool vertical = (axis_id >= ImAxis_Y1);

	if (vertical) {
		if (!item.config.specifiedLabel.empty()) {
			ImPlot::TagY(*config.value.get(), config.color, "%s", item.config.specifiedLabel.c_str());	
		} else {
			ImPlot::TagY(*config.value.get(), config.color, config.auto_rounding);
		}
	}
	else {
		if (!item.config.specifiedLabel.empty()) {
			ImPlot::TagX(*config.value.get(), config.color, "%s", item.config.specifiedLabel.c_str());	
		} else {
			ImPlot::TagX(*config.value.get(), config.color, config.auto_rounding);
		}
	}
}

void
DearPyGui::draw_drag_rect(ImDrawList* drawlist, mvAppItem& item, mvDragRectConfig& config)
{
	if (!item.config.show)
		return;

	ScopedID id(item.uuid);

	static double xmin = (*config.value.get())[0];
	static double ymin = (*config.value.get())[1];
	static double xmax = (*config.value.get())[2];
	static double ymax = (*config.value.get())[3];
	// I still don't get why we need to do this
	xmin = (*config.value.get())[0];
	ymin = (*config.value.get())[1];
	xmax = (*config.value.get())[2];
	ymax = (*config.value.get())[3];

	// item.config.specifiedLabel.c_str(),
	if (ImPlot::DragRect(item.uuid, &xmin, &ymin, &xmax, &ymax, config.color, config.flags))
	{
		(*config.value.get())[0] = xmin;
		(*config.value.get())[1] = ymin;
		(*config.value.get())[2] = xmax;
		(*config.value.get())[3] = ymax;
		mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
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

	bool hovered = false;
	bool held = false;
	if (ImPlot::DragPoint(item.uuid, &dummyx, &dummyy, config.color, config.radius, config.flags, nullptr, &hovered, &held))
	{
		(*config.value.get())[0] = dummyx;
		(*config.value.get())[1] = dummyy;
		mvAddCallback(item.config.callback, item.uuid, nullptr, item.config.user_data);
	}
	if (config.show_label && !item.config.specifiedLabel.empty() && (hovered || held)) {
		ImPlotContext& gp = *GImPlot;
		char x_buff[IMPLOT_LABEL_MAX_SIZE];
		ImPlotAxis& x_axis = gp.CurrentPlot->Axes[gp.CurrentPlot->CurrentX];
		ImPlot::LabelAxisValue(x_axis, dummyx, x_buff, sizeof(x_buff), true);
		char y_buff[IMPLOT_LABEL_MAX_SIZE];
		ImPlotAxis& y_axis = gp.CurrentPlot->Axes[gp.CurrentPlot->CurrentY];
		ImPlot::LabelAxisValue(y_axis, dummyy, y_buff, sizeof(y_buff), true);
		ImVec4 color = ImPlot::IsColorAuto(config.color.toVec4()) ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : config.color;
		ImPlot::Annotation(dummyx, dummyy, color, config.pixOffset, config.clamped, "%s = %s, %s", item.config.specifiedLabel.c_str(), x_buff, y_buff);
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

		ImPlot::PlotBars(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.weight, config.flags);

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
DearPyGui::draw_bar_group_series(ImDrawList* drawlist, mvAppItem& item, const mvBarGroupSeriesConfig& config)
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

		static const std::vector<double>* values;

		values = &(*config.value.get())[0];

		// item.info.internalLabel.c_str()
		std::vector<const char*> strings;
		for (int i = 0; i < config.label_ids.size(); ++i)
			strings.push_back(config.label_ids[i].c_str());

		const int item_count = int(values->size() / config.group_size);

		ImPlot::PlotBarGroups(strings.data(), values->data(), config.group_size, item_count, config.group_width, config.shift, config.flags);

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
DearPyGui::draw_line_series(ImDrawList* drawlist, mvAppItem& item, const mvLineSeriesConfig& config)
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

		ImPlot::PlotLine(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.flags);

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
DearPyGui::draw_scatter_series(ImDrawList* drawlist, mvAppItem& item, const mvScatterSeriesConfig& config)
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

		ImPlot::PlotScatter(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.flags);

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
DearPyGui::draw_stair_series(ImDrawList* drawlist, mvAppItem& item, const mvStairSeriesConfig& config)
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

		ImPlot::PlotStairs(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.flags);

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
DearPyGui::draw_stem_series(ImDrawList* drawlist, mvAppItem& item, const mvStemSeriesConfig& config)
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

		ImPlot::PlotStems(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), 0.0, config.flags);

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
DearPyGui::draw_shade_series(ImDrawList* drawlist, mvAppItem& item, const mvShadeSeriesConfig& config)
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
			y2ptr->data(), (int)xptr->size(), config.flags);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(item.info.internalLabel.c_str(), ImGuiMouseButton_Right))
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
DearPyGui::draw_inf_lines_series(ImDrawList* drawlist, mvAppItem& item, const mvInfLineSeriesConfig& config)
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

		ImPlot::PlotInfLines(item.info.internalLabel.c_str(), xptr->data(), (int)xptr->size(), config.flags);

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
			config.xbins, config.ybins, ImPlotRect(config.xmin, config.xmax, config.ymin, config.ymax), config.flags);

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

		ImPlot::PlotErrorBars(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size(), config.flags);

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
			config.format.c_str(), { config.bounds_min.x, config.bounds_min.y }, { config.bounds_max.x, config.bounds_max.y }, config.flags);

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

		ImPlotRange range = ImPlotRange();

		if(config.min != 0.0f || config.max != 0.0f){
			range = ImPlotRange(config.min, config.max);
		}

		ImPlot::PlotHistogram(item.info.internalLabel.c_str(), xptr->data(), (int)xptr->size(), config.bins,
			(double)config.barScale, range, config.flags);

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
DearPyGui::draw_digital_series(ImDrawList* drawlist, mvAppItem& item, const mvDigitalSeriesConfig& config)
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

		ImPlot::PlotDigital(item.info.internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(), config.flags, 0);
		
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
			config.x, config.y, config.radius, config.format.c_str(), config.angle, config.flags);

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

	static const std::vector<double>* xptr;
	static const std::vector<double>* yptr;

	xptr = &(*config.value.get())[0];
	yptr = &(*config.value.get())[1];

	if (xptr->size() == 0 || yptr->size() == 0) {
		return;
	}

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

		ImPlot::PlotText(item.config.specifiedLabel.c_str(), (*xptr)[0], (*yptr)[0], config.offset, config.flags);

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

			ImPlot::PlotImage(item.info.internalLabel.c_str(), texture, config.bounds_min, config.bounds_max, config.uv_min, config.uv_max, config.tintColor, config.flags);

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
		ImPlot::RegisterOrGetItem(item.info.internalLabel.c_str(), ImPlotItemFlags_None);
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
		// TODO: (Maybe) Update this to reflect nex axes
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
			if(ImGui::BeginTooltip()) {
				for (auto& item : item.childslots[1])
					item->draw(draw_list, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
				ImGui::EndTooltip();
			}
		}

		// begin plot item
		if (ImPlot::BeginItem(item.info.internalLabel.c_str(), config.flags))
		{
			// override legend icon color
			ImPlot::GetCurrentItem()->Color = ImGui::ColorConvertFloat4ToU32({ 0.25f, 0.25f, 0.25f, 1.0f });

			// fit data if requested
			if (ImPlot::FitThisFrame() && !ImHasFlag(config.flags, ImPlotItemFlags_NoFit))
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
				PyDict_SetItemString(helperData, "MouseX_PlotSpace", ToPyDouble(mouse.x));
				PyDict_SetItemString(helperData, "MouseY_PlotSpace", ToPyDouble(mouse.y));
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

	ImPlot::Annotation((*config.value.get())[0], (*config.value.get())[1], config.color.toVec4(), config.pixOffset, config.clamped, "%s", item.config.specifiedLabel.c_str());
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvLineSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvLineSeries)], inDict))
		return;

	for(int i = 0; i < PyTuple_Size(inDict); i++)
		(*outConfig.value)[i] = ToDoubleVect(PyTuple_GetItem(inDict, i));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvDigitalSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvDigitalSeries)], inDict))
		return;

	for(int i = 0; i < PyTuple_Size(inDict); i++)
		(*outConfig.value)[i] = ToDoubleVect(PyTuple_GetItem(inDict, i));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvBarSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

static bool ValidateBarGroupConfig(mvBarGroupSeriesConfig& outConfig) 
{
	if (outConfig.group_size == 0)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "draw_bar_group_series", "`group_size` can't be 0", nullptr);
		return false;
	}
	const std::vector<double>* values = &(*outConfig.value.get())[0];
	const size_t values_size = values->size();
	const int item_count = values_size / outConfig.group_size;

	if (values_size % outConfig.group_size != 0)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "draw_bar_group_series",
			"`values` size " + std::to_string(values_size) + " must be a multiple of `group_size` " + std::to_string(outConfig.group_size), nullptr);
		return false;
	}
	if (outConfig.label_ids.size() != outConfig.group_size) 
	{
		mvThrowPythonError(mvErrorCode::mvNone, "draw_bar_group_series",
			"The number of labels " + std::to_string(outConfig.label_ids.size()) + " must be equal to the number of items in a group " + std::to_string(outConfig.group_size) , nullptr);
		return false;
	}
	return true;
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvBarGroupSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvBarGroupSeries)], inDict))
		return;

	auto backup_value = (*outConfig.value)[0];
	auto backup_label_ids = outConfig.label_ids;
	int backup_group_size = outConfig.group_size;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	outConfig.label_ids = ToStringVect(PyTuple_GetItem(inDict, 1));
	outConfig.group_size = ToInt(PyTuple_GetItem(inDict, 2));

	if(!ValidateBarGroupConfig(outConfig)) {
		(*outConfig.value)[0] = backup_value;
		outConfig.label_ids = backup_label_ids;
		outConfig.group_size = backup_group_size;
	}
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvStairSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvStairSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
	(*outConfig.value)[1] = ToDoubleVect(PyTuple_GetItem(inDict, 1));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvInfLineSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvInfLineSeries)], inDict))
		return;

	(*outConfig.value)[0] = ToDoubleVect(PyTuple_GetItem(inDict, 0));
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvShadeSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvShadeSeries)], inDict))
		return;

	for(int i = 0; i < PyTuple_Size(inDict); i++)
		(*outConfig.value)[i] = ToDoubleVect(PyTuple_GetItem(inDict, i));

	(*outConfig.value)[2] = (*outConfig.value)[1];
	for (auto& item : (*outConfig.value)[2])
		item = 0.0;
}

void
DearPyGui::set_positional_configuration(PyObject* inDict, mvStemSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvStemSeries)], inDict))
		return;

	for(int i = 0; i < PyTuple_Size(inDict); i++)
		(*outConfig.value)[i] = ToDoubleVect(PyTuple_GetItem(inDict, i));
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
DearPyGui::set_positional_configuration(PyObject* inDict, mvScatterSeriesConfig& outConfig)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvScatterSeries)], inDict))
		return;

	for(int i = 0; i < PyTuple_Size(inDict); i++)
		(*outConfig.value)[i] = ToDoubleVect(PyTuple_GetItem(inDict, i));
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
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(mvAppItemType::mvLabelSeries)], inDict))
		return;

	(*outConfig.value)[0] = std::vector<double>{ToDouble(PyTuple_GetItem(inDict, 0))};
	(*outConfig.value)[1] = std::vector<double>{ToDouble(PyTuple_GetItem(inDict, 1))};
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

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// drag line flags
	flagop("delayed", ImPlotDragToolFlags_Delayed, outConfig.flags);
	flagop("no_cursor", ImPlotDragToolFlags_NoCursors, outConfig.flags);
	flagop("no_fit", ImPlotDragToolFlags_NoFit, outConfig.flags);
	flagop("no_inputs", ImPlotDragToolFlags_NoInputs, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvDragRectConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "color")) outConfig.color = ToColor(item);

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// drag rect flags
	flagop("delayed", ImPlotDragToolFlags_Delayed, outConfig.flags);
	flagop("no_cursor", ImPlotDragToolFlags_NoCursors, outConfig.flags);
	flagop("no_fit", ImPlotDragToolFlags_NoFit, outConfig.flags);
	flagop("no_inputs", ImPlotDragToolFlags_NoInputs, outConfig.flags);

}

void
DearPyGui::set_configuration(PyObject* inDict, mvPlotConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "pan_button")) outConfig.pan = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "fit_button")) outConfig.fit = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "context_menu_button")) outConfig.menu = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "box_select_button")) outConfig.select = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "box_select_cancel_button")) outConfig.select_cancel = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "box_select_mod_button")) outConfig.select_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "query_toggle_mod")) outConfig.query_toggle_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "horizontal_mod")) outConfig.select_horz_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "vertical_mod")) outConfig.select_vert_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "use_local_time")) outConfig.localTime = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "use_ISO8601")) outConfig.iSO8601 = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "use_24hour_clock")) outConfig.clock24Hour = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "pan_mod")) outConfig.pan_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "override_mod")) outConfig.override_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "zoom_mod")) outConfig.zoom_mod = static_cast<ImGuiKey>(ToInt(item));
	if (PyObject* item = PyDict_GetItemString(inDict, "zoom_rate")) outConfig.zoom_rate = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "query")) outConfig.query_enabled = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "query_color")) outConfig.query_color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "min_query_rects")) outConfig.min_query_rects = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "max_query_rects")) outConfig.max_query_rects = ToInt(item);
	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// plot flags
	flagop("no_title", ImPlotFlags_NoTitle, outConfig._flags);
	flagop("no_menus", ImPlotFlags_NoMenus, outConfig._flags);
	flagop("no_box_select", ImPlotFlags_NoBoxSelect, outConfig._flags);
	flagop("no_mouse_pos", ImPlotFlags_NoMouseText, outConfig._flags);
	flagop("crosshairs", ImPlotFlags_Crosshairs, outConfig._flags);
	flagop("equal_aspects", ImPlotFlags_Equal, outConfig._flags);
	flagop("no_inputs", ImPlotFlags_NoInputs, outConfig._flags);
	flagop("no_frame", ImPlotFlags_NoFrame, outConfig._flags);	
	// flagop("canvas_only", ImPlotFlags_CanvasOnly, outConfig._flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvDragPointConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "color")) outConfig.color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "radius")) outConfig.radius = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "show_label")) outConfig.show_label = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "clamped")) outConfig.clamped = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "offset")) outConfig.pixOffset = ToVec2(item);

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// drag point flags
	flagop("delayed", ImPlotDragToolFlags_Delayed, outConfig.flags);
	flagop("no_cursor", ImPlotDragToolFlags_NoCursors, outConfig.flags);
	flagop("no_fit", ImPlotDragToolFlags_NoFit, outConfig.flags);
	flagop("no_inputs", ImPlotDragToolFlags_NoInputs, outConfig.flags);

}

void

DearPyGui::set_configuration(PyObject* inDict, mvPlotLegendConfig& outConfig, mvAppItem& item)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "location")) { outConfig.legendLocation = ToInt(item); outConfig.dirty = true; }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, mvPlotLegendConfig& outConfig)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) { 
			ToBool(item) ? outConfig.flags |= flag : outConfig.flags &= ~flag; 
			outConfig.dirty = true; 
		}
	};

	// plot flags
	flagop("no_buttons", ImPlotLegendFlags_NoButtons, outConfig);
	flagop("no_highlight_item", ImPlotLegendFlags_NoHighlightItem, outConfig);
	flagop("no_highlight_axis", ImPlotLegendFlags_NoHighlightAxis, outConfig);
	flagop("no_menus", ImPlotLegendFlags_NoMenus, outConfig);
	flagop("outside", ImPlotLegendFlags_Outside, outConfig);
	flagop("horizontal", ImPlotLegendFlags_Horizontal, outConfig);
	flagop("sort", ImPlotLegendFlags_Sort, outConfig);

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
DearPyGui::set_configuration(PyObject* inDict, mvLineSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("segments", ImPlotLineFlags_Segments, outConfig.flags);
	flagop("loop", ImPlotLineFlags_Loop, outConfig.flags);
	flagop("skip_nan", ImPlotLineFlags_SkipNaN, outConfig.flags);
	flagop("no_clip", ImPlotLineFlags_NoClip, outConfig.flags);
	flagop("shaded", ImPlotLineFlags_Shaded, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "weight")) outConfig.weight = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("horizontal", ImPlotBarsFlags_Horizontal, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvBarGroupSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	auto backup_value = (*outConfig.value)[0];
	auto backup_label_ids = outConfig.label_ids;
	auto backup_group_size = outConfig.group_size;

	if (PyObject* item = PyDict_GetItemString(inDict, "values")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "label_ids")) { outConfig.label_ids = ToStringVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "group_size")) { outConfig.group_size = ToInt(item); }

	if (!ValidateBarGroupConfig(outConfig)) {
		(*outConfig.value)[0] = backup_value;
		outConfig.label_ids = backup_label_ids;
		outConfig.group_size = backup_group_size;
		return;
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "group_width")) outConfig.group_width = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "shift")) outConfig.shift = ToInt(item);

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("horizontal", ImPlotBarGroupsFlags_Horizontal, outConfig.flags);
	flagop("stacked", ImPlotBarGroupsFlags_Stacked, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvShadeSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	bool valueChanged = false;
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { valueChanged = true; (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { valueChanged = true; (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y1")) { valueChanged = true; (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y2")) { valueChanged = true; (*outConfig.value)[2] = ToDoubleVect(item); }

	if (valueChanged)
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
DearPyGui::set_configuration(PyObject* inDict, mvStemSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("horizontal", ImPlotStemsFlags_Horizontal, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvStairSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("pre_step", ImPlotStairsFlags_PreStep, outConfig.flags);
	flagop("shaded", ImPlotStairsFlags_Shaded, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvInfLineSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("horizontal", ImPlotInfLinesFlags_Horizontal, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvScatterSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("no_clip", ImPlotScatterFlags_NoClip, outConfig.flags);
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

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	auto reverse_flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags &= ~flag : flags |= flag;
	};

	// 2D histogram series flags
	flagop("density", ImPlotHistogramFlags_Density, outConfig.flags);
	reverse_flagop("outliers", ImPlotHistogramFlags_NoOutliers, outConfig.flags);
	flagop("col_major", ImPlotHistogramFlags_ColMajor, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "negative")) { (*outConfig.value)[2] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "positive")) { (*outConfig.value)[3] = ToDoubleVect(item); }
	
	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// error series flags
	flagop("horizontal", ImPlotErrorBarsFlags_Horizontal, outConfig.flags);
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

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("col_major", ImPlotHeatmapFlags_ColMajor, outConfig.flags);

	bool valueChanged = false;
	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { valueChanged = true; (*outConfig.value)[0] = ToDoubleVect(item); }

	if (valueChanged)
	{
		(*outConfig.value)[1].push_back(outConfig.bounds_min.y);
		(*outConfig.value)[1].push_back(outConfig.bounds_max.y);
	}
}

void
DearPyGui::set_configuration(PyObject* inDict, mvDigitalSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = ToDoubleVect(item); }
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

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	auto reverse_flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags &= ~flag : flags |= flag;
	};

	// histogram series flags
	flagop("cumulative", ImPlotHistogramFlags_Cumulative, outConfig.flags);
	flagop("density", ImPlotHistogramFlags_Density, outConfig.flags);
	reverse_flagop("outliers", ImPlotHistogramFlags_NoOutliers, outConfig.flags);
	flagop("horizontal", ImPlotHistogramFlags_Horizontal, outConfig.flags);
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
	if (PyObject* item = PyDict_GetItemString(inDict, "labels"))
	{
		outConfig.labels = ToStringVect(item);
		outConfig.clabels.clear();
		for (const auto& label : outConfig.labels)
			outConfig.clabels.push_back(label.c_str());
	}

	if (PyObject* item = PyDict_GetItemString(inDict, "values")) { (*outConfig.value)[0] = ToDoubleVect(item); }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// pie chart flags
	flagop("normalize", ImPlotPieChartFlags_Normalize, outConfig.flags);
	flagop("ignore_hidden", ImPlotPieChartFlags_IgnoreHidden, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvLabelSeriesConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "offset")) outConfig.offset = ToVec2(item);

	if (PyObject* item = PyDict_GetItemString(inDict, "x")) { (*outConfig.value)[0] = std::vector<double>{ToDouble(item)}; }
	if (PyObject* item = PyDict_GetItemString(inDict, "y")) { (*outConfig.value)[1] = std::vector<double>{ToDouble(item)}; }

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// plot text flags
	flagop("vertical", ImPlotTextFlags_Vertical, outConfig.flags);

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

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// flags
	flagop("no_fit", ImPlotItemFlags_NoFit, outConfig.flags);
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
DearPyGui::set_configuration(PyObject* inDict, mvAxisTagConfig& outConfig)
{
	if (inDict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(inDict, "color")) outConfig.color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "auto_rounding")) outConfig.auto_rounding = ToBool(item);
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
	flagop("share_series", ImPlotSubplotFlags_ShareItems, outConfig.flags);
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

	if (PyObject* item = PyDict_GetItemString(inDict, "scale")) outConfig.scale = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(inDict, "tick_format")) outConfig.formatter = ToString(item);

	// Legacy
	if (PyObject* item = PyDict_GetItemString(inDict, "log_scale")) outConfig.scale = (ToBool(item) ? ImPlotScale_Log10 : outConfig.scale);
	if (PyObject* item = PyDict_GetItemString(inDict, "time")) outConfig.scale = (ToBool(item) ? ImPlotScale_Time : outConfig.scale);

	// helper for bit flipping
	auto flagop = [inDict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// axis flags
	flagop("no_label", ImPlotAxisFlags_NoLabel, outConfig.flags);
	flagop("no_gridlines", ImPlotAxisFlags_NoGridLines, outConfig.flags);
	flagop("no_tick_marks", ImPlotAxisFlags_NoTickMarks, outConfig.flags);
	flagop("no_tick_labels", ImPlotAxisFlags_NoTickLabels, outConfig.flags);
	flagop("no_initial_fit", ImPlotAxisFlags_NoInitialFit, outConfig.flags);
	flagop("no_menus", ImPlotAxisFlags_NoMenus, outConfig.flags);
	flagop("no_side_switch", ImPlotAxisFlags_NoSideSwitch, outConfig.flags);
	flagop("no_highlight", ImPlotAxisFlags_NoHighlight, outConfig.flags);
	flagop("opposite", ImPlotAxisFlags_Opposite, outConfig.flags);
	flagop("foreground_grid", ImPlotAxisFlags_Foreground, outConfig.flags);
	flagop("invert", ImPlotAxisFlags_Invert, outConfig.flags);
	flagop("auto_fit", ImPlotAxisFlags_AutoFit, outConfig.flags);
	flagop("range_fit", ImPlotAxisFlags_RangeFit, outConfig.flags);
	flagop("pan_stretch", ImPlotAxisFlags_PanStretch, outConfig.flags);
	flagop("lock_min", ImPlotAxisFlags_LockMin, outConfig.flags);
	flagop("lock_max", ImPlotAxisFlags_LockMax, outConfig.flags);

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

	PyDict_SetItemString(outDict, "pan_button", mvPyObject(ToPyInt(inConfig.pan)));
	PyDict_SetItemString(outDict, "pan_mod", mvPyObject(ToPyInt(inConfig.pan_mod)));
	PyDict_SetItemString(outDict, "fit_button", mvPyObject(ToPyInt(inConfig.fit)));
	PyDict_SetItemString(outDict, "context_menu_button", mvPyObject(ToPyInt(inConfig.menu)));
	PyDict_SetItemString(outDict, "box_select_button", mvPyObject(ToPyInt(inConfig.select)));
	PyDict_SetItemString(outDict, "box_select_mod", mvPyObject(ToPyInt(inConfig.select_mod)));
	PyDict_SetItemString(outDict, "box_select_cancel_button", mvPyObject(ToPyInt(inConfig.select_cancel)));
	PyDict_SetItemString(outDict, "query_toggle_mod", mvPyObject(ToPyInt(inConfig.query_toggle_mod)));
	PyDict_SetItemString(outDict, "horizontal_mod", mvPyObject(ToPyInt(inConfig.select_horz_mod)));
	PyDict_SetItemString(outDict, "vertical_mod", mvPyObject(ToPyInt(inConfig.select_vert_mod)));
	PyDict_SetItemString(outDict, "override_mod", mvPyObject(ToPyInt(inConfig.override_mod)));
	PyDict_SetItemString(outDict, "zoom_mod", mvPyObject(ToPyInt(inConfig.zoom_mod)));
	PyDict_SetItemString(outDict, "zoom_rate", mvPyObject(ToPyFloat(inConfig.zoom_rate)));
	PyDict_SetItemString(outDict, "use_local_time", mvPyObject(ToPyBool(inConfig.localTime)));
	PyDict_SetItemString(outDict, "use_ISO8601", mvPyObject(ToPyBool(inConfig.iSO8601)));
	PyDict_SetItemString(outDict, "use_24hour_clock", mvPyObject(ToPyBool(inConfig.clock24Hour)));
	PyDict_SetItemString(outDict, "query", mvPyObject(ToPyBool(inConfig.query_enabled)));
	PyDict_SetItemString(outDict, "query_color", mvPyObject(ToPyColor(inConfig.query_color)));
	PyDict_SetItemString(outDict, "min_query_rects", mvPyObject(ToPyInt(inConfig.min_query_rects)));
	PyDict_SetItemString(outDict, "max_query_rects", mvPyObject(ToPyInt(inConfig.max_query_rects)));

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
	checkbitset("no_mouse_pos", ImPlotFlags_NoMouseText, inConfig._flags);
	checkbitset("crosshairs", ImPlotFlags_Crosshairs, inConfig._flags);
	checkbitset("equal_aspects", ImPlotFlags_Equal, inConfig._flags);
	checkbitset("no_inputs", ImPlotFlags_NoInputs, inConfig._flags);
	checkbitset("no_frame", ImPlotFlags_NoFrame, inConfig._flags);
	// checkbitset("canvas_only", ImPlotFlags_CanvasOnly, inConfig._flags);
}

void
DearPyGui::fill_configuration_dict(const mvDragLineConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "color", ToPyColor(inConfig.color));
	PyDict_SetItemString(outDict, "thickness", ToPyFloat(inConfig.thickness));
	PyDict_SetItemString(outDict, "show_label", ToPyBool(inConfig.thickness));
	PyDict_SetItemString(outDict, "vertical", ToPyBool(inConfig.vertical));


	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, ToPyBool(flags & flag));
	};

	// drag line flags
	checkbitset("delayed", ImPlotDragToolFlags_Delayed, inConfig.flags);
	checkbitset("no_cursor", ImPlotDragToolFlags_NoCursors, inConfig.flags);
	checkbitset("no_fit", ImPlotDragToolFlags_NoFit, inConfig.flags);
	checkbitset("no_inputs", ImPlotDragToolFlags_NoInputs, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvDragRectConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "color", mvPyObject(ToPyColor(inConfig.color)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, ToPyBool(flags & flag));
	};

	// drag rect flags
	checkbitset("delayed", ImPlotDragToolFlags_Delayed, inConfig.flags);
	checkbitset("no_cursor", ImPlotDragToolFlags_NoCursors, inConfig.flags);
	checkbitset("no_fit", ImPlotDragToolFlags_NoFit, inConfig.flags);
	checkbitset("no_inputs", ImPlotDragToolFlags_NoInputs, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvDragPointConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "radius", mvPyObject(ToPyFloat(inConfig.radius)));
	PyDict_SetItemString(outDict, "color", mvPyObject(ToPyColor(inConfig.color)));
	PyDict_SetItemString(outDict, "show_label", mvPyObject(ToPyBool(inConfig.show_label)));
	PyDict_SetItemString(outDict, "clamped", mvPyObject(ToPyBool(inConfig.clamped)));
	PyDict_SetItemString(outDict, "offset", mvPyObject(ToPyPair(inConfig.pixOffset.x, inConfig.pixOffset.y)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, ToPyBool(flags & flag));
	};

	// drag rect flags
	checkbitset("delayed", ImPlotDragToolFlags_Delayed, inConfig.flags);
	checkbitset("no_cursor", ImPlotDragToolFlags_NoCursors, inConfig.flags);
	checkbitset("no_fit", ImPlotDragToolFlags_NoFit, inConfig.flags);
	checkbitset("no_inputs", ImPlotDragToolFlags_NoInputs, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvPlotLegendConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "location", mvPyObject(ToPyInt(inConfig.legendLocation)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// plot flags
	checkbitset("horizontal", ImPlotLegendFlags_Horizontal, inConfig.flags);
	checkbitset("no_highlight_axis", ImPlotLegendFlags_NoHighlightAxis, inConfig.flags);
	checkbitset("no_highlight_item", ImPlotLegendFlags_NoHighlightItem, inConfig.flags);
	checkbitset("no_menus", ImPlotLegendFlags_NoMenus, inConfig.flags);
	checkbitset("outside", ImPlotLegendFlags_Outside, inConfig.flags);
	checkbitset("no_buttons", ImPlotLegendFlags_NoButtons, inConfig.flags);
	checkbitset("sort", ImPlotLegendFlags_Sort, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvLineSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// flags
	checkbitset("segments", ImPlotLineFlags_Segments, inConfig.flags);
	checkbitset("loop", ImPlotLineFlags_Loop, inConfig.flags);
	checkbitset("skip_nan", ImPlotLineFlags_SkipNaN, inConfig.flags);
	checkbitset("no_clip", ImPlotLineFlags_NoClip, inConfig.flags);
	checkbitset("shaded", ImPlotLineFlags_Shaded, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvShadeSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
}

void
DearPyGui::fill_configuration_dict(const mvBarSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "weight", ToPyFloat(inConfig.weight));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// bar flags
	checkbitset("horizontal", ImPlotBarsFlags_Horizontal, inConfig.flags);
}


void
DearPyGui::fill_configuration_dict(const mvStemSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// flags
	checkbitset("horizontal", ImPlotStemsFlags_Horizontal, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvBarGroupSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "label_ids", ToPyList(inConfig.label_ids));
	PyDict_SetItemString(outDict, "group_width", ToPyFloat(inConfig.group_width));
	PyDict_SetItemString(outDict, "group_size", ToPyInt(inConfig.group_size));
	PyDict_SetItemString(outDict, "shift", ToPyInt(inConfig.shift));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// flags
	checkbitset("horizontal", ImPlotBarGroupsFlags_Horizontal, inConfig.flags);
	checkbitset("stacked", ImPlotBarGroupsFlags_Stacked, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvStairSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// stair flags
	checkbitset("pre_step", ImPlotStairsFlags_PreStep, inConfig.flags);
	checkbitset("shaded", ImPlotStairsFlags_Shaded, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvInfLineSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// flags
	checkbitset("horizontal", ImPlotInfLinesFlags_Horizontal, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvScatterSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// flags
	checkbitset("no_clip", ImPlotScatterFlags_NoClip, inConfig.flags);
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

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	auto reverse_checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(!ImHasFlag(flags, flag));
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// flags
	checkbitset("density", ImPlotHistogramFlags_Density, inConfig.flags);
	reverse_checkbitset("outliers", ImPlotHistogramFlags_NoOutliers, inConfig.flags);
	checkbitset("col_major", ImPlotHistogramFlags_ColMajor, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvErrorSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
	
	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// flags
	checkbitset("horizontal", ImPlotErrorBarsFlags_Horizontal, inConfig.flags);
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

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// flags
	checkbitset("col_major", ImPlotHeatmapFlags_ColMajor, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvDigitalSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
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
	
	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	auto reverse_checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(!ImHasFlag(flags, flag));
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// histogram flags
	checkbitset("horizontal", ImPlotHistogramFlags_Horizontal, inConfig.flags);
	checkbitset("cumulative", ImPlotHistogramFlags_Cumulative, inConfig.flags);
	reverse_checkbitset("outliers", ImPlotHistogramFlags_NoOutliers, inConfig.flags);
	checkbitset("density", ImPlotHistogramFlags_Density, inConfig.flags);
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
	PyDict_SetItemString(outDict, "labels", mvPyObject(ToPyList(inConfig.labels)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// pie chart flags
	checkbitset("normalize", ImPlotPieChartFlags_Normalize, inConfig.flags);
	checkbitset("ignore_hidden", ImPlotPieChartFlags_IgnoreHidden, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvLabelSeriesConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "offset", mvPyObject(ToPyPair(inConfig.offset.x, inConfig.offset.y)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// plot text flags
	checkbitset("vertical", ImPlotTextFlags_Vertical, inConfig.flags);
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

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// flags
	checkbitset("no_fit", ImPlotItemFlags_NoFit, inConfig.flags);
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
DearPyGui::fill_configuration_dict(const mvAxisTagConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;
	PyDict_SetItemString(outDict, "color", mvPyObject(ToPyColor(inConfig.color)));
	PyDict_SetItemString(outDict, "auto_rounding", mvPyObject(ToPyBool(inConfig.auto_rounding)));
}

void
DearPyGui::fill_configuration_dict(const mvSubPlotsConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "rows", mvPyObject(ToPyInt(inConfig.rows)));
	PyDict_SetItemString(outDict, "columns", mvPyObject(ToPyInt(inConfig.cols)));
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
	checkbitset("share_series", ImPlotSubplotFlags_ShareItems, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvPlotAxisConfig& inConfig, PyObject* outDict)
{
	if (outDict == nullptr)
		return;

	PyDict_SetItemString(outDict, "scale", mvPyObject(ToPyInt(inConfig.scale)));
	PyDict_SetItemString(outDict, "tick_format", mvPyObject(ToPyString(inConfig.formatter)));
	// Legacy
	PyDict_SetItemString(outDict, "log_scale", mvPyObject(ToPyBool(inConfig.scale == ImPlotScale_Log10)));
	PyDict_SetItemString(outDict, "time", mvPyObject(ToPyBool(inConfig.scale == ImPlotScale_Time)));

	// helper to check and set bit
	auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_result = ToPyBool(flags & flag);
		PyDict_SetItemString(outDict, keyword, py_result);
	};

	// plot flags
	checkbitset("no_label", ImPlotAxisFlags_NoLabel, inConfig.flags);
	checkbitset("no_gridlines", ImPlotAxisFlags_NoGridLines, inConfig.flags);
	checkbitset("no_tick_marks", ImPlotAxisFlags_NoTickMarks, inConfig.flags);
	checkbitset("no_tick_labels", ImPlotAxisFlags_NoTickLabels, inConfig.flags);
	checkbitset("no_initial_fit", ImPlotAxisFlags_NoInitialFit, inConfig.flags);
	checkbitset("no_menus", ImPlotAxisFlags_NoMenus, inConfig.flags);
	checkbitset("no_side_switch", ImPlotAxisFlags_NoSideSwitch, inConfig.flags);
	checkbitset("no_highlight", ImPlotAxisFlags_NoHighlight, inConfig.flags);
	checkbitset("opposite", ImPlotAxisFlags_Opposite, inConfig.flags);
	checkbitset("foreground_grid", ImPlotAxisFlags_Foreground, inConfig.flags);
	checkbitset("invert", ImPlotAxisFlags_Invert, inConfig.flags);
	checkbitset("auto_fit", ImPlotAxisFlags_AutoFit, inConfig.flags);
	checkbitset("range_fit", ImPlotAxisFlags_RangeFit, inConfig.flags);
	checkbitset("pan_stretch", ImPlotAxisFlags_PanStretch, inConfig.flags);
	checkbitset("lock_min", ImPlotAxisFlags_LockMin, inConfig.flags);
	checkbitset("lock_max", ImPlotAxisFlags_LockMax, inConfig.flags);
}

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------

void
mvDragPoint::setPyValue(PyObject* value)
{
	std::vector<double> temp = ToDoubleVect(value);
	while (temp.size() < 2)
		temp.push_back(0.0);
	std::array<double, 2> temp_array;
	for (size_t i = 0; i < temp_array.size(); i++)
		temp_array[i] = temp[i];
	if (configData.value)
		*configData.value = temp_array;
	else
		configData.value = std::make_shared<std::array<double, 2>>(temp_array);
}

void
mvDragRect::setPyValue(PyObject* value)
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