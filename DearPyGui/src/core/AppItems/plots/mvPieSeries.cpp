#include <utility>
#include "mvPieSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvPieSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a pie series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Double>("x");
		parser.addArg<mvPyDataType::Double>("y");
		parser.addArg<mvPyDataType::Double>("radius");
		parser.addArg<mvPyDataType::DoubleList>("values");
		parser.addArg<mvPyDataType::StringList>("labels");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.2f'");

		parser.addArg<mvPyDataType::Double>("angle", mvArgType::KEYWORD_ARG, "90.0");

		parser.addArg<mvPyDataType::Bool>("normalize", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvPieSeries::mvPieSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvPieSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvPieSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvPieSeries::setDataSource(mvUUID dataSource)
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

	void mvPieSeries::draw(ImDrawList* drawlist, float x, float y)
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

			ImPlot::PlotPieChart(_clabels.data(), xptr->data(), (int)_labels.size(),
				_x, _y, _radius, _normalize, _format.c_str(), _angle);

			// Begin a popup for a legend entry.
			if (ImPlot::BeginLegendPopup(_internalLabel.c_str(), 1))
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

	void mvPieSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_x = ToDouble(item);
				break;

			case 1:
				_y = ToDouble(item);
				break;

			case 2:
				_radius = ToDouble(item);
				break;

			case 3:
				(*_value)[0] = ToDoubleVect(item);
				break;

			case 4:
				_labels = ToStringVect(item);
				_clabels.clear();
				for (const auto& label : _labels)
					_clabels.push_back(label.c_str());
				break;

			default:
				break;
			}
		}
	}

	void mvPieSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x")) _x = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y")) _y = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "angle")) _angle = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "normalize")) _normalize = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "labels"))
		{
			_labels = ToStringVect(item);
			_clabels.clear();
			for (const auto& label : _labels)
				_clabels.push_back(label.c_str());
		}

		if (PyObject* item = PyDict_GetItemString(dict, "values")) { (*_value)[0] = ToDoubleVect(item); }


	}

	void mvPieSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "format", ToPyString(_format));
		PyDict_SetItemString(dict, "x", ToPyDouble(_x));
		PyDict_SetItemString(dict, "y", ToPyDouble(_y));
		PyDict_SetItemString(dict, "radius", ToPyDouble(_radius));
		PyDict_SetItemString(dict, "angle", ToPyDouble(_angle));
		PyDict_SetItemString(dict, "normalize", ToPyBool(_normalize));
		PyDict_SetItemString(dict, "labels", ToPyList(_labels));
	}

}