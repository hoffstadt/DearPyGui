#include "mvColorValue.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    mvColorValue::mvColorValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
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
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
	}
}
