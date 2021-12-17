#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFontRangeHint : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRangeHint, add_font_range_hint)

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
