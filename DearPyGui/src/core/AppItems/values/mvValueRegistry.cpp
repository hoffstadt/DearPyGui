#include "mvValueRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvValueRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Values", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvValueRegistry::mvValueRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvValueRegistry::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvBoolValue) return true;
		if (type == mvAppItemType::mvIntValue) return true;
		if (type == mvAppItemType::mvInt4Value) return true;
		if (type == mvAppItemType::mvFloatValue) return true;
		if (type == mvAppItemType::mvFloat4Value) return true;
		if (type == mvAppItemType::mvStringValue) return true;
		if (type == mvAppItemType::mvDoubleValue) return true;
		if (type == mvAppItemType::mvDouble4Value) return true;
		if (type == mvAppItemType::mvColorValue) return true;
		if (type == mvAppItemType::mvFloatVectValue) return true;
		if (type == mvAppItemType::mvSeriesValue) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleChild, s_command,
			"Incompatible child. Acceptable children include: mv*Value", this);

		MV_ITEM_REGISTRY_ERROR("Drawing children must be draw commands only.");
		assert(false);

		return false;
	}

}