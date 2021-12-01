#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvStaticTexture : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStaticTexture, add_static_texture)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTextureRegistry)
        MV_END_PARENTS

    public:

        explicit mvStaticTexture(mvUUID uuid);
        ~mvStaticTexture();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        
        void* getRawTexture() { return _texture; }
        void markDirty() { _dirty = true; }

    private:

        mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
        void*                     _texture = nullptr;
        bool                      _dirty = true;
        int                       _permWidth = 0;
        int                       _permHeight = 0;

    };

}
