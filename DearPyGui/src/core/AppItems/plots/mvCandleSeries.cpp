#include "mvCandleSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

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

    static void PlotCandlestick(const char* label_id, const double* xs, const double* opens,
        const double* closes, const double* lows, const double* highs, int count,
        bool tooltip, float width_percent, const ImVec4& bullCol, const ImVec4& bearCol) {

        // get ImGui window DrawList
        ImDrawList* draw_list = ImPlot::GetPlotDrawList();
        // calc real value width
        float half_width = count > 1 ? ((float)xs[1] - (float)xs[0]) * width_percent : width_percent;

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
            int idx = BinarySearch(xs, 0, count - 1, mouse.x);
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
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::DoubleList, "dates" });
		args.push_back({ mvPyDataType::DoubleList, "opens" });
		args.push_back({ mvPyDataType::DoubleList, "closes" });
		args.push_back({ mvPyDataType::DoubleList, "lows" });
		args.push_back({ mvPyDataType::DoubleList, "highs" });
		args.push_back({ mvPyDataType::IntList, "bull_color", mvArgType::KEYWORD_ARG, "(0, 255, 113, 255)"});
		args.push_back({ mvPyDataType::IntList, "bear_color", mvArgType::KEYWORD_ARG, "(218, 13, 79, 255)"});
		args.push_back({ mvPyDataType::Integer, "weight", mvArgType::KEYWORD_ARG, "0.25" });
		args.push_back({ mvPyDataType::Bool, "tooltip", mvArgType::KEYWORD_ARG, "True" });

		mvPythonParserSetup setup;
		setup.about = "Adds a candle series to a plot.";
		setup.category = { "Plotting", "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvCandleSeries::mvCandleSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
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
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
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
		if (!_show)
			return;

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

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

			PlotCandlestick(_internalLabel.c_str(), datesptr->data(), openptr->data(), closeptr->data(),
				lowptr->data(), highptr->data(), (int)datesptr->size(), _tooltip, _weight, _bullColor,
				_bearColor);

			// Begin a popup for a legend entry.
			if (ImPlot::BeginLegendPopup(_internalLabel.c_str(), 1))
			{
				for (auto& childset : _children)
				{
					for (auto& item : childset)
					{
						// skip item if it's not shown
						if (!item->_show)
							continue;
						item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
						UpdateAppItemState(item->_state);
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
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

	}

	void mvCandleSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				(*_value)[0] = ToDoubleVect(item);
				break;

			case 1:
				(*_value)[1] = ToDoubleVect(item);
				break;

			case 2:
				(*_value)[2] = ToDoubleVect(item);
				break;

			case 3:
				(*_value)[3] = ToDoubleVect(item);
				break;

			case 4:
				(*_value)[4] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}
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

	}

	void mvCandleSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_bull_color = ToPyColor(_bullColor);
		mvPyObject py_bear_color = ToPyColor(_bearColor);
		mvPyObject py_weight = ToPyFloat(_weight);
		mvPyObject py_tooltip = ToPyBool(_tooltip);

		PyDict_SetItemString(dict, "bull_color", py_bull_color);
		PyDict_SetItemString(dict, "bear_color", py_bear_color);
		PyDict_SetItemString(dict, "weight", py_weight);
		PyDict_SetItemString(dict, "tooltip", py_tooltip);
	}

	void mvCandleSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvCandleSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_weight = titem->_weight;
		_tooltip = titem->_tooltip;
		_bullColor = titem->_bullColor;
		_bearColor = titem->_bearColor;
	}

}