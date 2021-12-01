#include "mvIntValue.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvIntValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::Integer, "default_value", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });
        
        mvPythonParserSetup setup;
        setup.about = "Adds a int value.";
        setup.category = { "Widgets", "Values" };
        setup.returnType = mvPyDataType::UUID;
        
        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvIntValue::mvIntValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvIntValue::setDataSource(mvUUID dataSource)
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

}
