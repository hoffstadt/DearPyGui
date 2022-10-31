#include "mvValues.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvPyUtils.h"

PyObject* mvBoolValue::getPyValue()
{
	return ToPyBool(*_value);
}

void mvBoolValue::setPyValue(PyObject* value)
{
	*_value = ToBool(value);
}

void mvBoolValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
}

PyObject* mvColorValue::getPyValue()
{
	// nasty hack
	int r = (int)(_value->data()[0] * 255.0f * 255.0f);
	int g = (int)(_value->data()[1] * 255.0f * 255.0f);
	int b = (int)(_value->data()[2] * 255.0f * 255.0f);
	int a = (int)(_value->data()[3] * 255.0f * 255.0f);

	auto color = mvColor(r, g, b, a);
	return ToPyColor(color);
}

void mvColorValue::setPyValue(PyObject* value)
{
	mvColor color = ToColor(value);
	std::array<float, 4> temp_array;
	temp_array[0] = color.r;
	temp_array[1] = color.g;
	temp_array[2] = color.b;
	temp_array[3] = color.a;
	if (_value)
		*_value = temp_array;
	else
		_value = std::make_shared<std::array<float, 4>>(temp_array);
}

void mvColorValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
}

PyObject* mvDouble4Value::getPyValue()
{
	return ToPyFloatList(_value->data(), 4);
}

void mvDouble4Value::setPyValue(PyObject* value)
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

void mvDouble4Value::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::array<double, 4>>*>(item->getValue());
}

PyObject* mvDoubleValue::getPyValue()
{
	return ToPyDouble(*_value);
}

void mvDoubleValue::setPyValue(PyObject* value)
{
	*_value = ToDouble(value);
}

void mvDoubleValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<double>*>(item->getValue());
}

PyObject* mvFloat4Value::getPyValue()
{
	return ToPyFloatList(_value->data(), 4);
}

void mvFloat4Value::setPyValue(PyObject* value)
{
	std::vector<float> temp = ToFloatVect(value);
	while (temp.size() < 4)
		temp.push_back(0.0f);
	std::array<float, 4> temp_array;
	for (size_t i = 0; i < temp_array.size(); i++)
		temp_array[i] = temp[i];
	if (_value)
		*_value = temp_array;
	else
		_value = std::make_shared<std::array<float, 4>>(temp_array);
}

void mvFloat4Value::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
}

PyObject* mvFloatValue::getPyValue()
{
	return ToPyFloat(*_value);
}

void mvFloatValue::setPyValue(PyObject* value)
{
	*_value = ToFloat(value);
}

void mvFloatValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<float>*>(item->getValue());
}

PyObject* mvFloatVectValue::getPyValue()
{
	return ToPyList(*_value);
}

void mvFloatVectValue::setPyValue(PyObject* value)
{
	*_value = ToFloatVect(value);
}

void mvFloatVectValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
}

PyObject* mvInt4Value::getPyValue()
{
	return ToPyIntList(_value->data(), 4);
}

void mvInt4Value::setPyValue(PyObject* value)
{
	std::vector<int> temp = ToIntVect(value);
	while (temp.size() < 4)
		temp.push_back(0);
	std::array<int, 4> temp_array;
	for (size_t i = 0; i < temp_array.size(); i++)
		temp_array[i] = temp[i];
	if (_value)
		*_value = temp_array;
	else
		_value = std::make_shared<std::array<int, 4>>(temp_array);
}

void mvInt4Value::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::array<int, 4>>*>(item->getValue());
}

void mvIntValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<int>*>(item->getValue());
}

PyObject* mvIntValue::getPyValue()
{
	return ToPyInt(*_value);
}

void mvIntValue::setPyValue(PyObject* value)
{
	*_value = ToInt(value);
}

PyObject* mvSeriesValue::getPyValue()
{
	return ToPyList(*_value);
}

void mvSeriesValue::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvSeriesValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
}

PyObject* mvStringValue::getPyValue()
{
	return ToPyString(*_value);
}

void mvStringValue::setPyValue(PyObject* value)
{
	*_value = ToString(value);
}

void mvStringValue::setDataSource(mvUUID dataSource)
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
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
}