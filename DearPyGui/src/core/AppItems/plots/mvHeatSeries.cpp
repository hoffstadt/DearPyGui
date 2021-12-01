#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvHeatSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::DoubleList, "x"});
		args.push_back({ mvPyDataType::Integer, "rows"});
		args.push_back({ mvPyDataType::Integer, "cols"});
		args.push_back({ mvPyDataType::Double, "scale_min", mvArgType::KEYWORD_ARG, "0.0", "Sets the color scale min. Typically paired with the color scale widget scale_min." });
		args.push_back({ mvPyDataType::Double, "scale_max", mvArgType::KEYWORD_ARG, "1.0", "Sets the color scale max. Typically paired with the color scale widget scale_max." });
		args.push_back({ mvPyDataType::DoubleList, "bounds_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)"});
		args.push_back({ mvPyDataType::DoubleList, "bounds_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)"});
		args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.1f'" });
		args.push_back({ mvPyDataType::Bool, "contribute_to_bounds", mvArgType::KEYWORD_ARG, "True" });

		mvPythonParserSetup setup;
		setup.about = "Adds a heat series to a plot.";
		setup.category = { "Plotting", "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvHeatSeries::mvHeatSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvHeatSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvHeatSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvHeatSeries::setDataSource(mvUUID dataSource)
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

	void mvHeatSeries::draw(ImDrawList* drawlist, float x, float y)
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


			ImPlot::PlotHeatmap(_internalLabel.c_str(), xptr->data(), _rows, _cols, _scale_min, _scale_max,
				_format.c_str(), { _bounds_min.x, _bounds_min.y }, { _bounds_max.x, _bounds_max.y });

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

	void mvHeatSeries::handleSpecificRequiredArgs(PyObject* dict)
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
				_rows = ToInt(item);
				break;

			case 2:
				_cols = ToInt(item);
				break;


			default:
				break;
			}
		}

		(*_value)[1].push_back(_bounds_min.y);
		(*_value)[1].push_back(_bounds_max.y);
	}

	void mvHeatSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "rows")) _rows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "cols")) _cols = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_min")) _bounds_min = ToPoint(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_max")) _bounds_max = ToPoint(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_min")) _scale_min = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_max")) _scale_max = ToDouble(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*_value)[0] = ToDoubleVect(item); }

		if (valueChanged)
		{
			(*_value)[1].push_back(_bounds_min.y);
			(*_value)[1].push_back(_bounds_max.y);
		}

	}

	void mvHeatSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_format = ToPyString(_format);
		mvPyObject py_rows = ToPyInt(_rows);
		mvPyObject py_cols = ToPyInt(_cols);
		mvPyObject py_bounds_min = ToPyPair(_bounds_min.x, _bounds_min.y);
		mvPyObject py_bounds_max = ToPyPair(_bounds_max.x, _bounds_max.y);
		mvPyObject py_scale_min = ToPyDouble(_scale_min);
		mvPyObject py_scale_max = ToPyDouble(_scale_max);

		PyDict_SetItemString(dict, "format", py_format);
		PyDict_SetItemString(dict, "rows", py_rows);
		PyDict_SetItemString(dict, "cols", py_cols);
		PyDict_SetItemString(dict, "bounds_min", py_bounds_min);
		PyDict_SetItemString(dict, "bounds_max", py_bounds_max);
		PyDict_SetItemString(dict, "scale_min", py_scale_min);
		PyDict_SetItemString(dict, "scale_max", py_scale_max);
	}

	void mvHeatSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvHeatSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_rows = titem->_rows;
		_cols = titem->_cols;
		_scale_min = titem->_scale_min;
		_scale_max = titem->_scale_max;
		_format = titem->_format;
		_bounds_min = titem->_bounds_min;
		_bounds_max = titem->_bounds_max;
	}
}