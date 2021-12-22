#include "mvDoubleValue.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    mvDoubleValue::mvDoubleValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
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
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<double>*>(item->getValue());
	}

}
