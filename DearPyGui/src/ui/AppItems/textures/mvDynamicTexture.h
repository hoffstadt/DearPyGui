#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDynamicTexture : public mvAppItem
    {

    public:

        explicit mvDynamicTexture(mvUUID uuid);
        ~mvDynamicTexture();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        
    public:

        mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
        void*                     _texture = nullptr;
        bool                      _dirty = true;
        int                       _permWidth = 0;
        int                       _permHeight = 0;

    };

}
