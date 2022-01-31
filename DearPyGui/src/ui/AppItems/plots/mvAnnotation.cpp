#include <utility>
#include "mvAnnotation.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

mvAnnotation::mvAnnotation(mvUUID uuid)
	: mvAppItem(uuid)
{
}

PyObject* mvAnnotation::getPyValue()
{
	return ToPyFloatList(_value->data(), 4);
}

void mvAnnotation::setPyValue(PyObject* value)
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

void mvAnnotation::setDataSource(mvUUID dataSource)
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

void mvAnnotation::draw(ImDrawList* drawlist, float x, float y)
{
	if (!config.show)
		return;

	ScopedID id(uuid);

	if (_clamped)
		ImPlot::AnnotateClamped((*_value.get())[0], (*_value.get())[1], _pixOffset, _color.toVec4(), "%s", config.specifiedLabel.c_str());
	else
		ImPlot::Annotate((*_value.get())[0], (*_value.get())[1], _pixOffset, _color.toVec4(), "%s", config.specifiedLabel.c_str());

}

void mvAnnotation::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "clamped")) _clamped = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "offset")) _pixOffset = ToVec2(item);

}

void mvAnnotation::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_color = ToPyColor(_color);
	mvPyObject py_clamped = ToPyBool(_clamped);
	mvPyObject py_offset = ToPyPair(_pixOffset.x, _pixOffset.y);

	PyDict_SetItemString(dict, "color", py_color);
	PyDict_SetItemString(dict, "clamped", py_clamped);
	PyDict_SetItemString(dict, "offset", py_offset);

}

void mvAnnotation::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvAnnotation*>(item);
	if (config.source != 0) _value = titem->_value;
	_disabled_value[0] = titem->_disabled_value[0];
	_disabled_value[1] = titem->_disabled_value[1];
	_disabled_value[2] = titem->_disabled_value[2];
	_disabled_value[3] = titem->_disabled_value[3];
	_color = titem->_color;
	_clamped = titem->_clamped;
	_pixOffset = titem->_pixOffset;
}