#include <utility>
#include "mvLabelSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvLabelSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::Double, "x"});
		args.push_back({ mvPyDataType::Double, "y"});
		args.push_back({ mvPyDataType::Integer, "x_offset", mvArgType::KEYWORD_ARG});
		args.push_back({ mvPyDataType::Integer, "y_offset", mvArgType::KEYWORD_ARG});
		args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "False" });

		mvPythonParserSetup setup;
		setup.about = "Adds a label series to a plot.";
		setup.category = { "Plotting", "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvLabelSeries::mvLabelSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvLabelSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvLabelSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvLabelSeries::setDataSource(mvUUID dataSource)
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

	void mvLabelSeries::draw(ImDrawList* drawlist, float x, float y)
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

			xptr = &(*_value.get())[0];
			yptr = &(*_value.get())[1];

			ImPlot::PlotText(_internalLabel.c_str(), (*xptr)[0], (*yptr)[0], _vertical,
				ImVec2((float)_xoffset, (float)_yoffset));

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

	void mvLabelSeries::handleSpecificRequiredArgs(PyObject* dict)
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

			default:
				break;
			}
		}
	}

	void mvLabelSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) _vertical = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x_offset")) _xoffset = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y_offset")) _yoffset = ToInt(item);

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { (*_value)[1] = ToDoubleVect(item); }

	}

	void mvLabelSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_vertical = ToPyBool(_vertical);
		mvPyObject py_x_offset = ToPyInt(_xoffset);
		mvPyObject py_y_offset = ToPyInt(_yoffset);

		PyDict_SetItemString(dict, "vertical", py_vertical);
		PyDict_SetItemString(dict, "x_offset", py_x_offset);
		PyDict_SetItemString(dict, "y_offset", py_y_offset);
	}

	void mvLabelSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvLabelSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_xoffset = titem->_xoffset;
		_yoffset = titem->_yoffset;
		_vertical = titem->_vertical;
	}
}