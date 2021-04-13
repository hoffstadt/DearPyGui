#include "mvCandleSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

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

	void mvCandleSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("dates");
		parser.addArg<mvPyDataType::FloatList>("opens");
		parser.addArg<mvPyDataType::FloatList>("closes");
		parser.addArg<mvPyDataType::FloatList>("lows");
		parser.addArg<mvPyDataType::FloatList>("highs");

		parser.addArg<mvPyDataType::IntList>("bull_color", mvArgType::KEYWORD_ARG, "(0, 255, 113, 255)");
		parser.addArg<mvPyDataType::IntList>("bear_color", mvArgType::KEYWORD_ARG, "(218, 13, 79, 255)");

		parser.addArg<mvPyDataType::Integer>("axis", mvArgType::KEYWORD_ARG, "0");

		parser.addArg<mvPyDataType::Integer>("weight", mvArgType::KEYWORD_ARG, "0.25");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");
		parser.addArg<mvPyDataType::Bool>("tooltip", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCandleSeries::mvCandleSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvCandleSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		switch (m_axis)
		{
		case ImPlotYAxis_1:
			ImPlot::SetPlotYAxis(ImPlotYAxis_1);
			break;
		case ImPlotYAxis_2:
			ImPlot::SetPlotYAxis(ImPlotYAxis_2);
			break;
		case ImPlotYAxis_3:
			ImPlot::SetPlotYAxis(ImPlotYAxis_3);
			break;
		default:
			break;
		}

		static const std::vector<float>* datesptr;
		static const std::vector<float>* openptr;
		static const std::vector<float>* closeptr;
		static const std::vector<float>* lowptr;
		static const std::vector<float>* highptr;

		datesptr = &(*m_value.get())[0];
		openptr = &(*m_value.get())[1];
		closeptr = &(*m_value.get())[2];
		lowptr = &(*m_value.get())[3];
		highptr = &(*m_value.get())[4];

		PlotCandlestick(m_label.c_str(), datesptr->data(), openptr->data(), closeptr->data(),
			lowptr->data(), highptr->data(), (int)datesptr->size(), m_tooltip, m_width, m_bullColor,
			m_bearColor);

	}

	void mvCandleSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				(*m_value)[0] = ToFloatVect(item);
				break;

			case 1:
				(*m_value)[1] = ToFloatVect(item);
				break;

			case 2:
				(*m_value)[2] = ToFloatVect(item);
				break;

			case 3:
				(*m_value)[3] = ToFloatVect(item);
				break;

			case 4:
				(*m_value)[4] = ToFloatVect(item);
				break;

			default:
				break;
			}
		}

		resetMaxMins();
		calculateMaxMins();
	}

	void mvCandleSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bull_color")) m_bullColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bear_color")) m_bearColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "width")) m_width = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tooltip")) m_tooltip = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "dates")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "opens")) { valueChanged = true; (*m_value)[1] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "closes")) { valueChanged = true; (*m_value)[2] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "lows")) { valueChanged = true; (*m_value)[3] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "highs")) { valueChanged = true; (*m_value)[4] = ToFloatVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvCandleSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}