#include <algorithm>
#include "mvPlot.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvDrawList.h"
#include "mvItemRegistry.h"
#include "mvAreaSeries.h"
#include "mvBarSeries.h"
#include "mvCandleSeries.h"
#include "mvErrorSeries.h"
#include "mvHeatSeries.h"
#include "mvImageSeries.h"
#include "mvInfiniteLineSeries.h"
#include "mvLabelSeries.h"
#include "mvLineSeries.h"
#include "mvPieSeries.h"
#include "mvScatterSeries.h"
#include "mvShadeSeries.h"
#include "mvStairSeries.h"
#include "mvStemSeries.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvPlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_plot", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "x_axis_name", "", "''"},
			{mvPythonDataType::String, "y_axis_name", "", "''"},

			// plot flags
			{mvPythonDataType::Bool, "no_legend", "", "False"},
			{mvPythonDataType::Bool, "no_menus", "", "False"},
			{mvPythonDataType::Bool, "no_box_select", "", "False"},
			{mvPythonDataType::Bool, "no_mouse_pos", "", "False"},
			{mvPythonDataType::Bool, "no_highlight", "", "False"},
			{mvPythonDataType::Bool, "no_child", "", "False"},
			{mvPythonDataType::Bool, "query", "", "False"},
			{mvPythonDataType::Bool, "crosshairs", "", "False"},
			{mvPythonDataType::Bool, "anti_aliased", "", "False"},
			{mvPythonDataType::Bool, "equal_aspects", "", "False"},
			{mvPythonDataType::Bool, "yaxis2", "", "False"},
			{mvPythonDataType::Bool, "yaxis3", "", "False"},

			// x axis flags
			{mvPythonDataType::Bool, "xaxis_no_gridlines", "", "False"},
			{mvPythonDataType::Bool, "xaxis_no_tick_marks", "", "False"},
			{mvPythonDataType::Bool, "xaxis_no_tick_labels", "", "False"},
			{mvPythonDataType::Bool, "xaxis_log_scale", "", "False"},
			{mvPythonDataType::Bool, "xaxis_time", "", "False"},
			{mvPythonDataType::Bool, "xaxis_invert", "", "False"},
			{mvPythonDataType::Bool, "xaxis_lock_min", "", "False"},
			{mvPythonDataType::Bool, "xaxis_lock_max", "", "False"},

			// y axis flags
			{mvPythonDataType::Bool, "yaxis_no_gridlines", "", "False"},
			{mvPythonDataType::Bool, "yaxis_no_tick_marks", "", "False"},
			{mvPythonDataType::Bool, "yaxis_no_tick_labels", "", "False"},
			{mvPythonDataType::Bool, "yaxis_log_scale", "", "False"},
			{mvPythonDataType::Bool, "yaxis_invert", "", "False"},
			{mvPythonDataType::Bool, "yaxis_lock_min", "", "False"},
			{mvPythonDataType::Bool, "yaxis_lock_max", "", "False"},

			// y2 axis flags
			{mvPythonDataType::Bool, "y2axis_no_gridlines", "", "False"},
			{mvPythonDataType::Bool, "y2axis_no_tick_marks", "", "False"},
			{mvPythonDataType::Bool, "y2axis_no_tick_labels", "", "False"},
			{mvPythonDataType::Bool, "y2axis_log_scale", "", "False"},
			{mvPythonDataType::Bool, "y2axis_invert", "", "False"},
			{mvPythonDataType::Bool, "y2axis_lock_min", "", "False"},
			{mvPythonDataType::Bool, "y2axis_lock_max", "", "False"},

			// y3 axis flags
			{mvPythonDataType::Bool, "y3axis_no_gridlines", "", "False"},
			{mvPythonDataType::Bool, "y3axis_no_tick_marks", "", "False"},
			{mvPythonDataType::Bool, "y3axis_no_tick_labels", "", "False"},
			{mvPythonDataType::Bool, "y3axis_log_scale", "", "False"},
			{mvPythonDataType::Bool, "y3axis_invert", "", "False"},
			{mvPythonDataType::Bool, "y3axis_lock_min", "", "False"},
			{mvPythonDataType::Bool, "y3axis_lock_max", "", "False"},

			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width", "", "-1"},
			{mvPythonDataType::Integer, "height", "", "-1"},
			{mvPythonDataType::Callable , "query_callback", "Callback ran when plot is queried. Should be of the form 'def Callback(sender, data)'\n Data is (x_min, x_max, y_min, y_max).", "None"},

			{mvPythonDataType::Bool, "show_color_scale", "", "False"},
			{mvPythonDataType::Float, "scale_min", "", "0.0"},
			{mvPythonDataType::Float, "scale_max", "", "1.0"},
			{mvPythonDataType::Integer, "scale_height", "", "100"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "show_annotations", "", "True"},
			{mvPythonDataType::Bool, "show_drag_lines", "", "True"},
			{mvPythonDataType::Bool, "show_drag_points", "", "True"}

		}, "Adds a plot widget.", "None", "Plotting") });

		parsers->insert({ "add_drag_line", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "thickness", "", "-1"},
			{mvPythonDataType::Bool, "y_line", "", "False"},
			{mvPythonDataType::Bool, "show_label", "", "True"},
			{mvPythonDataType::Callable, "callback", "function to run when line is dragged", "None"},
			{mvPythonDataType::Float, "default_value", "", "0.0"},
		}, "Adds a drag line to a plot.", "None", "Plotting") });

		parsers->insert({ "delete_drag_line", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"}
		}, "Deletes a drag line if it exists.", "None", "Plotting") });

		parsers->insert({ "add_drag_point", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "radius", "", "4.0"},
			{mvPythonDataType::Bool, "show_label", "", "True"},
			{mvPythonDataType::Callable, "callback", "function to run when point is moved", "None"},
			{mvPythonDataType::Float, "default_x", "", "0.0"},
			{mvPythonDataType::Float, "default_y", "", "0.0"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });

		parsers->insert({ "delete_drag_point", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"}
		}, "Deletes a drag point if it exists.", "None", "Plotting") });

		parsers->insert({ "add_annotation", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "text"},
			{mvPythonDataType::Double, "x"},
			{mvPythonDataType::Double, "y"},
			{mvPythonDataType::Float, "xoffset"},
			{mvPythonDataType::Float, "yoffset"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Bool, "clamped", "", "True"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Adds an annotation to a plot.", "None", "Plotting") });

		parsers->insert({ "delete_annotation", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
		}, "Deletes an annotation", "None", "Plotting") });

		parsers->insert({ "clear_plot", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Clears a plot.", "None", "Plotting") });

		parsers->insert({ "is_plot_queried", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Returns true if plot was queried", "bool", "Plotting") });

		parsers->insert({ "get_plot_query_area", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Returns the bounding axis limits for the query area [x_min, x_max, y_min, y_max]", "List[float]", "Plotting") });

		parsers->insert({ "delete_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "series"}
		}, "Deletes a series if it exists.", "None", "Plotting") });

		parsers->insert({ "get_plot_xlimits", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Returns the plots x limits", "List[float]", "Plotting") });

		parsers->insert({ "get_plot_ylimits", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Returns the plots x limits", "List[float]", "Plotting") });

		parsers->insert({ "set_plot_xlimits", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Float, "xmin"},
			{mvPythonDataType::Float, "xmax"},
		}, "Sets x axis limits of a plot. (can be undone with set_plot_xlimits_auto()", "None", "Plotting") });

		parsers->insert({ "set_plot_ylimits", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Float, "ymin"},
			{mvPythonDataType::Float, "ymax"},
		}, "Sets y axis limits of a plot. (can be undone with set_plot_ylimits_auto()", "None", "Plotting") });

		parsers->insert({ "set_plot_xlimits_auto", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots x limits to be automatic.", "None", "Plotting") });

		parsers->insert({ "set_plot_ylimits_auto", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots y limits to be automatic.", "None", "Plotting") });

		parsers->insert({ "add_image_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::FloatList, "bounds_min", "bottom left coordinate"},
			{mvPythonDataType::FloatList, "bounds_max", "top right coordinate"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "uv_min", "normalized texture coordinates", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "uv_max", "normalized texture coordinates", "(1.0, 1.0)"},
			{mvPythonDataType::IntList, "tint_color", "", "(255, 255, 255, 255)"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a image series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_pie_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "values"},
			{mvPythonDataType::StringList, "labels"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "normalize", "", "False"},
			{mvPythonDataType::Float, "angle", "", "90.0"},
			{mvPythonDataType::String, "format", "", "'%0.2f'"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a pie series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_line_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a line series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_stair_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a stair series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_error_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::FloatList, "negative"},
			{mvPythonDataType::FloatList, "positive"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds an error series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_bar_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a bar series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_shade_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y1"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "y2"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a shade series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_candle_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "date"},
			{mvPythonDataType::FloatList, "opens"},
			{mvPythonDataType::FloatList, "highs"},
			{mvPythonDataType::FloatList, "lows"},
			{mvPythonDataType::FloatList, "closes"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "tooltip", "", "True"},
			{mvPythonDataType::FloatList, "bull_color", "", "(0, 255, 113, 255)"},
			{mvPythonDataType::FloatList, "bear_color", "", "(218, 13, 79, 255)"},
			{mvPythonDataType::Float, "weight", "", "0.25"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a candle series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_scatter_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "marker", "", "0"},
			{mvPythonDataType::Float, "size", "", "4.0"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::FloatList, "outline", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Bool, "xy_data_format", "split x and y", "False"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a scatter series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_stem_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "marker", "", "0"},
			{mvPythonDataType::Float, "size", "", "4.0"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::FloatList, "outline", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a stem series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_text_point", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "vertical", "", "False"},
			{mvPythonDataType::Integer, "xoffset", "", "0.0"},
			{mvPythonDataType::Integer, "yoffset", "", "0.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a point with text to a plot.", "None", "Plotting") });

		parsers->insert({ "add_area_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::FloatList, "y"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds an area series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_heat_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "values"},
			{mvPythonDataType::Integer, "rows"},
			{mvPythonDataType::Integer, "columns"},
			{mvPythonDataType::Double, "scale_min"},
			{mvPythonDataType::Double, "scale_max"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "format", "", "'%0.1f'"},
			{mvPythonDataType::FloatList, "bounds_min", "", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "bounds_max", "", "(1.0, 1.0)"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a heat series to a plot.", "None", "Plotting") });

		parsers->insert({ "set_xticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Object, "label_pairs", "list of [str,float]"},
		}, "Sets plots x ticks and labels", "None", "Plotting") });

		parsers->insert({ "set_yticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Object, "label_pairs", "list of [str,float]"},
		}, "Sets plots y ticks and labels", "None", "Plotting") });

		parsers->insert({ "reset_xticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots x ticks and labels back to automatic", "None", "Plotting") });

		parsers->insert({ "reset_yticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots y ticks and labels back to automatic", "None", "Plotting") });

		parsers->insert({ "add_vline_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a infinite vertical line series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_hline_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "x"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "weight", "", "1.0"},
			{mvPythonDataType::Bool, "update_bounds", "update plot bounds", "True"},
			{mvPythonDataType::Integer, "axis", "", "0"},
		}, "Adds a infinite horizontal line series to a plot.", "None", "Plotting") });
	}

	static bool CheckList(const char* plot, PyObject* list)
	{
		if (!PyList_Check(list))
		{
			ThrowPythonException(std::string(plot) + " add area series requires a list of floats.");
			return false;
		}
		return true;
	}

	static bool CheckIfPlotOk(const char* name, mvAppItem* plot)
	{
		if (plot == nullptr)
		{
			ThrowPythonException(std::string(name) + " plot does not exist.");
			return false;
		}

		if (plot->getType() != mvAppItemType::mvPlot)
		{
			ThrowPythonException(std::string(name) + " is not a plot.");
			return false;
		}
		return true;
	}

	static bool Check2ArraySizes(const char* name, const std::vector<float>* first, const std::vector<float>* second)
	{
		if (second == nullptr)
			return true;

		return first->size() == second->size();
	}

	static bool CheckArraySizes(const char* name, const std::vector<const std::vector<float>*>& arrays)
	{
		for (size_t i = 0; i < arrays.size() - 1; i++)
		{
			if (!Check2ArraySizes(name, arrays[i], arrays[i + 1]))
			{
				ThrowPythonException(std::string(name) + " data list must be the same size.");
				return false;
			}
		}
		return true;
	}

	mvSeries::mvSeries(std::string name, const std::vector<const std::vector<float>*>& data, ImPlotYAxis_ axis)
		:
		m_name(name),
		m_axis(axis)
	{

		for (const auto* list : data)
			m_data.push_back(*list);

		if (!m_data[0].empty())
		{
			m_maxX = m_data[0][0];
			m_minX = m_data[0][0];
		}

		if (m_data.size() > 1)
		{
			if (!m_data[1].empty())
			{
				m_maxY = m_data[1][0];
				m_minY = m_data[1][0];
			}
		}

		for (const auto& x : m_data[0])
		{
			if (x > m_maxX) m_maxX = x;
			if (x < m_minX) m_minX = x;

		}

		if (m_data.size() > 1)
		{
			for (const auto& y : m_data[1])
			{
				if (y > m_maxY) m_maxY = y;
				if (y < m_minY) m_minY = y;
			}
		}
		else // for the infinite line series case
		{
			m_maxY = m_maxX;
			m_minY = m_minX;
		}

	}

	mvSeries::mvSeries(std::string name, const ImPlotPoint& boundsMin, const ImPlotPoint& boundsMax, ImPlotYAxis_ axis)
		:
		m_name(std::move(name)),
		m_axis(axis)
	{
		m_maxX = (float)boundsMax.x;
		m_maxY = (float)boundsMax.y;
		m_minX = (float)boundsMin.x;
		m_minY = (float)boundsMin.y;
	}

	mvPlot::mvPlot(const std::string& name, mvCallable queryCallback)
		: mvAppItem(name), m_queryCallback(queryCallback)
	{
		m_drawList = CreateRef<mvDrawList>();
		m_core_config.width = -1;
		m_core_config.height = -1;
	}

	void mvPlot::addDragPoint(const std::string& name, bool show_label, const mvColor& color, float radius, mvCallable callback, const double* dummyValue, const std::string& source)
	{
		if (!source.empty())
		{
			mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(source);
			if (!item)
			{
				ThrowPythonException("Source item not found.");
				return;
			}
			if (item->getDescription().valueType != StorageValueTypes::Float2)
			{
				ThrowPythonException("Values types do not match");
				return;
			}
			auto value = std::get<std::shared_ptr<std::array<float, 2>>>(item->getValue());
			double dummyx = value->data()[0];
			double dummyy = value->data()[1];

			m_dragPoints.push_back({ name, value, show_label, color, radius, callback, source });
			return;
		}
		auto value = std::make_shared<std::array<float, 2>>(std::array{ (float)dummyValue[0], (float)dummyValue[1] });

		m_dragPoints.push_back({ name, value, show_label, color, radius, callback, source });
	}

	void mvPlot::updateDragPoint(const std::string& name, bool show_label, const mvColor& color, float radius, mvCallable callback, const double* dummyValue, const std::string& source)
	{
		// check if drag point exist
		bool exists = false;
		for (auto& item : m_dragPoints)
		{
			if (item.name == name)
			{
				exists = true;
				if (item.source != source)
				{
					mvRef<mvAppItem> newitem = mvApp::GetApp()->getItemRegistry().getItem(source);
					if (!newitem)
					{
						ThrowPythonException("Source item not found.");
						continue;
					}
					if (newitem->getDescription().valueType != StorageValueTypes::Float2)
					{
						ThrowPythonException("Values types do not match");
						continue;
					}
					item.value = std::get<std::shared_ptr<std::array<float, 2>>>(newitem->getValue());
				}
				item.show_label = show_label;
				item.color = color;
				item.radius = radius;
				item.callback = callback;
				item.source = source;
				break;
			}
		}

		if (!exists)
			addDragPoint(name, show_label, color, radius, callback, dummyValue, source);
	}

	void mvPlot::addDragLine(const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, mvCallable callback, double dummyValue, const std::string& source)
	{
		if (!source.empty())
		{
			mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(source);
			if (!item)
			{
				ThrowPythonException("Source item not found.");
				return;
			}
			if (item->getDescription().valueType != StorageValueTypes::Float)
			{
				ThrowPythonException("Values types do not match");
				return;
			}
			auto value = std::get<std::shared_ptr<float>>(item->getValue());

			m_dragLines.push_back({ name, value, show_label, color, thickness, y_line, callback, source });
			return;
		}
		auto value = std::make_shared<float>((float)dummyValue);

		m_dragLines.push_back({ name, value, show_label, color, thickness, y_line, callback, source });
	}

	void mvPlot::updateDragLine(const std::string& name, bool show_label, const mvColor& color, float thickness, bool y_line, mvCallable callback, double dummyValue, const std::string& source)
	{

		// check if drag line exist
		bool exists = false;
		for (auto& item : m_dragLines)
		{
			if (item.name == name)
			{
				exists = true;
				if (item.source != source)
				{
					mvRef<mvAppItem> newitem = mvApp::GetApp()->getItemRegistry().getItem(source);
					if (!newitem)
					{
						ThrowPythonException("Source item not found.");
						continue;
					}
					if (newitem->getDescription().valueType != StorageValueTypes::Float)
					{
						ThrowPythonException("Values types do not match");
						continue;
					}
					item.value = std::get<std::shared_ptr<float>>(newitem->getValue());
				}
				item.show_label = show_label;
				item.color = color;
				item.thickness = thickness;
				item.y_line = y_line;
				item.callback = callback;
				item.source = source;
				break;
			}
		}

		if (!exists)
			addDragLine(name, show_label, color, thickness, y_line, callback, dummyValue, source);
	}

	void mvPlot::deleteDragPoint(const std::string& name)
	{
		// check if annotations exist
		bool exists = false;
		for (auto item : m_dragPoints)
		{
			if (item.name == name)
				exists = true;
		}

		if (exists)
		{
			auto oldDragPoints = m_dragPoints;
			m_dragPoints.clear();

			for (auto item : oldDragPoints)
			{
				if (item.name == name)
				{
					continue;
				}

				m_dragPoints.push_back(item);
			}

		}
	}

	void mvPlot::deleteDragLine(const std::string& name)
	{
		// check if annotations exist
		bool exists = false;
		for (auto item : m_dragLines)
		{
			if (item.name == name)
				exists = true;
		}

		if (exists)
		{
			auto oldDragLines = m_dragLines;
			m_dragLines.clear();

			for (auto item : oldDragLines)
			{
				if (item.name == name)
				{
					continue;
				}

				m_dragLines.push_back(item);
			}

		}
	}

	void mvPlot::addAnnotation(const std::string& name, double x, double y, float xoffset, float yoffset, const mvColor& color, const std::string& text, bool clamped)
	{
		m_annotations.push_back({ name, x, y, { xoffset, yoffset }, color, text, clamped });
	}

	void mvPlot::updateAnnotation(const std::string& name, double x, double y, float xoffset, float yoffset, const mvColor& color, const std::string& text, bool clamped)
	{
		if (name.empty())
		{
			addAnnotation(name, x, y, xoffset, yoffset, color, text, clamped);
			return;
		}

		// check if annotation exist
		bool exists = false;
		for (auto& item : m_annotations)
		{
			if (item.name == name)
			{
				exists = true;
				item.x = x;
				item.y = y;
				item.pix_offset.x = xoffset;
				item.pix_offset.y = yoffset;
				item.color = color;
				item.text = text;
				item.clamped = clamped;
			}
		}

		if(!exists)
			addAnnotation(name, x, y, xoffset, yoffset, color, text, clamped);
	}

	void mvPlot::deleteAnnotation(const std::string& name)
	{
		// check if annotations exist
		bool exists = false;
		for (auto item : m_annotations)
		{
			if (item.name == name)
				exists = true;
		}

		if (exists)
		{
			auto oldAnnotations = m_annotations;
			m_annotations.clear();

			for (auto item : oldAnnotations)
			{
				if (item.name == name)
					continue;

				m_annotations.push_back(item);
			}

		}
	}

	void mvPlot::addSeries(mvRef<mvSeries> series, bool updateBounds)
	{

		if (m_series.empty())
		{
			if (!m_setXLimits)
			{
				m_xlimits.x = series->m_minX;
				m_xlimits.y = series->m_maxX;
			}

			if (!m_setYLimits && series->m_axis == ImPlotYAxis_1)
			{
				m_ylimits.x = series->m_minY;
				m_ylimits.y = series->m_maxY;
			}
			else if (!m_setY2Limits && series->m_axis == ImPlotYAxis_2)
			{
				m_y2limits.x = series->m_minY;
				m_y2limits.y = series->m_maxY;
			}
			else if (!m_setY3Limits && series->m_axis == ImPlotYAxis_3)
			{
				m_y3limits.x = series->m_minY;
				m_y3limits.y = series->m_maxY;
			}
		}
		else
		{
			if (series->m_minX < m_xlimits.x && !m_setXLimits) m_xlimits.x = series->m_minX;
			if (series->m_maxX > m_xlimits.y && !m_setXLimits) m_xlimits.y = series->m_maxX;

			if (series->m_axis == ImPlotYAxis_1)
			{
				if (series->m_minY < m_ylimits.x && !m_setYLimits) m_ylimits.x = series->m_minY;
				if (series->m_maxY > m_ylimits.y && !m_setYLimits) m_ylimits.y = series->m_maxY;
			}
			else if (series->m_axis == ImPlotYAxis_2)
			{
				if (series->m_minY < m_y2limits.x && !m_setY2Limits) m_y2limits.x = series->m_minY;
				if (series->m_maxY > m_y2limits.y && !m_setY2Limits) m_y2limits.y = series->m_maxY;
			}
			else if (series->m_axis == ImPlotYAxis_3)
			{
				if (series->m_minY < m_y3limits.x && !m_setY3Limits) m_y3limits.x = series->m_minY;
				if (series->m_maxY > m_y3limits.y && !m_setY3Limits) m_y3limits.y = series->m_maxY;
			}
		}

		m_series.push_back(series);

		if(updateBounds)
			m_dirty = true;
	}

	void mvPlot::updateSeries(mvRef<mvSeries> series, bool updateBounds)
	{

		// check if series exist
		bool exists = false;
		for (auto& item : m_series)
		{
			if (item->getName() == series->getName() && series->getSeriesType() == item->getSeriesType())
			{
				exists = true;
				break;
			}
		}

		if (exists)
		{
			auto oldSeries = m_series;
			m_series.clear();

			for (auto& item : oldSeries)
			{
				if (item->getName() == series->getName() && series->getSeriesType() == item->getSeriesType())
				{
					item = nullptr;
					addSeries(series, updateBounds);
					continue;
				}

				m_series.push_back(item);
			}

			return;
		}

		addSeries(series, updateBounds);
	}

	void mvPlot::deleteSeries(const std::string& name)
	{

		// check if series exist
		bool exists = false;
		for (auto& item : m_series)
		{
			if (item->getName() == name)
			{
				exists = true;
				break;
			}
		}

		if (exists)
		{
			auto oldSeries = m_series;
			m_series.clear();

			for (auto& item : oldSeries)
			{
				if (item->getName() == name)
				{
					item = nullptr;
					continue;
				}

				m_series.push_back(item);
			}

		}

	}

	void mvPlot::SetColorMap(ImPlotColormap colormap)
	{
		if (colormap < ImPlot::GetColormapCount())
		m_dirty = true;			{
			m_colormap = colormap;
			m_dirty = true;
		}
	}

	void mvPlot::resetXTicks()
	{
		m_xlabels.clear();
		m_xclabels.clear();
		m_xlabelLocations.clear();
	}

	void mvPlot::resetYTicks()
	{
		m_ylabels.clear();
		m_yclabels.clear();
		m_ylabelLocations.clear();
	}

	void mvPlot::setXTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_xlabels = labels;
		m_xlabelLocations = locations;

		for (const auto& item : m_xlabels)
			m_xclabels.push_back(item.data());
	}

	void mvPlot::setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
	{
		m_ylabels = labels;
		m_ylabelLocations = locations;

		for (const auto& item : m_ylabels)
			m_yclabels.push_back(item.data());
	}

	void mvPlot::clear()
	{

		m_series.clear();
		m_annotations.clear();
		m_dragLines.clear();
		m_dragPoints.clear();

	}

	void mvPlot::draw()
	{
		if (m_colormapscale)
		{
			ImPlot::ColormapScale(std::string(m_core_config.name + "##colorscale").c_str(), m_scale_min, m_scale_max, ImVec2(0, m_scale_height));
			ImGui::SameLine();
		}

		ImGui::PushID(m_colormap);

		if (m_setXLimits || m_dirty)
			ImPlot::SetNextPlotLimitsX(m_xlimits.x, m_xlimits.y, ImGuiCond_Always);

		if (m_setYLimits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_ylimits.x, m_ylimits.y, ImGuiCond_Always);

		if (m_setY2Limits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_y2limits.x, m_y2limits.y, ImGuiCond_Always, ImPlotYAxis_2);

		if (m_setY3Limits || m_dirty)
			ImPlot::SetNextPlotLimitsY(m_y3limits.x, m_y3limits.y, ImGuiCond_Always, ImPlotYAxis_3);

		// resets automatic sizing when new data is added
		if (m_dirty) m_dirty = false;

		if (!m_xlabels.empty())
		{
			// TODO: Checks
			ImPlot::SetNextPlotTicksX(m_xlabelLocations.data(), (int)m_xlabels.size(), m_xclabels.data());
		}
		if (!m_ylabels.empty())
		{
			// TODO: Checks
			ImPlot::SetNextPlotTicksY(m_ylabelLocations.data(), (int)m_ylabels.size(), m_yclabels.data());
		}

		mvImPlotThemeScope scope(this);

		if (ImPlot::BeginPlot(m_label.c_str(), m_xaxisName.empty() ? nullptr : m_xaxisName.c_str(), m_yaxisName.empty() ? nullptr : m_yaxisName.c_str(),
			ImVec2((float)m_core_config.width, (float)m_core_config.height), m_flags,
			m_xflags, m_yflags, m_y2flags, m_y3flags))
		{
			ImPlot::PushColormap(m_colormap);

			// series
			for (auto series : m_series)
			{
				switch (series->m_axis)
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
				series->draw();
			}

			// annotations
			if (m_showAnnotations)
			{
				for (const auto& annotation : m_annotations)
				{
					if (annotation.clamped)
						ImPlot::AnnotateClamped(annotation.x, annotation.y, annotation.pix_offset, annotation.color.toVec4(), annotation.text.c_str());
					else
						ImPlot::Annotate(annotation.x, annotation.y, annotation.pix_offset, annotation.color.toVec4(), annotation.text.c_str());
				}
			}

			// drag lines
			if (m_showDragLines)
			{
				for (auto& line : m_dragLines)
				{
					static double dummy = 0.0;
					dummy = *line.value;
					if (line.y_line)
					{
						if (ImPlot::DragLineY(line.name.c_str(),&dummy, line.show_label, line.color, line.thickness))
						{
							*line.value = (float)dummy;
							mvApp::GetApp()->getCallbackRegistry().addCallback(line.callback, line.name, nullptr);
						}
					}
					else
					{
						if (ImPlot::DragLineX(line.name.c_str(), &dummy, line.show_label, line.color, line.thickness))
						{
							*line.value = (float)dummy;
							mvApp::GetApp()->getCallbackRegistry().addCallback(line.callback, line.name, nullptr);
						}
					}
				}
			}

			// drag points
			if (m_showDragPoints)
			{
				for (auto& point : m_dragPoints)
				{
					if (ImPlot::DragPoint(point.name.c_str(), (double*)point.value.get(), (double*)(&point.value.get()[1]), point.show_label, point.color, point.radius))
					{
						mvApp::GetApp()->getCallbackRegistry().addCallback(point.callback, point.name, nullptr);
					}
				}
			}

			ImPlot::PopColormap();

			m_queried = ImPlot::IsPlotQueried();

			if (m_queried)
			{
				auto area = ImPlot::GetPlotQuery();
				m_queryArea[0] = (float)area.X.Min;
				m_queryArea[1] = (float)area.X.Max;
				m_queryArea[2] = (float)area.Y.Min;
				m_queryArea[3] = (float)area.Y.Max;
			}

			if (m_queryCallback != nullptr && m_queried)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					PyObject* area = PyTuple_New(4);
					PyTuple_SetItem(area, 0, PyFloat_FromDouble(m_queryArea[0]));
					PyTuple_SetItem(area, 1, PyFloat_FromDouble(m_queryArea[1]));
					PyTuple_SetItem(area, 2, PyFloat_FromDouble(m_queryArea[2]));
					PyTuple_SetItem(area, 3, PyFloat_FromDouble(m_queryArea[3]));
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_queryCallback, m_core_config.name, area);
					});
			}


			if (ImPlot::IsPlotHovered())
				mvInput::setPlotMousePosition((float)ImPlot::GetPlotMousePos().x, (float)ImPlot::GetPlotMousePos().y);

			m_xlimits_actual.x = ImPlot::GetPlotLimits().X.Min;
			m_xlimits_actual.y = ImPlot::GetPlotLimits().X.Max;
			m_ylimits_actual.x = ImPlot::GetPlotLimits().Y.Min;
			m_ylimits_actual.y = ImPlot::GetPlotLimits().Y.Max;
			m_y2limits_actual.x = ImPlot::GetPlotLimits(ImPlotYAxis_2).Y.Min;
			m_y2limits_actual.y = ImPlot::GetPlotLimits(ImPlotYAxis_2).Y.Max;
			m_y3limits_actual.x = ImPlot::GetPlotLimits(ImPlotYAxis_3).Y.Min;
			m_y3limits_actual.y = ImPlot::GetPlotLimits(ImPlotYAxis_3).Y.Max;

			ImPlot::PushPlotClipRect();
			auto topleft = ImPlot::GetPlotPos();
			m_drawList->draw(ImPlot::GetPlotDrawList(), topleft.x, topleft.y);
			ImPlot::PopPlotClipRect();

			ImPlot::EndPlot();
		}

		ImGui::PopID();
	}

	void mvPlot::setXLimits(float x_min, float x_max)
	{
		m_setXLimits = true;
		m_xlimits = ImVec2(x_min, x_max);
	}

	void mvPlot::setYLimits(float y_min, float y_max)
	{
		m_setYLimits = true;
		m_ylimits = ImVec2(y_min, y_max);
	}

	void mvPlot::setY2Limits(float y_min, float y_max)
	{
		m_setY2Limits = true;
		m_y2limits = ImVec2(y_min, y_max);
	}

	void mvPlot::setY3Limits(float y_min, float y_max)
	{
		m_setY3Limits = true;
		m_y3limits = ImVec2(y_min, y_max);
	}

	void mvPlot::setXLimitsAuto()
	{
		m_setXLimits = false;
	}

	void mvPlot::setYLimitsAuto()
	{
		m_setYLimits = false;
	}

	void mvPlot::setY2LimitsAuto()
	{
		m_setY2Limits = false;
	}

	void mvPlot::setY3LimitsAuto()
	{
		m_setY3Limits = false;
	}

	bool mvPlot::isPlotQueried() const
	{
		return m_queried;
	}

	float* mvPlot::getPlotQueryArea()
	{
		return m_queryArea;
	}

