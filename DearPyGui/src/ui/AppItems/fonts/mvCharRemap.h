#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvCharRemap : public mvAppItem
    {

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
