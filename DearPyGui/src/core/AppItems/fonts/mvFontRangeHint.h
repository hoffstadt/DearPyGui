/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/
#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvFontRangeHint, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
    class mvFontRangeHint : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRangeHint, add_font_range_hint)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvFont),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

        MV_CREATE_CONSTANT(mvFontRangeHint_Default, 0);
        MV_CREATE_CONSTANT(mvFontRangeHint_Japanese, 1);
        MV_CREATE_CONSTANT(mvFontRangeHint_Korean, 2);
        MV_CREATE_CONSTANT(mvFontRangeHint_Chinese_Full, 3);
        MV_CREATE_CONSTANT(mvFontRangeHint_Chinese_Simplified_Common, 4);
        MV_CREATE_CONSTANT(mvFontRangeHint_Cyrillic, 5);
        MV_CREATE_CONSTANT(mvFontRangeHint_Thai, 6);
        MV_CREATE_CONSTANT(mvFontRangeHint_Vietnamese, 7);

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvFontRangeHint_Default),
            MV_ADD_CONSTANT(mvFontRangeHint_Japanese),
            MV_ADD_CONSTANT(mvFontRangeHint_Korean),
            MV_ADD_CONSTANT(mvFontRangeHint_Chinese_Full),
            MV_ADD_CONSTANT(mvFontRangeHint_Chinese_Simplified_Common),
            MV_ADD_CONSTANT(mvFontRangeHint_Cyrillic),
            MV_ADD_CONSTANT(mvFontRangeHint_Thai),
            MV_ADD_CONSTANT(mvFontRangeHint_Vietnamese),
        MV_END_CONSTANTS

    public:

        explicit mvFontRangeHint(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        int getHint() const { return _hint; }

    private:
        
        int _hint = 0;

    };

}