#ifdef MV_CPP
#else

	void mvPlot::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		 

		if (PyObject* item = PyDict_GetItemString(dict, "x_axis_name"))m_xaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y_axis_name")) m_yaxisName = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_color_scale")) m_colormapscale = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_annotations")) m_showAnnotations = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_drag_lines")) m_showDragLines = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_drag_points")) m_showDragPoints = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_min")) m_scale_min = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_max")) m_scale_max = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_height")) m_scale_height = ToInt(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// plot flags
		flagop("no_legend",            ImPlotFlags_NoLegend,         m_flags);
		flagop("no_menus",             ImPlotFlags_NoMenus,          m_flags);
		flagop("no_box_select",        ImPlotFlags_NoBoxSelect,      m_flags);
		flagop("no_mouse_pos",         ImPlotFlags_NoMousePos,       m_flags);
		flagop("no_highlight",         ImPlotFlags_NoHighlight,      m_flags);
		flagop("no_child",             ImPlotFlags_NoChild,          m_flags);
		flagop("query",                ImPlotFlags_Query,            m_flags);
		flagop("crosshairs",           ImPlotFlags_Crosshairs,       m_flags);
		flagop("anti_aliased",         ImPlotFlags_AntiAliased,      m_flags);
		flagop("equal_aspects",        ImPlotFlags_Equal,            m_flags);
		flagop("yaxis2",               ImPlotFlags_YAxis2,           m_flags);
		flagop("yaxis3",               ImPlotFlags_YAxis3,           m_flags);

		// x axis flags
		flagop("xaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_xflags);
		flagop("xaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_xflags);
		flagop("xaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_xflags);
		flagop("xaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_xflags);
		flagop("xaxis_time",           ImPlotAxisFlags_Time,         m_xflags);
		flagop("xaxis_invert",         ImPlotAxisFlags_Invert,       m_xflags);
		flagop("xaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_xflags);
		flagop("xaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_xflags);

		// y axis flags
		flagop("yaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_yflags);
		flagop("yaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_yflags);
		flagop("yaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_yflags);
		flagop("yaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_yflags);
		flagop("yaxis_invert",         ImPlotAxisFlags_Invert,       m_yflags);
		flagop("yaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_yflags);
		flagop("yaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_yflags);

		// y2 axis flags
		flagop("y2axis_no_gridlines", ImPlotAxisFlags_NoGridLines,    m_y2flags);
		flagop("y2axis_no_tick_marks", ImPlotAxisFlags_NoTickMarks,   m_y2flags);
		flagop("y2axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y2flags);
		flagop("y2axis_log_scale", ImPlotAxisFlags_LogScale,          m_y2flags);
		flagop("y2axis_invert", ImPlotAxisFlags_Invert,               m_y2flags);
		flagop("y2axis_lock_min", ImPlotAxisFlags_LockMin,            m_y2flags);
		flagop("y2axis_lock_max", ImPlotAxisFlags_LockMax,            m_y2flags);

		// y3 axis flags
		flagop("y3axis_no_gridlines", ImPlotAxisFlags_NoGridLines,    m_y3flags);
		flagop("y3axis_no_tick_marks", ImPlotAxisFlags_NoTickMarks,   m_y3flags);
		flagop("y3axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y3flags);
		flagop("y3axis_log_scale", ImPlotAxisFlags_LogScale,          m_y3flags);
		flagop("y3axis_invert", ImPlotAxisFlags_Invert,               m_y3flags);
		flagop("y3axis_lock_min", ImPlotAxisFlags_LockMin,            m_y3flags);
		flagop("y3axis_lock_max", ImPlotAxisFlags_LockMax,            m_y3flags);

	}

	void mvPlot::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		 

		PyDict_SetItemString(dict, "x_axis_name", ToPyString(m_xaxisName));
		PyDict_SetItemString(dict, "y_axis_name", ToPyString(m_yaxisName));
		PyDict_SetItemString(dict, "show_color_scale", ToPyBool(m_colormapscale));
		PyDict_SetItemString(dict, "show_annotations", ToPyBool(m_showAnnotations));
		PyDict_SetItemString(dict, "show_drag_lines", ToPyBool(m_showDragLines));
		PyDict_SetItemString(dict, "show_drag_points", ToPyBool(m_showDragPoints));
		PyDict_SetItemString(dict, "scale_min", ToPyFloat(m_scale_min));
		PyDict_SetItemString(dict, "scale_max", ToPyFloat(m_scale_max));
		PyDict_SetItemString(dict, "scale_height", ToPyInt(m_scale_height));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// plot flags
		checkbitset("no_legend",            ImPlotFlags_NoLegend,         m_flags);
		checkbitset("no_menus",             ImPlotFlags_NoMenus,          m_flags);
		checkbitset("no_box_select",        ImPlotFlags_NoBoxSelect,      m_flags);
		checkbitset("no_mouse_pos",         ImPlotFlags_NoMousePos,       m_flags);
		checkbitset("no_highlight",         ImPlotFlags_NoHighlight,      m_flags);
		checkbitset("no_child",             ImPlotFlags_NoChild,          m_flags);
		checkbitset("query",                ImPlotFlags_Query,            m_flags);
		checkbitset("crosshairs",           ImPlotFlags_Crosshairs,       m_flags);
		checkbitset("anti_aliased",         ImPlotFlags_AntiAliased,      m_flags);
		checkbitset("equal_aspects",        ImPlotFlags_Equal,            m_flags);
		checkbitset("yaxis2",               ImPlotFlags_YAxis2,           m_flags);
		checkbitset("yaxis3",               ImPlotFlags_YAxis3,           m_flags);

		// x axis flags
		checkbitset("xaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_xflags);
		checkbitset("xaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_xflags);
		checkbitset("xaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_xflags);
		checkbitset("xaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_xflags);
		checkbitset("xaxis_time",           ImPlotAxisFlags_Time,         m_xflags);
		checkbitset("xaxis_invert",         ImPlotAxisFlags_Invert,       m_xflags);
		checkbitset("xaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_xflags);
		checkbitset("xaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_xflags);

		// y axis flags
		checkbitset("yaxis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_yflags);
		checkbitset("yaxis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_yflags);
		checkbitset("yaxis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_yflags);
		checkbitset("yaxis_log_scale",      ImPlotAxisFlags_LogScale,     m_yflags);
		checkbitset("yaxis_invert",         ImPlotAxisFlags_Invert,       m_yflags);
		checkbitset("yaxis_lock_min",       ImPlotAxisFlags_LockMin,      m_yflags);
		checkbitset("yaxis_lock_max",       ImPlotAxisFlags_LockMax,      m_yflags);

		// y2 axis flags
		checkbitset("y2axis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_y2flags);
		checkbitset("y2axis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_y2flags);
		checkbitset("y2axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y2flags);
		checkbitset("y2axis_log_scale",      ImPlotAxisFlags_LogScale,     m_y2flags);
		checkbitset("y2axis_invert",         ImPlotAxisFlags_Invert,       m_y2flags);
		checkbitset("y2axis_lock_min",       ImPlotAxisFlags_LockMin,      m_y2flags);
		checkbitset("y2axis_lock_max",       ImPlotAxisFlags_LockMax,      m_y2flags);

		// y3 axis flags
		checkbitset("y3axis_no_gridlines",   ImPlotAxisFlags_NoGridLines,  m_y3flags);
		checkbitset("y3axis_no_tick_marks",  ImPlotAxisFlags_NoTickMarks,  m_y3flags);
		checkbitset("y3axis_no_tick_labels", ImPlotAxisFlags_NoTickLabels, m_y3flags);
		checkbitset("y3axis_log_scale",      ImPlotAxisFlags_LogScale,     m_y3flags);
		checkbitset("y3axis_invert",         ImPlotAxisFlags_Invert,       m_y3flags);
		checkbitset("y3axis_lock_min",       ImPlotAxisFlags_LockMin,      m_y3flags);
		checkbitset("y3axis_lock_max",       ImPlotAxisFlags_LockMax,      m_y3flags);
	}

	PyObject* add_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* xAxisName = "";
		const char* yAxisName = "";

		// plot flags
		int no_legend = false;
		int no_menus = false;
		int no_box_select = false;
		int no_mouse_pos = false;
		int no_highlight = false;
		int no_child = false;
		int query = false;
		int crosshairs = false;
		int antialiased = false;
		int equal_aspects = false;
		int yaxis2 = false;
		int yaxis3 = false;

		// x axis flags
		int xaxis_no_gridlines = false;
		int xaxis_no_tick_marks = false;
		int xaxis_no_tick_labels = false;
		int xaxis_log_scale = false;
		int xaxis_time = false;
		int xaxis_invert = false;
		int xaxis_lock_min = false;
		int xaxis_lock_max = false;

		// y axis flags
		int yaxis_no_gridlines = false;
		int yaxis_no_tick_marks = false;
		int yaxis_no_tick_labels = false;
		int yaxis_log_scale = false;
		int yaxis_invert = false;
		int yaxis_lock_min = false;
		int yaxis_lock_max = false;

		// y2 axis flags
		int y2axis_no_gridlines = false;
		int y2axis_no_tick_marks = false;
		int y2axis_no_tick_labels = false;
		int y2axis_log_scale = false;
		int y2axis_invert = false;
		int y2axis_lock_min = false;
		int y2axis_lock_max = false;

		// y3 axis flags
		int y3axis_no_gridlines = false;
		int y3axis_no_tick_marks = false;
		int y3axis_no_tick_labels = false;
		int y3axis_log_scale = false;
		int y3axis_invert = false;
		int y3axis_lock_min = false;
		int y3axis_lock_max = false;

		const char* parent = "";
		const char* before = "";
		int width = -1;
		int height = -1;
		PyObject* query_callback = nullptr;

		int show_color_scale = false;
		float scale_min = 0.0f;
		float scale_max = 1.0f;
		int scale_height = 100;

		const char* label = "";
		int show = true;
		int show_annotations = true;
		int show_drag_lines = true;
		int show_drag_points = true;

		if (!(mvApp::GetApp()->getParsers())["add_plot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName,
			&no_legend, &no_menus, &no_box_select, &no_mouse_pos, &no_highlight, &no_child, &query, &crosshairs, 
			&antialiased, &equal_aspects,
			&yaxis2, &yaxis3,
			&xaxis_no_gridlines,
			&xaxis_no_tick_marks,
			&xaxis_no_tick_labels,
			&xaxis_log_scale,
			&xaxis_time,
			&xaxis_invert,
			&xaxis_lock_min,
			&xaxis_lock_max,
			&yaxis_no_gridlines,
			&yaxis_no_tick_marks,
			&yaxis_no_tick_labels,
			&yaxis_log_scale,
			&yaxis_invert,
			&yaxis_lock_min,
			&yaxis_lock_max,
			&y2axis_no_gridlines,
			&y2axis_no_tick_marks,
			&y2axis_no_tick_labels,
			&y2axis_log_scale,
			&y2axis_invert,
			&y2axis_lock_min,
			&y2axis_lock_max,
			&yaxis_no_gridlines,
			&y3axis_no_tick_marks,
			&y3axis_no_tick_labels,
			&y3axis_log_scale,
			&y3axis_invert,
			&y3axis_lock_min,
			&y3axis_lock_max,
			&parent, &before, &width, &height, &query_callback, &show_color_scale, &scale_min, &scale_max,
			&scale_height, &label, &show, &show_annotations, &show_drag_lines, &show_drag_points))
			return GetPyNone();

		if (query_callback)
			Py_XINCREF(query_callback);

		auto item = CreateRef<mvPlot>(name, query_callback);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);
		
		return GetPyNone();
	}

	PyObject* add_drag_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		const char* source = "";
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(0));
		float thickness = 1.0f;
		int y_line = false;
		int show_label = true;
		PyObject* callback = nullptr;
		float default_value = 0.0f;

		if (!(mvApp::GetApp()->getParsers())["add_drag_line"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &source, &color, &thickness, &y_line, &show_label, &callback, &default_value))
			return GetPyNone();

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		if (callback)
			Py_XINCREF(callback);

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());
		
		graph->updateDragLine(name, show_label, ToColor(color), thickness, y_line, callback, default_value, source);
		
		return GetPyNone();
	}

	PyObject* delete_drag_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["delete_drag_line"].parse(args, kwargs, __FUNCTION__,
			&plot, &name))
			return GetPyNone();

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());
		
		graph->deleteDragLine(name);

		return GetPyNone();
	}

	PyObject* add_drag_point(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		const char* source = "";
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-1));
		float radius = 4.0f;
		int show_label = true;
		PyObject* callback = nullptr;
		float default_x = 0.0f;
		float default_y = 0.0f;

		if (!(mvApp::GetApp()->getParsers())["add_drag_point"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &source, &color, &radius, &show_label, &callback, &default_x, &default_y))
			return GetPyNone();

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		if (callback)
			Py_XINCREF(callback);

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());
		double defaults[2] = { (double)default_x, (double)default_y };
		
		graph->updateDragPoint(name, show_label, ToColor(color), radius, callback, defaults, source);

		return GetPyNone();
	}

	PyObject* delete_drag_point(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["delete_drag_point"].parse(args, kwargs, __FUNCTION__,
			&plot, &name))
			return GetPyNone();

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->deleteDragPoint(name);
		
		return GetPyNone();
	}

	PyObject* add_annotation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* text;
		double x;
		double y;
		float xoffset;
		float yoffset;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(0));
		int clamped = true;
		const char* tag = "";

		if (!(mvApp::GetApp()->getParsers())["add_annotation"].parse(args, kwargs, __FUNCTION__,
			&plot, &text, &x, &y, &xoffset, &yoffset, &color, &clamped, &tag))
			return GetPyNone();

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateAnnotation(tag, x, y, xoffset, yoffset, ToColor(color), text, clamped);

		return GetPyNone();
	}

	PyObject* delete_annotation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["delete_annotation"].parse(args, kwargs, __FUNCTION__,
			&plot, &name))
			return GetPyNone();

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->deleteAnnotation(name);

		return GetPyNone();
	}

	PyObject* clear_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["clear_plot"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->clear();

		return GetPyNone();
	}

	PyObject* reset_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_xticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->resetXTicks();

		return GetPyNone();
	}

	PyObject* reset_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["reset_yticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->resetYTicks();

		return GetPyNone();
	}

	PyObject* set_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_xticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}
		graph->setXTicks(labels, locations);


		return GetPyNone();
	}

	PyObject* set_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(mvApp::GetApp()->getParsers())["set_yticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}
		graph->setYTicks(labels, locations);

		return GetPyNone();
	}

	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setXLimitsAuto();

		return GetPyNone();
	}

	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["set_plot_ylimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setYLimitsAuto();

		return GetPyNone();
	}

	PyObject* set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float xmin;
		float xmax;

		if (!(mvApp::GetApp()->getParsers())["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setXLimits(xmin, xmax);

		return GetPyNone();
	}

	PyObject* set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float ymin;
		float ymax;

		if (!(mvApp::GetApp()->getParsers())["set_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot, &ymin, &ymax))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->setYLimits(ymin, ymax);

		return GetPyNone();
	}

	PyObject* is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		return ToPyBool(graph->isPlotQueried());
	}

	PyObject* get_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		const ImVec2& lim = graph->getXLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* get_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		const ImVec2& lim = graph->getYLimits();
		return ToPyPair(lim.x, lim.y);
	}

	PyObject* get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(mvApp::GetApp()->getParsers())["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		float* result = graph->getPlotQueryArea();
		return Py_BuildValue("(ffff)", result[0], result[1], result[2], result[3]);
	}

	PyObject* delete_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* series;

		if (!(mvApp::GetApp()->getParsers())["delete_series"].parse(args, kwargs, __FUNCTION__, &plot, &series))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->deleteSeries(series);

		return GetPyNone();
	}

	PyObject* add_image_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		const char* value;
		PyObject* bounds_min;
		PyObject* bounds_max;

		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));

		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));

		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(255.0));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_image_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &value, &bounds_min, &bounds_max, &uv_min, &uv_max, &tintcolor, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, bounds_min)) return GetPyNone();
		if (!CheckList(plot, bounds_max)) return GetPyNone();

		auto mbounds_min = ToFloatVect(bounds_min);
		auto mbounds_max = ToFloatVect(bounds_max);
		auto muv_min = ToVec2(uv_min);
		auto muv_max = ToVec2(uv_max);
		auto mcolor = ToColor(tintcolor);

		auto series = CreateRef<mvImageSeries>(name, value, ImPlotPoint(mbounds_min[0], mbounds_min[1]), ImPlotPoint(mbounds_max[0], mbounds_max[1]),
			muv_min, muv_max, mcolor, (ImPlotYAxis_)axis);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_pie_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* values;
		PyObject* labels;
		float x;
		float y;
		float radius;
		int normalize = false;
		double angle = 90.0;
		const char* format = "%0.2f";
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_pie_series"].parse(args, kwargs, __FUNCTION__, &plot, &name,
			&values, &labels, &x,
			&y, &radius, &normalize, &angle, &format, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, values)) return GetPyNone();
		if (!CheckList(plot, labels)) return GetPyNone();

		auto avalues = ToFloatVect(values);
		auto alabels = ToStringVect(labels);

		if (avalues.size() != alabels.size())
		{
			ThrowPythonException(std::string(name) + " data list must be the same size.");
			return GetPyNone();
		}

		auto series = CreateRef<mvPieSeries>(name, &avalues, x, y, radius, normalize, angle, format, alabels, (ImPlotYAxis_)axis);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_line_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_line_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &color, &weight, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();

		auto mcolor = ToColor(color);
		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);

		if (!CheckArraySizes(plot, { &xs, &ys })) return GetPyNone();

		auto series = CreateRef<mvLineSeries>(name, &xs, &ys, mcolor, (ImPlotYAxis_)axis);
		series->setWeight(weight);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_stair_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_stair_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &color, &weight, &update_bounds))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();

		auto mcolor = ToColor(color);
		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);

		if (!CheckArraySizes(plot, { &xs, &ys })) return GetPyNone();

		auto series = CreateRef<mvStairSeries>(name, &xs, &ys, mcolor, (ImPlotYAxis_)axis);

		series->setWeight(weight);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_bar_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		float weight = 1.0f;
		int horizontal = false;
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_bar_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &weight, &horizontal, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();

		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);

		if (!CheckArraySizes(plot, { &xs, &ys })) return GetPyNone();

		if (xs.size() == 0)
			return GetPyNone();

		auto series = CreateRef<mvBarSeries>(name, &xs, &ys, horizontal, (ImPlotYAxis_)axis);
		series->setWeight(weight);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_shade_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y1;
		PyObject* y2 = nullptr;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-255));

		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_shade_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y1, &y2, &color, &fill, &weight, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y1)) return GetPyNone();

		auto mcolor = ToColor(color);
		auto mfill = ToColor(fill);
		auto xs = ToFloatVect(x);
		auto y1s = ToFloatVect(y1);

		std::vector<float> y2s;
		if (y2)
			y2s = ToFloatVect(y2);
		else
		{
			for (auto item : y1s)
				y2s.push_back(0.0f);
		}

		if (!CheckArraySizes(plot, { &xs, &y1s, &y2s })) return GetPyNone();

		if (xs.size() == 0)
			return GetPyNone();

		auto series = CreateRef<mvShadeSeries>(name, mcolor, mfill, &xs, &y1s, &y2s, (ImPlotYAxis_)axis);
		series->setWeight(weight);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_candle_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* dates;
		PyObject* opens;
		PyObject* highs;
		PyObject* lows;
		PyObject* closes;
		int tooltip = true;

		PyObject* bull_color = PyTuple_New(4);
		PyTuple_SetItem(bull_color, 0, PyLong_FromLong(0));
		PyTuple_SetItem(bull_color, 1, PyLong_FromLong(255));
		PyTuple_SetItem(bull_color, 2, PyLong_FromLong(113));
		PyTuple_SetItem(bull_color, 3, PyLong_FromLong(255));

		PyObject* bear_color = PyTuple_New(4);
		PyTuple_SetItem(bear_color, 0, PyLong_FromLong(218));
		PyTuple_SetItem(bear_color, 1, PyLong_FromLong(13));
		PyTuple_SetItem(bear_color, 2, PyLong_FromLong(79));
		PyTuple_SetItem(bear_color, 3, PyLong_FromLong(255));
		float weight = 0.25f;
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_candle_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &dates, &opens, &highs, &lows, &closes, &tooltip,
			&bull_color, &bear_color, &weight, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, dates)) return GetPyNone();
		if (!CheckList(plot, opens)) return GetPyNone();
		if (!CheckList(plot, highs)) return GetPyNone();
		if (!CheckList(plot, closes)) return GetPyNone();
		if (!CheckList(plot, lows)) return GetPyNone();

		auto mdates = ToFloatVect(dates);
		auto mopens = ToFloatVect(opens);
		auto mhighs = ToFloatVect(highs);
		auto mlows = ToFloatVect(lows);
		auto mcloses = ToFloatVect(closes);
		auto mbull = ToColor(bull_color);
		auto mbear = ToColor(bear_color);

		if (!CheckArraySizes(plot, { &mdates, &mopens, &mhighs, &mlows, &mcloses })) return GetPyNone();

		if (mdates.size() == 0)
			return GetPyNone();

		auto series = CreateRef<mvCandleSeries>(name, &mdates, &mopens, &mhighs, &mlows, &mcloses,
			weight, mbull, mbear, (ImPlotYAxis_)axis);
		series->setWeight(weight);
		std::string errorMessage;

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_scatter_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		int marker = ImPlotMarker_Circle;
		float size = 4.0f;
		float weight = 1.0f;
		PyObject* outline = PyTuple_New(4);
		PyTuple_SetItem(outline, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(outline, 1, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 2, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 3, PyLong_FromLong(-255));
		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int xy_data_format = false;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_scatter_series"].parse(args, kwargs, __FUNCTION__, &plot,
			&name, &x, &y, &marker,
			&size, &weight, &outline, &fill, &update_bounds, &xy_data_format, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();

		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);
		auto mmarkerOutlineColor = ToColor(outline);
		auto mmarkerFillColor = ToColor(fill);

		if (!CheckArraySizes(plot, { &xs, &ys })) return GetPyNone();

		auto series = CreateRef<mvScatterSeries>(name, &xs, &ys, marker, size, weight, mmarkerOutlineColor, mmarkerFillColor, (ImPlotYAxis_)axis);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_stem_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		int marker = 1;
		float size = 4.0f;
		float weight = 1.0f;
		PyObject* outline = PyTuple_New(4);
		PyTuple_SetItem(outline, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(outline, 1, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 2, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 3, PyLong_FromLong(-255));
		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_stem_series"].parse(args, kwargs, __FUNCTION__, &plot, &name,
			&x, &y, &marker,
			&size, &weight, &outline, &fill, &update_bounds, (ImPlotYAxis_)axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();

		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);
		auto mmarkerOutlineColor = ToColor(outline);
		auto mmarkerFillColor = ToColor(fill);

		if (!CheckArraySizes(plot, { &xs, &ys })) return GetPyNone();

		if (xs.size() == 0)
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(CreateRef<mvStemSeries>(name, &xs, &ys, marker, size, weight, mmarkerOutlineColor,
			mmarkerFillColor, (ImPlotYAxis_)axis), update_bounds);

		return GetPyNone();
	}

	PyObject* add_text_point(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		float x;
		float y;
		int vertical = false;
		int xoffset = 0;
		int yoffset = 0;
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_text_point"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &vertical, &xoffset, &yoffset, &update_bounds, &axis))
			return GetPyNone();

		std::vector<float> ax = { x };
		std::vector<float> ay = { y };

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(CreateRef<mvLabelSeries>(name, &ax, &ay, xoffset, yoffset, vertical, (ImPlotYAxis_)axis), update_bounds);

		return GetPyNone();
	}

	PyObject* add_area_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		PyObject* color;
		PyObject* fill;
		float weight = 1.0f;
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_area_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &color, &fill, &weight, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();

		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);

		if (!CheckArraySizes(plot, { &xs, &ys })) return GetPyNone();

		if (xs.size() == 0)
			return GetPyNone();

		auto mcolor = ToColor(color);
		auto mfill = ToColor(fill);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->deleteSeries(name);
		auto aseries = CreateRef<mvAreaSeries>(name, &xs, &ys, mcolor, mfill, (ImPlotYAxis_)axis);
		auto lseries = CreateRef<mvLineSeries>(name, &xs, &ys, mcolor, (ImPlotYAxis_)axis);
		aseries->setWeight(weight);
		lseries->setWeight(weight);
		graph->addSeries(aseries, update_bounds);
		graph->addSeries(lseries, update_bounds); // this allows our custom render to work

		return GetPyNone();
	}

	PyObject* add_error_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		PyObject* y;
		PyObject* negative;
		PyObject* positive;
		int horizontal = false;
		int update_bounds = true;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-255));
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_error_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &negative, &positive, &horizontal, &update_bounds, &color, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();
		if (!CheckList(plot, y)) return GetPyNone();
		if (!CheckList(plot, negative)) return GetPyNone();
		if (!CheckList(plot, positive)) return GetPyNone();

		auto xs = ToFloatVect(x);
		auto ys = ToFloatVect(y);
		auto negatives = ToFloatVect(negative);
		auto positives = ToFloatVect(positive);

		if (!CheckArraySizes(plot, { &xs, &ys, &negatives, &positives })) return GetPyNone();

		if (xs.size() == 0)
			return GetPyNone();

		auto mcolor = ToColor(color);
		auto series = CreateRef<mvErrorSeries>(name, &xs, &ys, &negatives, &positives, horizontal, mcolor, (ImPlotYAxis_)axis);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_heat_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* values;
		int rows;
		int columns;
		double scale_min;
		double scale_max;
		const char* format = "%0.1f";
		PyObject* bounds_min = PyTuple_New(2);
		PyTuple_SetItem(bounds_min, 0, PyLong_FromLong(0));
		PyTuple_SetItem(bounds_min, 1, PyLong_FromLong(0));
		PyObject* bounds_max = PyTuple_New(2);
		PyTuple_SetItem(bounds_max, 0, PyLong_FromLong(1));
		PyTuple_SetItem(bounds_max, 1, PyLong_FromLong(1));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_heat_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &values, &rows, &columns, &scale_min, &scale_max, &format,
			&bounds_min, &bounds_max, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, values)) return GetPyNone();

		auto mvalues = ToFloatVect(values);
		auto mbounds_min = ToVec2(bounds_min);
		auto mbounds_max = ToVec2(bounds_max);

		if (mvalues.size() == 0)
			return GetPyNone();

		if (mvalues.size() % columns != 0)
		{
			ThrowPythonException(std::string(name) + " series dimensions are wrong.");
			return GetPyNone();
		}

		if (rows * columns != mvalues.size())
		{
			ThrowPythonException(std::string(name) + " series dimensions do not match data sizes.");
			return GetPyNone();
		}

		auto series = CreateRef<mvHeatSeries>(name, &mvalues, rows, columns, scale_min,
			scale_max, format, mbounds_min, mbounds_max, (ImPlotYAxis_)axis);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_vline_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_vline_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &color, &weight, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();

		auto mcolor = ToColor(color);
		auto xs = ToFloatVect(x);

		auto series = CreateRef<mvVLineSeries>(name, &xs, mcolor, (ImPlotYAxis_)axis);
		series->setWeight(weight);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_hline_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* x;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(-255));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-255));
		int update_bounds = true;
		int axis = 0;

		if (!(mvApp::GetApp()->getParsers())["add_hline_series"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &color, &weight, &update_bounds, &axis))
			return GetPyNone();

		if (!CheckList(plot, x)) return GetPyNone();

		auto mcolor = ToColor(color);
		auto xs = ToFloatVect(x);

		auto series = CreateRef<mvHLineSeries>(name, &xs, mcolor, (ImPlotYAxis_)axis);
		series->setWeight(weight);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvPlot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot.get());

		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

