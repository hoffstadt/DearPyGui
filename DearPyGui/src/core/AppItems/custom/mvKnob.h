#pragma once

#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>

namespace Marvel {

    MV_REGISTER_WIDGET(mvKnobFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvKnobFloat : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvKnobFloat, add_knob_float)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

    public:

        mvKnobFloat(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        mvRef<float> _value = CreateRef<float>(0.0f);
        float        _disabled_value = 0.0f;
        float        _min = 0.0f;
        float        _max = 100.0f;
        float        _step = 50.0f;

    };

}