#include <utility>
#include "mvHistogramSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvHistogramSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a histogram series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");


		parser.addArg<mvPyDataType::Integer>("bins", mvArgType::KEYWORD_ARG, "-1");
		parser.addArg<mvPyDataType::Float>("bar_scale", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Double>("min_range", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Double>("max_range", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Bool>("cumlative", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("density", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("outliers", mvArgType::KEYWORD_ARG, "True");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHistogramSeries::mvHistogramSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvHistogramSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvHistogramSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvHistogramSeries::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
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
		_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
	}

	void mvHistogramSeries::draw(ImDrawList* drawlist, float x, float y)
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

			static const std::vector<double>* xptr;

			xptr = &(*_value.get())[0];

			ImPlot::PlotHistogram(_internalLabel.c_str(), xptr->data(), (int)xptr->size(), _bins,
				_cumlative, _density, ImPlotRange(_min, _max), _outliers, (double)_barScale);

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
						item->getState().update();
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

	void mvHistogramSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				(*_value)[0] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvHistogramSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "bins")) { _bins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "bar_scale")) { _barScale = ToFloat(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "min_range")) { _min = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "max_range")) { _max = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "cumlative")) { _cumlative = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "density")) { _density = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "outliers")) { _outliers = ToBool(item); }

	}

	void mvHistogramSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	void mvHistogramSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvHistogramSeries*>(item);
		_cumlative = titem->_cumlative;
		if(_source != 0) _value = titem->_value;
		_bins = titem->_bins;
		_density = titem->_density;
		_outliers = titem->_outliers;
		_min = titem->_min;
		_max = titem->_max;
		_barScale = titem->_barScale;

	}

}