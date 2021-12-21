#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

    class mvNodeAttribute : public mvAppItem
    {

        enum class AttributeType { 
            mvAttr_Input = 0L, 
            mvAttr_Output, 
            mvAttr_Static
        };

    public:

        explicit mvNodeAttribute(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        int getId() const {return _id;}

    private:

        int                _id = 1;
        AttributeType     _attrType = AttributeType::mvAttr_Input;
        imnodes::PinShape _shape = imnodes::PinShape_CircleFilled;
        std::string       _category = "general";

    };

}
