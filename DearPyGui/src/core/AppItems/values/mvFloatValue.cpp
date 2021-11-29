#include "mvFloatValue.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvFloatValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
		std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

		args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });
        
		mvPythonParserSetup setup;
		setup.about = "Adds a float value.";
		setup.category = { "Widgets", "Values" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvFloatValue::mvFloatValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
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
		_value = *static_cast<std::shared_ptr<float>*>(item->getValue());
	}
}
