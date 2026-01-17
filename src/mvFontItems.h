#pragma once

#include <array>
#include <imgui.h>
#include "mvItemRegistry.h"

class mvFontRegistry : public mvAppItem
{

public:

    explicit mvFontRegistry(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void customAction(void* data = nullptr) override;
    void onChildAdd(std::shared_ptr<mvAppItem> item) { _dirty = true; }

private:

    bool _dirty = true;

};

class mvCharRemap : public mvAppItem
{

public:

    explicit mvCharRemap(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void handleSpecificRequiredArgs(PyObject* dict) override;
    int getSourceChar() const { return _source; }
    int getTargetChar() const { return _target; }

private:

    int _source = 0;
    int _target = 0;

};

class mvFont : public mvAppItem
{

public:

    explicit mvFont(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvFont();

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    ImFont* getFontPtr() { return _fontPtr; }
    float getSize() { return _size; }
    void pushFont() { ImGui::PushFont(_fontPtr, _size); }

public:

    // config
    std::string _file;
    float       _size = 13.0f;
    bool        _pixelSnapH = false;
    bool        _pixelSnapV = false;

    // finalized
    ImFont* _fontPtr = nullptr;

};
