#pragma once

#include "mvPlot.h"
#include "mvTextureItems.h"

class mvImageSeries : public mvAppItem
{

public:

    explicit mvImageSeries(mvUUID uuid) : mvAppItem(uuid) {}

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

    // config
    mvUUID      _textureUUID = 0;
    mvPlotPoint _bounds_min = {0.0, 0.0};
    mvPlotPoint _bounds_max = {0.0, 0.0};
    mvVec2      _uv_min = { 0.0f, 0.0f };
    mvVec2      _uv_max = { 1.0f, 1.0f };
    mvColor     _tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        
    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> _texture = nullptr;
    bool _internalTexture = false; // create a local texture if necessary

    mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });

};