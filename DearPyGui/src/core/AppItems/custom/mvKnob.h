#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>

namespace Marvel {

    class mvKnobFloat : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvKnobFloat, add_knob_float)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvKnobFloat(mvUUID uuid);

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

        mvRef<float> _value = CreateRef<float>(0.0f);
        float        _disabled_value = 0.0f;
        float        _min = 0.0f;
        float        _max = 100.0f;
        float        _step = 50.0f;

    };

}
