#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDragPoint : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragPoint, add_drag_point)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvPlot)
        MV_END_PARENTS

    public:

        explicit mvDragPoint(mvUUID uuid);

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

        mvRef<std::array<double, 4>> _value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
        double                       _disabled_value[4]{};
        bool                         _show_label = true;
        mvColor                      _color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
        float                        _radius = 4.0f;

    };

}
