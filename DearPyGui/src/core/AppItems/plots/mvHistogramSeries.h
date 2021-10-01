#pragma once

#include "mvPlot.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvHistogramSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
    class mvHistogramSeries : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHistogramSeries, add_histogram_series)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvPlotAxis),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

        MV_CREATE_CONSTANT(mvPlotBin_Sqrt,    -1L);
        MV_CREATE_CONSTANT(mvPlotBin_Sturges, -2L);
        MV_CREATE_CONSTANT(mvPlotBin_Rice,    -3L);
        MV_CREATE_CONSTANT(mvPlotBin_Scott,   -4L);

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvPlotBin_Sqrt),
            MV_ADD_CONSTANT(mvPlotBin_Sturges),
            MV_ADD_CONSTANT(mvPlotBin_Rice),
            MV_ADD_CONSTANT(mvPlotBin_Scott),
        MV_END_CONSTANTS

    public:

        explicit mvHistogramSeries(mvUUID uuid);

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

        int _bins = -1;
        bool _cumlative = false;
        bool _density = false;
        bool _outliers = true;
        float _barScale = 1.0f;
        double _min = 0.0;
        double _max = 1.0;
        mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
            std::vector<std::vector<double>>{ std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{} });

    };

}
