#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvCharRemap : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCharRemap, add_char_remap)

    public:

        explicit mvCharRemap(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void handleSpecificRequiredArgs(PyObject* dict) override;
        int getSourceChar() const { return _source; }
        int getTargetChar() const { return _target; }

    private:
        
        int _source = 0;
        int _target = 0;

    };

}
