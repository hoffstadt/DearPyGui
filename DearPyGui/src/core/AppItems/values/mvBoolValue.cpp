#include "mvBoolValue.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvBoolValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
		std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

		args.push_back({ mvPyDataType::Bool, "default_value", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });
        
		mvPythonParserSetup setup;
		setup.about = "Adds a bool value.";
		setup.category = { "Widgets", "Values" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvBoolValue::mvBoolValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

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
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}
}
