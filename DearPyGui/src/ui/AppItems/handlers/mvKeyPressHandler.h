#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvKeyPressHandler : public mvAppItem
    {

    public:

        explicit mvKeyPressHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        int _key = -1;

    };

}
