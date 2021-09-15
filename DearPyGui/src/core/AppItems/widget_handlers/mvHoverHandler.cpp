#include "mvHoverHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

    void mvHoverHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        mvPythonParserSetup setup;
        setup.about = "Adds a hover handler.";
        setup.category = { "Widgets", "Events" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvHoverHandler::mvHoverHandler(mvUUID uuid)
        :
        mvAppItem(uuid)
    {

    }

    void mvHoverHandler::customAction(void* data)
    {

        if (static_cast<mvAppItemState*>(data)->hovered)
        {
            GContext->callbackRegistry->submitCallback([=]()
                {
                    if (_alias.empty())
                        GContext->callbackRegistry->runCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
                    else
                        GContext->callbackRegistry->runCallback(getCallback(false), _alias, GetPyNone(), _user_data);
                });
        }
    }
}
