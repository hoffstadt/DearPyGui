#pragma once

#include "mvPlot.h"

namespace Marvel {

    class mvCandleSeries : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>*parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCandleSeries, add_candle_series)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvPlotAxis),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry)
        MV_END_PARENTS

    public:

        explicit mvCandleSeries(mvUUID uuid);

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

        float _weight = 0.25f;
        bool _tooltip = true;
        mvColor _bullColor = { 0, 255, 113, 255};
        mvColor _bearColor = { 218, 13, 79, 255 };
        mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
            std::vector<std::vector<double>>{ std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{} });

    };

}
