#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvHeatSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a heat series to a plot. Typically a color scale widget is also added to show the legend.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");
		parser.addArg<mvPyDataType::Integer>("rows");
		parser.addArg<mvPyDataType::Integer>("cols");


		parser.addArg<mvPyDataType::Double>("scale_min", mvArgType::KEYWORD_ARG, "0.0", "Sets the color scale min. Typically paired with the color scale widget scale_min.");
		parser.addArg<mvPyDataType::Double>("scale_max", mvArgType::KEYWORD_ARG, "1.0", "Sets the color scale max. Typically paired with the color scale widget scale_max.");

		parser.addArg<mvPyDataType::DoubleList>("bounds_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		parser.addArg<mvPyDataType::DoubleList>("bounds_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.1f'");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

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
		_value = std::get<std::shared_ptr<std::vector<std::vector<double>>>>(item->getValue());
	}

	bool mvHeatSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
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

		// handle enabled theming
		if (_enabled)
		{
			// push class theme (if it exists)
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

			// push item theme (if it exists)
			if (_theme)
				static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{

			static const std::vector<double>* xptr;

			xptr = &(*_value.get())[0];


			ImPlot::PlotHeatmap(_label.c_str(), xptr->data(), _rows, _cols, _scale_min, _scale_max,
				_format.c_str(), { _bounds_min.x, _bounds_min.y }, { _bounds_max.x, _bounds_max.y });

			// Begin a popup for a legend entry.
			if (ImPlot::BeginLegendPopup(_label.c_str(), 1))
			{
				for (auto& childset : _children)
				{
					for (auto& item : childset)
					{
						// skip item if it's not shown
						if (!item->isShown())
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

		// handle popping styles
		if (_enabled)
		{
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->customAction();

			if (_theme)
				static_cast<mvTheme*>(_theme.get())->customAction();
		}
	}

	void mvHeatSeries::handleSpecificRequiredArgs(PyObject* dict)
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

		PyDict_SetItemString(dict, "format", ToPyString(_format));
		PyDict_SetItemString(dict, "rows", ToPyInt(_rows));
		PyDict_SetItemString(dict, "cols", ToPyInt(_cols));
		PyDict_SetItemString(dict, "bounds_min", ToPyPair(_bounds_min.x, _bounds_min.y));
		PyDict_SetItemString(dict, "bounds_max", ToPyPair(_bounds_max.x, _bounds_max.y));
		PyDict_SetItemString(dict, "scale_min", ToPyDouble(_scale_min));
		PyDict_SetItemString(dict, "scale_max", ToPyDouble(_scale_max));
	}

}