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

class mvItemHandler : public mvAppItem
{
public:
    explicit mvItemHandler(mvUUID uuid) : mvAppItem(uuid) {}

protected:
    void submitHandler(mvAppItem* parent);
};

class mvActivatedHandler : public mvItemHandler
{
public:
    explicit mvActivatedHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};

class mvActiveHandler : public mvItemHandler
{
public:
    explicit mvActiveHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

};

class mvClickedHandler : public mvItemHandler
{
public:
    int _button = -1;
    explicit mvClickedHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvDoubleClickedHandler : public mvItemHandler
{
public:
    int _button = -1;
    explicit mvDoubleClickedHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvDeactivatedAfterEditHandler : public mvItemHandler
{
public:
    explicit mvDeactivatedAfterEditHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvDeactivatedHandler : public mvItemHandler
{
public:
    explicit mvDeactivatedHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvEditedHandler : public mvItemHandler
{
public:
    explicit mvEditedHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvFocusHandler : public mvItemHandler
{
public:
    explicit mvFocusHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvHoverHandler : public mvItemHandler
{
public:
    explicit mvHoverHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvResizeHandler : public mvItemHandler
{
public:
    explicit mvResizeHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvToggledOpenHandler : public mvItemHandler
{
public:
    explicit mvToggledOpenHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};

class mvVisibleHandler : public mvItemHandler
{
public:
    explicit mvVisibleHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
};
