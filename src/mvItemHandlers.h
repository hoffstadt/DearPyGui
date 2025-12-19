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

enum mvEventType
{
    // These constants can be used as a combination of flags
    mvEventType_None       = 0,
    mvEventType_Off        = 1 << 1,
    mvEventType_Enter      = 1 << 2,
    mvEventType_On         = 1 << 3,
    mvEventType_Leave      = 1 << 4,
    // When the state changes, we observe two events in a single frame:
    // both "enter" and "on" or both "leave" and "off".  We need to define
    // these flags here so that they can be used as a mask later on.
    mvEventType_EnterAndOn = mvEventType_Enter | mvEventType_On,
    mvEventType_LeaveAndOff = mvEventType_Leave | mvEventType_Off,
    // This is the state the handler will react to by default
    mvEventType_Default    = mvEventType_On
};

// This is a base class for handlers that monitor a single bool variable
// that can switch on or off and reflects the item state, like "focused"
// or "hovered".
class mvBoolStateHandler : public mvItemHandler
{
public:
    explicit mvBoolStateHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void checkEvent(bool curState, bool prevState, mvAppItem* parent);
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

protected:
    mvEventType trackedEventType = mvEventType_Default;
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

class mvFocusHandler : public mvBoolStateHandler
{
public:
    explicit mvFocusHandler(mvUUID uuid) : mvBoolStateHandler(uuid) {}
    void customAction(void* data = nullptr) override;
};

class mvHoverHandler : public mvBoolStateHandler
{
public:
    explicit mvHoverHandler(mvUUID uuid) : mvBoolStateHandler(uuid) {}
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

enum mvScrollDirection
{
    // These constants can be used as a combination of flags
    mvScrollDirection_XAxis = 1,
    mvScrollDirection_Horizontal = 1,
    mvScrollDirection_YAxis = 2,
    mvScrollDirection_Vertical = 2,
};

class mvScrollHandler : public mvItemHandler
{
public:
    explicit mvScrollHandler(mvUUID uuid) : mvItemHandler(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;

private:
    static PyObject* makeAppData(mvUUID uuid, const std::string& alias, mvScrollDirection dir, bool isScrolling, float pos, float max);
};
