#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvLog.h"
#include "mvUtilities.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvIntPtrBase::mvIntPtrBase(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvIntPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<int>>(item->getValue());
	}

	PyObject* mvIntPtrBase::getPyValue()
	{
		return ToPyInt(*_value);
	}

	void mvIntPtrBase::setPyValue(PyObject* value)
	{
		*_value = ToInt(value);
	}

	mvInt4PtrBase::mvInt4PtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvInt4PtrBase::getPyValue()
	{
		return ToPyIntList(_value->data(), 4);
	}

	void mvInt4PtrBase::setPyValue(PyObject* value)
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

	void mvInt4PtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::array<int, 4>>>(item->getValue());
	}

	mvFloatPtrBase::mvFloatPtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvFloatPtrBase::getPyValue()
	{
		return ToPyFloat(*_value);
	}

	void mvFloatPtrBase::setPyValue(PyObject* value)
	{
		*_value = ToFloat(value);
	}

	void mvFloatPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<float>>(item->getValue());
	}

	mvDoublePtrBase::mvDoublePtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvDoublePtrBase::getPyValue()
	{
		return ToPyDouble(*_value);
	}

	void mvDoublePtrBase::setPyValue(PyObject* value)
	{
		*_value = ToDouble(value);
	}

	void mvDoublePtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<double>>(item->getValue());
	}

	mvFloat4PtrBase::mvFloat4PtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvFloat4PtrBase::getPyValue()
	{
		return ToPyFloatList(_value->data(), 4);
	}

	void mvFloat4PtrBase::setPyValue(PyObject* value)
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

	void mvFloat4PtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::array<float, 4>>>(item->getValue());
	}

	mvDouble4PtrBase::mvDouble4PtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvDouble4PtrBase::getPyValue()
	{
		return ToPyFloatList(_value->data(), 4);
	}

	void mvDouble4PtrBase::setPyValue(PyObject* value)
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

	void mvDouble4PtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::array<double, 4>>>(item->getValue());
	}

	mvColorPtrBase::mvColorPtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvColorPtrBase::getPyValue()
	{
		// nasty hack
		int r = (int)(_value->data()[0] * 255.0f * 255.0f);
		int g = (int)(_value->data()[1] * 255.0f * 255.0f);
		int b = (int)(_value->data()[2] * 255.0f * 255.0f);
		int a = (int)(_value->data()[3] * 255.0f * 255.0f);

		auto color = mvColor(r, g, b, a);
		return ToPyColor(color);
	}

	void mvColorPtrBase::setPyValue(PyObject* value)
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

	void mvColorPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::array<float, 4>>>(item->getValue());
	}

	mvBoolPtrBase::mvBoolPtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvBoolPtrBase::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvBoolPtrBase::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvBoolPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<bool>>(item->getValue());
	}

	mvStringPtrBase::mvStringPtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvStringPtrBase::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvStringPtrBase::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
	}

	void mvStringPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::string>>(item->getValue());
	}

	mvTimePtrBase::mvTimePtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvTimePtrBase::getPyValue()
	{
		return ToPyTime(*_value);
	}

	void mvTimePtrBase::setPyValue(PyObject* value)
	{

		if (value)
			*_value = ToTime(value);
		else
			_value = {};

		*_imvalue = ImPlot::MkGmtTime(_value.get());
	}

	mvFloatVectPtrBase::mvFloatVectPtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvFloatVectPtrBase::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvFloatVectPtrBase::setPyValue(PyObject* value)
	{
		*_value = ToFloatVect(value);
	}

	void mvFloatVectPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<std::vector<float>>>(item->getValue());
	}

	mvSeriesBase::mvSeriesBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
		_label = "Series###" + std::to_string(_uuid);
	}

	PyObject* mvSeriesBase::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvSeriesBase::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvSeriesBase::setDataSource(mvUUID dataSource)
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

	mvUUIDPtrBase::mvUUIDPtrBase(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvUUIDPtrBase::getPyValue()
	{
		return ToPyUUID(*_value);
	}

	void mvUUIDPtrBase::setPyValue(PyObject* value)
	{
		*_value = ToUUID(value);
	}

	void mvUUIDPtrBase::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<mvUUID>>(item->getValue());
	}
}