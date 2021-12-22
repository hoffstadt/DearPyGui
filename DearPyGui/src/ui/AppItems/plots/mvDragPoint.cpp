#include <utility>
#include "mvDragPoint.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	mvDragPoint::mvDragPoint(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvDragPoint::getPyValue()
	{
		return ToPyFloatList(_value->data(), 4);
	}

	void mvDragPoint::setPyValue(PyObject* value)
	{
		std::vector<double> temp = ToDoubleVect(value);
		while (temp.size() < 4)
			temp.push_back(0.0);
		std::array<double, 4> temp_array;
		for (size_t i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (_value)
			*_value = temp_array;
		else
			_value = std::make_shared<std::array<double, 4>>(temp_array);
	}

	void mvDragPoint::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::array<double, 4>>*>(item->getValue());
	}

	void mvDragPoint::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!config.show)
			return;

		ScopedID id(uuid);

		static double dummyx = (*_value.get())[0];
		static double dummyy = (*_value.get())[1];
		dummyx = (*_value.get())[0];
		dummyy = (*_value.get())[1];

		if (ImPlot::DragPoint(config.specifiedLabel.c_str(), &dummyx, &dummyy, _show_label, _color, _radius))
		{
			(*_value.get())[0] = dummyx;
			(*_value.get())[1] = dummyy;
			mvAddCallback(config.callback, uuid, nullptr, config.user_data);
		}

	}

	void mvDragPoint::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) _show_label = ToBool(item);

	}

	void mvDragPoint::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_color = ToPyColor(_color);
		mvPyObject py_radius = ToPyFloat(_radius);
		mvPyObject py_show_label = ToPyBool(_show_label);

		PyDict_SetItemString(dict, "color", py_color);
		PyDict_SetItemString(dict, "radius", py_radius);
		PyDict_SetItemString(dict, "show_label", py_show_label);
	}

	void mvDragPoint::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDragPoint*>(item);
		if (config.source != 0) _value = titem->_value;
		_disabled_value[0] = titem->_disabled_value[0];
		_disabled_value[1] = titem->_disabled_value[1];
		_disabled_value[2] = titem->_disabled_value[2];
		_disabled_value[3] = titem->_disabled_value[3];
		_show_label = titem->_show_label;
		_color = titem->_color;
		_radius = titem->_radius;
	}

}