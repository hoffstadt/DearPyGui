#pragma once

#include "mvItemRegistry.h"
#include "dearpygui.h"
#include "cpp.hint"

class mvTextureRegistry : public mvAppItem
{

public:

    explicit mvTextureRegistry(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void show_debugger();

private:

    int _selection = -1;
};

class mvStaticTexture : public mvAppItem
{

public:

    explicit mvStaticTexture(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvStaticTexture();

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

    void markDirty() { _dirty = true; }

public:

    mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
    void* _texture = nullptr;
    bool                      _dirty = true;
    int                       _permWidth = 0;
    int                       _permHeight = 0;

};

class mvRawTexture : public mvAppItem
{

    enum class ComponentType {
        MV_FLOAT_COMPONENT,
        MV_INT_COMPONENT,
    };

public:

    explicit mvRawTexture(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvRawTexture();

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

public:

    PyObject* _buffer = nullptr;
    void* _value = nullptr;
    void* _texture = nullptr;
    bool          _dirty = true;
    ComponentType _componentType = ComponentType::MV_FLOAT_COMPONENT;
    int           _components = 4;
    int           _permWidth = 0;
    int           _permHeight = 0;

};

class mvDynamicTexture : public mvAppItem
{

public:

    explicit mvDynamicTexture(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvDynamicTexture();

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

public:

    mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
    void* _texture = nullptr;
    bool                      _dirty = true;
    int                       _permWidth = 0;
    int                       _permHeight = 0;

};