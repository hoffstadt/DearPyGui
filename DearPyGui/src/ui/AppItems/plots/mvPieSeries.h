#pragma once

#include "mvPlot.h"

namespace Marvel {

    class mvPieSeries : public mvAppItem
    {

    public:

        explicit mvPieSeries(mvUUID uuid);

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

        double _x = 0.0;
        double _y = 0.0;
        double _radius = 0.5;
        bool _normalize = true;
        double _angle = 90.0;
        std::string _format;
        std::vector<std::string> _labels;
        std::vector<const char*> _clabels;

        mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
            std::vector<std::vector<double>>{ std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{},
            std::vector<double>{} });

    };

}
