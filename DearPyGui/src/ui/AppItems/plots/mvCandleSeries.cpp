#include "mvCandleSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

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

static void PlotCandlestick(const char* label_id, const double* xs, const double* opens,
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
				ImGui::Text("Microsecond: %d", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Ms)
			{
				ImGui::Text("Millisecond: %d", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_S)
			{
				ImGui::Text("Second: %d", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Min)
			{
				ImGui::Text("Minute: %d", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Hr)
			{
				ImGui::Text("Hour: %d", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Mo)
			{
				ImGui::Text("Month: %d", xs[idx]);
			}
			else if (time_unit == ImPlotTimeUnit_Yr)
			{
				ImGui::Text("Year: %d", xs[idx]);
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

PyObject* mvCandleSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvCandleSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvCandleSeries::setDataSource(mvUUID dataSource)
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
	_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
}

void mvCandleSeries::draw(ImDrawList* drawlist, float x, float y)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!config.show)
		return;

	// push font if a font object is attached
	if (font)
	{
		ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(this);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* datesptr;
		static const std::vector<double>* openptr;
		static const std::vector<double>* closeptr;
		static const std::vector<double>* lowptr;
		static const std::vector<double>* highptr;

		datesptr = &(*_value.get())[0];
		openptr = &(*_value.get())[1];
		closeptr = &(*_value.get())[2];
		lowptr = &(*_value.get())[3];
		highptr = &(*_value.get())[4];

		PlotCandlestick(info.internalLabel.c_str(), datesptr->data(), openptr->data(), closeptr->data(),
			lowptr->data(), highptr->data(), (int)datesptr->size(), _tooltip, _weight, _bullColor,
			_bearColor, _timeunit);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(info.internalLabel.c_str(), 1))
		{
			for (auto& childset : childslots)
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
	if (font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(this);

}

void mvCandleSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	(*_value)[0] = ToDoubleVect(PyTuple_GetItem(dict, 0));
	(*_value)[1] = ToDoubleVect(PyTuple_GetItem(dict, 1));
	(*_value)[2] = ToDoubleVect(PyTuple_GetItem(dict, 2));
	(*_value)[3] = ToDoubleVect(PyTuple_GetItem(dict, 3));
	(*_value)[4] = ToDoubleVect(PyTuple_GetItem(dict, 4));
}

void mvCandleSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "bull_color")) _bullColor = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "bear_color")) _bearColor = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "weight")) _weight = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "tooltip")) _tooltip = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "dates")) { (*_value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "opens")) { (*_value)[1] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "closes")) { (*_value)[2] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "lows")) { (*_value)[3] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "highs")) { (*_value)[4] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "time_unit")) { _timeunit = ToUUID(item); }

}

void mvCandleSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_bull_color = ToPyColor(_bullColor);
	mvPyObject py_bear_color = ToPyColor(_bearColor);
	mvPyObject py_weight = ToPyFloat(_weight);
	mvPyObject py_tooltip = ToPyBool(_tooltip);
	mvPyObject py_timeunit = ToPyLong(_timeunit);

	PyDict_SetItemString(dict, "bull_color", py_bull_color);
	PyDict_SetItemString(dict, "bear_color", py_bear_color);
	PyDict_SetItemString(dict, "weight", py_weight);
	PyDict_SetItemString(dict, "tooltip", py_tooltip);
	PyDict_SetItemString(dict, "time_unit", py_timeunit);
}

void mvCandleSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvCandleSeries*>(item);
	if (config.source != 0) _value = titem->_value;
	_weight = titem->_weight;
	_tooltip = titem->_tooltip;
	_bullColor = titem->_bullColor;
	_bearColor = titem->_bearColor;
}