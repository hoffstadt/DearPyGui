#include "mvSeriesValue.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvSeriesValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
		std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

		args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "()" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });
        
		mvPythonParserSetup setup;
		setup.about = "Adds a plot series value.";
		setup.category = { "Widgets", "Values" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvSeriesValue::mvSeriesValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

	PyObject* mvSeriesValue::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvSeriesValue::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvSeriesValue::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
	}

}
