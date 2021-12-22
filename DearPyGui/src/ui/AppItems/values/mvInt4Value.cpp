#include "mvInt4Value.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    mvInt4Value::mvInt4Value(mvUUID uuid)
        : mvAppItem(uuid)
    {
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
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::array<int, 4>>*>(item->getValue());
	}
}
