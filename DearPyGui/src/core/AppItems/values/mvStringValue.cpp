#include "mvStringValue.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvStringValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Widgets", "Values"});
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
        parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)");
        parser.finalize();

        parsers->insert({ s_command, parser });
    }

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
		_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}
}
