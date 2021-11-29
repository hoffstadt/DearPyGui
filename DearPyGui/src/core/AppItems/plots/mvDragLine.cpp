#include <utility>
#include "mvDragLine.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvDragLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
		args.push_back({ mvPyDataType::Bool, "show_label", mvArgType::KEYWORD_ARG, "True" });
		args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "True" });

		mvPythonParserSetup setup;
		setup.about = "Adds a drag line to a plot.";
		setup.category = { "Plotting", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDragLine::mvDragLine(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDragLine::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!_show)
			return;

		ScopedID id(_uuid);

		if (_vertical)
		{
			if (ImPlot::DragLineX(_specifiedLabel.c_str(), _value.get(), _show_label, _color, _thickness))
			{
				mvAddCallback(_callback, _uuid, nullptr, _user_data);
			}
		}
		else
		{
			if (ImPlot::DragLineY(_specifiedLabel.c_str(), _value.get(), _show_label, _color, _thickness))
			{
				mvAddCallback(_callback, _uuid, nullptr, _user_data);
			}
		}

	}

	PyObject* mvDragLine::getPyValue()
	{
		return ToPyDouble(*_value);
	}

	void mvDragLine::setPyValue(PyObject* value)
	{
		*_value = ToDouble(value);
	}

	void mvDragLine::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<double>*>(item->getValue());
	}

	void mvDragLine::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) _show_label = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) _vertical = ToBool(item);

	}

	void mvDragLine::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_color = ToPyColor(_color);
		mvPyObject py_thickness = ToPyFloat(_thickness);
		mvPyObject py_show_label = ToPyBool(_show_label);
		mvPyObject py_vertical = ToPyBool(_vertical);

		PyDict_SetItemString(dict, "color", py_color);
		PyDict_SetItemString(dict, "thickness", py_thickness);
		PyDict_SetItemString(dict, "show_label", py_show_label);
		PyDict_SetItemString(dict, "vertical", py_vertical);
	}

	void mvDragLine::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDragLine*>(item);
		if(_source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_show_label = titem->_show_label;
		_color = titem->_color;
		_thickness = titem->_thickness;
		_vertical = titem->_vertical;
	}

}