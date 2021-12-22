#include "mvFloatVectValue.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    mvFloatVectValue::mvFloatVectValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
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
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
	}

}
