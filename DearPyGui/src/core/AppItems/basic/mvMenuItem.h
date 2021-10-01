#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvMenuItem, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
    class mvMenuItem : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenuItem, add_menu_item)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

    public:

        explicit mvMenuItem(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<bool> _value = CreateRef<bool>(false);
        bool        _disabled_value = false;
        std::string _shortcut;
        bool        _check = false;

    };

}
