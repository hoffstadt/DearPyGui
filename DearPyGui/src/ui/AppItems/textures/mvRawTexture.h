#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvRawTexture : public mvAppItem
    {

        enum class ComponentType {
            MV_FLOAT_COMPONENT,
            MV_INT_COMPONENT,
        };

    public:

        explicit mvRawTexture(mvUUID uuid);
        ~mvRawTexture();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

        void* getRawTexture() { return _texture; }

    private:

        PyObject*     _buffer = nullptr;
        void*         _value = nullptr;
        void*         _texture = nullptr;
        bool          _dirty = true;
        ComponentType _componentType = ComponentType::MV_FLOAT_COMPONENT;
        int           _components = 4;
        int           _permWidth = 0;
        int           _permHeight = 0;

    };

}
