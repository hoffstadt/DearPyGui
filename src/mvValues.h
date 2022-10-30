#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"
#include <array>
#include <string>

class mvValueRegistry : public mvAppItem
{
public:
    explicit mvValueRegistry(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
};

class mvBoolValue : public mvAppItem
{

public:

    explicit mvBoolValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<bool> _value = std::make_shared<bool>(false);
    bool  _disabled_value = false;
};

class mvColorValue : public mvAppItem
{

public:

    explicit mvColorValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::array<float, 4>> _value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float  _disabled_value[4]{};

};

class mvDouble4Value : public mvAppItem
{

public:

    explicit mvDouble4Value(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

protected:

    std::shared_ptr<std::array<double, 4>> _value = std::make_shared<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double  _disabled_value[4]{};
};

class mvDoubleValue : public mvAppItem
{

public:

    explicit mvDoubleValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<double> _value = std::make_shared<double>(0.0);
    float         _disabled_value = 0.0;

};

class mvFloat4Value : public mvAppItem
{

public:

    explicit mvFloat4Value(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::array<float, 4>> _value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float  _disabled_value[4]{};

};

class mvFloatValue : public mvAppItem
{

public:

    explicit mvFloatValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<float> _value = std::make_shared<float>(0.0f);
    float  _disabled_value = 0.0f;

};

class mvFloatVectValue : public mvAppItem
{

public:

    explicit mvFloatVectValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::vector<float>> _value = std::make_shared<std::vector<float>>(std::vector<float>{0.0f});

};

class mvInt4Value : public mvAppItem
{

public:

    explicit mvInt4Value(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::array<int, 4>> _value = std::make_shared<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
    int  _disabled_value[4]{};
};

class mvIntValue : public mvAppItem
{

public:

    explicit mvIntValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<int> _value = std::make_shared<int>(0);
    int        _disabled_value = 0;

};

class mvSeriesValue : public mvAppItem
{

public:

    explicit mvSeriesValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::vector<std::vector<double>>> _value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });

};

class mvStringValue : public mvAppItem
{

public:

    explicit mvStringValue(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::string> _value = std::make_shared<std::string>("");
    std::string  _disabled_value = "";
};