#include <utility>
#include "mvDragLine.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	mvDragLine::mvDragLine(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDragLine::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!config.show)
			return;

		ScopedID id(uuid);

		if (_vertical)
		{
			if (ImPlot::DragLineX(config.specifiedLabel.c_str(), _value.get(), _show_label, _color, _thickness))
			{
				mvAddCallback(config.callback, uuid, nullptr, config.user_data);
			}
		}
		else
		{
			if (ImPlot::DragLineY(config.specifiedLabel.c_str(), _value.get(), _show_label, _color, _thickness))
			{
				mvAddCallback(config.callback, uuid, nullptr, config.user_data);
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
		if (dataSource == config.source) return;
		config.source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
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
		if (config.source != 0) _value = titem->_value;
		_disabled_value = titem->_disabled_value;
		_show_label = titem->_show_label;
		_color = titem->_color;
		_thickness = titem->_thickness;
		_vertical = titem->_vertical;
	}

}