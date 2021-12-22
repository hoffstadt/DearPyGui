#include "mvStringValue.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    mvStringValue::mvStringValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
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
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}
}
