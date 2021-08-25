#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvSlider3D, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
    class mvSlider3D : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSlider3D, add_3d_slider)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

    public:

        explicit mvSlider3D(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
        float                       _disabled_value[4]{};
        float                       _minX = 0.0f;
        float                       _minY = 0.0f;
        float                       _minZ = 0.0f;
        float                       _maxX = 100.0f;
        float                       _maxY = 100.0f;
        float                       _maxZ = 100.0f;
        float                       _scale = 1.0f;

    };

}