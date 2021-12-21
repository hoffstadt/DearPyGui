#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFontChars : public mvAppItem
    {

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
