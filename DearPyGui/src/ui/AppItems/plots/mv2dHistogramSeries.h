#pragma once

#include "mvPlot.h"

namespace Marvel {

    class mv2dHistogramSeries : public mvAppItem
    {

    public:

        explicit mv2dHistogramSeries(mvUUID uuid);

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

        mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
            std::vector<std::vector<double>>{ std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{} });

        int _xbins = -1;
        int _ybins = -1;
        bool _density = false;
        bool _outliers = true;
        double _xmin = 0.0;
        double _xmax = 1.0;
        double _ymin = 0.0;
        double _ymax = 1.0;

    };

}
