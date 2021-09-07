#include <utility>
#include "mvErrorSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvErrorSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds an error series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");
		parser.addArg<mvPyDataType::DoubleList>("y");
		parser.addArg<mvPyDataType::DoubleList>("negative");
		parser.addArg<mvPyDataType::DoubleList>("positive");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");
		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvErrorSeries::mvErrorSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvErrorSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvErrorSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvErrorSeries::setDataSource(mvUUID dataSource)
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

	void mvErrorSeries::draw(ImDrawList* drawlist, float x, float y)
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
			static const std::vector<double>* yptr;
			static const std::vector<double>* zptr;
			static const std::vector<double>* wptr;

			xptr = &(*_value.get())[0];
			yptr = &(*_value.get())[1];
			zptr = &(*_value.get())[2];
			wptr = &(*_value.get())[3];

			if (_horizontal)
				ImPlot::PlotErrorBarsH(_internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());
			else
				ImPlot::PlotErrorBars(_internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());

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

	void mvErrorSeries::handleSpecificRequiredArgs(PyObject* dict)
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
				(*_value)[1] = ToDoubleVect(item);
				break;

			case 2:
				(*_value)[2] = ToDoubleVect(item);
				break;

			case 3:
				(*_value)[3] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvErrorSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal= ToBool(item);

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { (*_value)[1] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "negative")) { (*_value)[2] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "positive")) { (*_value)[3] = ToDoubleVect(item); }

	}

	void mvErrorSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_horizontal = ToPyBool(_horizontal);

		PyDict_SetItemString(dict, "horizontal", py_horizontal);
	}

	void mvErrorSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvErrorSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_horizontal = titem->_horizontal;
	}
}