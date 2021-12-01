#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvRadioButton : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvRadioButton, add_radio_button)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvRadioButton(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setPyValue(PyObject* value) override;
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;

    private:

        void updateIndex();

    private:

        mvRef<std::string>       _value = CreateRef<std::string>("");
        std::string              _disabled_value = "";
        std::vector<std::string> _itemnames;
        bool                     _horizontal = false;
        int                      _index = 0;
        int                      _disabledindex = 0;
    };

}
