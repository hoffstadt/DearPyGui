#include <utility>
#include "mvPieSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvPieSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
		args.push_back({ mvPyDataType::Double, "radius" });
		args.push_back({ mvPyDataType::DoubleList, "values"});
		args.push_back({ mvPyDataType::StringList, "labels"});
		args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.2f'" });
		args.push_back({ mvPyDataType::Double, "angle", mvArgType::KEYWORD_ARG, "90.0" });
		args.push_back({ mvPyDataType::Bool, "normalize", mvArgType::KEYWORD_ARG, "False" });

		mvPythonParserSetup setup;
		setup.about = "Adds an pie series to a plot.";
		setup.category = { "Plotting", "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

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

	void mvPieSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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

		mvPyObject py_format = ToPyString(_format);
		mvPyObject py_x = ToPyDouble(_x);
		mvPyObject py_y = ToPyDouble(_y);
		mvPyObject py_radius = ToPyDouble(_radius);
		mvPyObject py_angle = ToPyDouble(_angle);
		mvPyObject py_normalize = ToPyBool(_normalize);
		mvPyObject py_labels = ToPyList(_labels);

		PyDict_SetItemString(dict, "format", py_format);
		PyDict_SetItemString(dict, "x", py_x);
		PyDict_SetItemString(dict, "y", py_y);
		PyDict_SetItemString(dict, "radius", py_radius);
		PyDict_SetItemString(dict, "angle", py_angle);
		PyDict_SetItemString(dict, "normalize", py_normalize);
		PyDict_SetItemString(dict, "labels", py_labels);
	}

	void mvPieSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvPieSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_x = titem->_x;
		_y = titem->_y;
		_radius = titem->_radius;
		_normalize = titem->_normalize;
		_angle = titem->_angle;
		_format = titem->_format;
		_labels = titem->_labels;
		_clabels = titem->_clabels;
	}
}