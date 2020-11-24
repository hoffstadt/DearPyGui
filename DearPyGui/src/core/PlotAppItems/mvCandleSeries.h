#pragma once
#include "mvPlot.h"

namespace Marvel {

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

    static void PlotCandlestick(const char* label_id, const float* xs, const float* opens, 
        const float* closes, const float* lows, const float* highs, int count, 
        bool tooltip, float width_percent, ImVec4 bullCol, ImVec4 bearCol) {

        // get ImGui window DrawList
        ImDrawList* draw_list = ImPlot::GetPlotDrawList();
        // calc real value width
        float half_width = count > 1 ? (xs[1] - xs[0]) * width_percent : width_percent;

        // custom tool
        if (ImPlot::IsPlotHovered() && tooltip) {
            ImPlotPoint mouse = ImPlot::GetPlotMousePos();
            mouse.x = ImPlot::RoundTime(ImPlotTime::FromDouble(mouse.x), ImPlotTimeUnit_Day).ToDouble();
            float  tool_l = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
            float  tool_r = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
            float  tool_t = ImPlot::GetPlotPos().y;
            float  tool_b = tool_t + ImPlot::GetPlotSize().y;
            ImPlot::PushPlotClipRect();
            draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128, 128, 128, 64));
            ImPlot::PopPlotClipRect();
            // find mouse location index
            int idx = BinarySearch(xs, 0, count - 1, (float)mouse.x);
            // render tool tip (won't be affected by plot clip rect)
            if (idx != -1) {
                ImGui::BeginTooltip();
                char buff[32];
                ImPlot::FormatDate(ImPlotTime::FromDouble(xs[idx]), buff, 32, ImPlotDateFmt_DayMoYr, ImPlot::GetStyle().UseISO8601);
                ImGui::Text("Day:   %s", buff);
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
            ImPlot::GetCurrentItem()->Color = ImVec4(0.25f, 0.25f, 0.25f, 1);
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


	class mvCandleSeries : public mvSeries
	{

	public:

		mvCandleSeries(std::string name, const std::vector<float>* dates, const std::vector<float>* opens,
			const std::vector<float>* highs, const std::vector<float>* lows, const std::vector<float>* closes,
			float width, mvColor bull, mvColor bear, ImPlotYAxis_ axis)
            : mvSeries(name, { dates, opens, highs, lows, closes }, axis),
			m_width(width),
			m_bullColor(bull),
			m_bearColor(bear)
		{

            if (!dates->empty())
            {
                auto minmax = minmax_element(dates->begin(), dates->end());
                m_maxX = *minmax.second;
                m_minX = *minmax.first;
                minmax = minmax_element(lows->begin(), lows->end());
                m_minY = *minmax.first;
                minmax = minmax_element(highs->begin(), highs->end());
                m_maxY = *minmax.second;
            }
			
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Candle; }

		void draw() override
		{

			PlotCandlestick(m_name.c_str(), m_data[0].data(), m_data[1].data(), m_data[4].data(),
				m_data[3].data(), m_data[2].data(), (int)m_data[0].size(), m_tooltip, m_width, m_bullColor.toVec4(), 
				m_bearColor.toVec4());
		}

	private:

		float m_width = 0.25f;
		bool m_tooltip = true;
		mvColor m_bullColor = { 0, 255, 113, 255};
		mvColor m_bearColor = { 218, 13, 79, 255 };

	};

}
