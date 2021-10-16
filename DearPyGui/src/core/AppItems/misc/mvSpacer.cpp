#include "mvSpacer.h"
#include "mvItemRegistry.h"

namespace Marvel {

    void mvSpacer::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        mvPythonParserSetup setup;
        setup.about = "Adds a spacer item that can be used to help with layouts or can be used as a placeholder item.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }


    mvSpacer::mvSpacer(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvSpacer::draw(ImDrawList* drawlist, float x, float y)
    {
        if(_width == 0 && _height == 0)
            ImGui::Spacing();
        else
            ImGui::Dummy({ (float)_width, (float)_height });
    }

}
