#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvCheckbox : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCheckbox, add_checkbox)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvCheckbox(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
        // values
        void setDataSource(mvUUID dataSource) override;
        void setPyValue(PyObject* value) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;

    private:

        mvRef<bool> _value = CreateRef<bool>(false);
        bool        _disabled_value = false;

    };

}
