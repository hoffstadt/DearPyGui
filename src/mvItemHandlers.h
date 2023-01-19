#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"

class mvItemHandlerRegistry : public mvAppItem
{

public:
    explicit mvItemHandlerRegistry(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void checkEvents(void* data = nullptr);
    void onBind(mvAppItem* item);
};

class mvActivatedHandler : public mvAppItem
{
public:
    explicit mvActivatedHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};

class mvActiveHandler : public mvAppItem
{
public:
    explicit mvActiveHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};

class mvClickedHandler : public mvAppItem
{
public:
    int _button = -1;
    explicit mvClickedHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvDoubleClickedHandler : public mvAppItem
{
public:
    int _button = -1;
    explicit mvDoubleClickedHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvDeactivatedAfterEditHandler : public mvAppItem
{
public:
    explicit mvDeactivatedAfterEditHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvDeactivatedHandler : public mvAppItem
{
public:
    explicit mvDeactivatedHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvEditedHandler : public mvAppItem
{
public:
    explicit mvEditedHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvFocusHandler : public mvAppItem
{
public:
    explicit mvFocusHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvHoverHandler : public mvAppItem
{
public:
    explicit mvHoverHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvResizeHandler : public mvAppItem
{
public:
    explicit mvResizeHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvToggledOpenHandler : public mvAppItem
{
public:
    explicit mvToggledOpenHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvVisibleHandler : public mvAppItem
{
public:
    explicit mvVisibleHandler(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};