#include "mvStringValue.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvItemRegistry.h"

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
        : mvStringPtrBase(uuid)
    {
    }

}
