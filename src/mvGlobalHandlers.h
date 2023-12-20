#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"

class mvHandlerRegistry : public mvAppItem
{
public:
    explicit mvHandlerRegistry(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
};

class mvKeyDownHandler : public mvAppItem
{
public:
    ImGuiKey _key = -1;
    explicit mvKeyDownHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvKeyPressHandler : public mvAppItem
{
public:
    ImGuiKey _key = -1;
    explicit mvKeyPressHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvKeyReleaseHandler : public mvAppItem
{
public:
    ImGuiKey _key = -1;
    explicit mvKeyReleaseHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvMouseClickHandler : public mvAppItem
{
public:
    int _button = -1;
    explicit mvMouseClickHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvMouseDoubleClickHandler : public mvAppItem
{
public:
    int _button = -1;
    explicit mvMouseDoubleClickHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvMouseDownHandler : public mvAppItem
{
public:
    int _button = -1;
    explicit mvMouseDownHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvMouseDragHandler : public mvAppItem
{
public:
    int   _button = -1;
    float _threshold = 10.0f;
    explicit mvMouseDragHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvMouseMoveHandler : public mvAppItem
{
public:
    ImVec2 _oldPos = {};
    explicit mvMouseMoveHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
};

class mvMouseReleaseHandler : public mvAppItem
{
public:
    int _button = -1;
    explicit mvMouseReleaseHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvMouseWheelHandler : public mvAppItem
{
public:
    explicit mvMouseWheelHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override;
};