#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFont : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFont, add_font)

        MV_CREATE_COMMAND(bind_font);
        MV_CREATE_COMMAND(get_text_size);

        MV_START_COMMANDS
            MV_ADD_COMMAND(bind_font);
            MV_ADD_COMMAND(get_text_size);
        MV_END_COMMANDS

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvFontRegistry)
        MV_END_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvFontChars),
            MV_ADD_CHILD(mvAppItemType::mvFontRange),
            MV_ADD_CHILD(mvAppItemType::mvCharRemap),
            MV_ADD_CHILD(mvAppItemType::mvFontRangeHint),
            MV_ADD_CHILD(mvAppItemType::mvTemplateRegistry),
        MV_END_CHILDREN

    public:

        explicit mvFont(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        ImFont* getFontPtr() { return _fontPtr; }

    public:

        // config
        std::string _file;
        float       _size = 13.0f;
        bool        _default = false;

        // finalized
        ImFont* _fontPtr = nullptr;
        ImVector<ImWchar> _ranges;

    };

}
