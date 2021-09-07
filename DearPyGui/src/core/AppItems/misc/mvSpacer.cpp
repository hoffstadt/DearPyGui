#include "mvSpacer.h"
#include "mvItemRegistry.h"

namespace Marvel {

    void mvSpacer::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Adds a spacer.", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        parser.finalize();

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
