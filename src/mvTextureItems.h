#pragma once

#include "mvItemRegistry.h"
#include "dearpygui.h"

class mvTextureRegistry : public mvAppItem
{

public:

    explicit mvTextureRegistry(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void show_debugger();

private:

    int _selection = -1;
};

class mvTextureItem : public mvAppItem
{

public:

    explicit mvTextureItem(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvTextureItem();

    // Must be used as a getter instead of directly accessing `_texture`.
    // For built-in textures like font atlas this may return a texture other than
    // `_texture`, and the texture reference may change between frames.
    ImTextureRef getTexRef() const;

public:

    ImTextureID               _texture = ImTextureID_Invalid;
};


class mvStaticTexture : public mvTextureItem
{

public:

    explicit mvStaticTexture(mvUUID uuid) : mvTextureItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

    void markDirty() { _dirty = true; }

public:

    std::shared_ptr<std::vector<float>> _value = std::make_shared<std::vector<float>>(std::vector<float>{0.0f});
    bool                      _dirty = true;
    int                       _permWidth = 0;
    int                       _permHeight = 0;

};

class mvRawTexture : public mvTextureItem
{

    enum class ComponentType {
        MV_FLOAT_COMPONENT,
        MV_INT_COMPONENT,
    };

public:

    explicit mvRawTexture(mvUUID uuid) : mvTextureItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    // values
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

public:

    mvPyObject _buffer = nullptr;
    void* _value = nullptr;
    bool          _dirty = true;
    ComponentType _componentType = ComponentType::MV_FLOAT_COMPONENT;
    int           _components = 4;
    int           _permWidth = 0;
    int           _permHeight = 0;

};

class mvDynamicTexture : public mvTextureItem
{

public:

    explicit mvDynamicTexture(mvUUID uuid) : mvTextureItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

public:

    std::shared_ptr<std::vector<float>> _value = std::make_shared<std::vector<float>>(std::vector<float>{0.0f});
    bool                      _dirty = true;
    int                       _permWidth = 0;
    int                       _permHeight = 0;

};