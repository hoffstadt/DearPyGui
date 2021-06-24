#include "mvFloatValue.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>
#include "mvItemRegistry.h"

namespace Marvel {

    void mvFloatValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Widgets", "Values"});
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SOURCE)
        );

        parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvFloatValue::mvFloatValue(mvUUID uuid)
        : mvFloatPtrBase(uuid)
    {
    }

}
