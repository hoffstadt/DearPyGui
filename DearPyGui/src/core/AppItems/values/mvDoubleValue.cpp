#include "mvDoubleValue.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvDoubleValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Widgets", "Values"});
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        parser.addArg<mvPyDataType::Double>("default_value", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)");
        parser.finalize();

        parsers->insert({ s_command, parser });
    }

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
		_value = std::get<std::shared_ptr<double>>(item->getValue());
	}

}
