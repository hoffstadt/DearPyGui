#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

    class mvNodeLink : public mvAppItem
    {

    public:

        explicit mvNodeLink(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void getSpecificConfiguration(PyObject* dict) override;
        
        int getId() const { return _id0; }
        int getId1() const { return _id1; }
        int getId2() const { return _id2; }

    public:

        int _id0 = 0;
        int _id1 = 0;
        int _id2 = 0;

        mvUUID _id1uuid = 0;
        mvUUID _id2uuid = 0;
    };

}
