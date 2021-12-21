#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    class mvFontRange : public mvAppItem
    {

    public:

        explicit mvFontRange(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        const std::array<ImWchar, 3>& getRange() const { return _range; }

    private:
        
        int _min = 0x0370;
        int _max = 0x03ff;
        std::array<ImWchar, 3> _range = { 0x0370, 0x03ff, 0 };

    };

}
