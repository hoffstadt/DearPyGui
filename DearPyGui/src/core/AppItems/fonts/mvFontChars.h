#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFontChars : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontChars, add_font_chars)

    public:

        explicit mvFontChars(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        const std::vector<ImWchar>& getCharacters() const { return _chars; }

    private:
        
        std::vector<ImWchar>  _chars;

    };

}
