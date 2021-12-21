#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawText : public mvAppItem
    {

    public:

        explicit mvDrawText(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        mvVec4      _pos = { 0.0f, 0.0f, 0.0f, 1.0f };
        std::string _text;
        mvColor     _color;
        float       _size = 1.0f;
    };

}