#endif

	void mvPlot::InsertConstants(std::vector<std::pair<std::string, long>>& constants)
	{
		//-----------------------------------------------------------------------------
		// Plot Colors
		//-----------------------------------------------------------------------------
		constants.emplace_back("mvPlotCol_Line"          , 0); // plot line/outline color (defaults to a rotating color set)
		constants.emplace_back("mvPlotCol_Fill"         , 1); // plot fill color for bars (defaults to the current line color)
		constants.emplace_back("mvPlotCol_MarkerOutline", 2); // marker outline color (defaults to the current line color)
		constants.emplace_back("mvPlotCol_MarkerFill"   , 3); // marker fill color (defaults to the current line color)
		constants.emplace_back("mvPlotCol_ErrorBar"     , 4); // error bar color (defaults to ImGuiCol_Text)
		constants.emplace_back("mvPlotCol_FrameBg"      , 5); // plot frame background color (defaults to ImGuiCol_FrameBg)
		constants.emplace_back("mvPlotCol_PlotBg"       , 6); // plot area background color (defaults to ImGuiCol_WindowBg)
		constants.emplace_back("mvPlotCol_PlotBorder"   , 7); // plot area border color (defaults to ImGuiCol_Text)
		constants.emplace_back("mvPlotCol_XAxis"        , 8); // x-axis grid/label color (defaults to 25% ImGuiCol_Text)
		constants.emplace_back("mvPlotCol_YAxis"        , 9); // y-axis grid/label color (defaults to 25% ImGuiCol_Text)
		constants.emplace_back("mvPlotCol_YAxis2"       ,10); // 2nd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
		constants.emplace_back("mvPlotCol_YAxis3"       ,11); // 3rd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
		constants.emplace_back("mvPlotCol_Selection"    ,12); // box-selection color (defaults to yellow)
		constants.emplace_back("mvPlotCol_Query"        ,13); // box-query color (defaults to green)
		constants.emplace_back("mvPlotCol_COUNT"        ,14);

		//-----------------------------------------------------------------------------
		// Plot Marker Specifications
		//-----------------------------------------------------------------------------
		constants.emplace_back("mvPlotMarker_None"    , -1);  // no marker
		constants.emplace_back("mvPlotMarker_Circle"  ,  0);  // a circle marker will be rendered at each point
		constants.emplace_back("mvPlotMarker_Square"  ,  1);  // a square maker will be rendered at each point
		constants.emplace_back("mvPlotMarker_Diamond" ,  2);  // a diamond marker will be rendered at each point
		constants.emplace_back("mvPlotMarker_Up"      ,  3);  // an upward-pointing triangle marker will up rendered at each point
		constants.emplace_back("mvPlotMarker_Down"    ,  4);  // an downward-pointing triangle marker will up rendered at each point
		constants.emplace_back("mvPlotMarker_Left"    ,  5);  // an leftward-pointing triangle marker will up rendered at each point
		constants.emplace_back("mvPlotMarker_Right"   ,  6);  // an rightward-pointing triangle marker will up rendered at each point
		constants.emplace_back("mvPlotMarker_Cross"   ,  7);  // a cross marker will be rendered at each point (not filled)
		constants.emplace_back("mvPlotMarker_Plus"    ,  8);  // a plus marker will be rendered at each point (not filled)
		constants.emplace_back("mvPlotMarker_Asterisk",  9); // a asterisk marker will be rendered at each point (not filled)

		//-----------------------------------------------------------------------------
		// Built-in ImPlot Color maps
		//-----------------------------------------------------------------------------
		constants.emplace_back("mvPlotColormap_Default",   0); // ImPlot default colormap         (n=10)
		constants.emplace_back("mvPlotColormap_Deep"   ,   1); // a.k.a. matplotlib "Set1"        (n=9)
		constants.emplace_back("mvPlotColormap_Dark"   ,   2); // a.k.a. matplotlib "Set1"        (n=9)
		constants.emplace_back("mvPlotColormap_Pastel" ,   3); // a.k.a. matplotlib "Pastel1"     (n=9)
		constants.emplace_back("mvPlotColormap_Paired" ,   4); // a.k.a. matplotlib "Paired"      (n=12)
		constants.emplace_back("mvPlotColormap_Viridis",   5); // a.k.a. matplotlib "viridis"     (n=11)
		constants.emplace_back("mvPlotColormap_Plasma" ,   6); // a.k.a. matplotlib "plasma"      (n=11)
		constants.emplace_back("mvPlotColormap_Hot"    ,   7); // a.k.a. matplotlib/MATLAB "hot"  (n=11)
		constants.emplace_back("mvPlotColormap_Cool"   ,   8); // a.k.a. matplotlib/MATLAB "cool" (n=11)
		constants.emplace_back("mvPlotColormap_Pink"   ,   9); // a.k.a. matplotlib/MATLAB "pink" (n=11)
		constants.emplace_back("mvPlotColormap_Jet"    ,  10); // a.k.a. MATLAB "jet"             (n=11)
	}
}
