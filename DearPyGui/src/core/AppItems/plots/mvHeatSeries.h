#pragma once

#include "mvPlot.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvHeatSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
    class mvHeatSeries : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHeatSeries, add_heat_series)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvPlotAxis),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

    public:

        explicit mvHeatSeries(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        int         _rows = 1;
        int         _cols = 1;
        double      _scale_min = 0.0;
        double      _scale_max = 1.0;
        std::string _format = "%0.1f";
        mvPlotPoint _bounds_min = {0.0, 0.0};
        mvPlotPoint _bounds_max = {1.0, 1.0};
        mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
            std::vector<std::vector<double>>{ std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{} });

    };

}
