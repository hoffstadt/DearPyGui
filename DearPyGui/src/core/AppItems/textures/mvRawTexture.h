#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvRawTexture, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
    class mvRawTexture : public mvAppItem
    {

        enum class ComponentType {
            MV_FLOAT_COMPONENT,
            MV_INT_COMPONENT,
        };

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvRawTexture, add_raw_texture)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_SET_STATES(MV_STATE_NONE);

        MV_CREATE_CONSTANT(mvFormat_Float_rgba, 0);
        MV_CREATE_CONSTANT(mvFormat_Float_rgb, 1);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTextureRegistry)
        MV_END_PARENTS

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvFormat_Float_rgba),
            MV_ADD_CONSTANT(mvFormat_Float_rgb),
        MV_END_CONSTANTS

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
