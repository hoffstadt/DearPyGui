#include "mvInt4Value.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvInt4Value::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
		std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

		args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });
        
		mvPythonParserSetup setup;
		setup.about = "Adds a int4 value.";
		setup.category = { "Widgets", "Values" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

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
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::array<int, 4>>*>(item->getValue());
	}
}
