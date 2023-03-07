#pragma once

#include <array>
#include "mvItemRegistry.h"

class mvFontRegistry : public mvAppItem
{

public:

    explicit mvFontRegistry(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void onChildAdd(std::shared_ptr<mvAppItem> item) { config.show = true; }

    bool isInvalid() const { return _dirty; }
    void resetFont();

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

    void draw(ImDrawList* drawlist, float x, float y) override;
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    ImFont* getFontPtr() { return _fontPtr; }

public:

    // config
    std::string _file;
    float       _size = 13.0f;
    bool        _default = false;

    // finalized
    ImFont* _fontPtr = nullptr;
    ImVector<ImWchar> _ranges;

};

class mvFontChars : public mvAppItem
{

public:

    explicit mvFontChars(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void handleSpecificRequiredArgs(PyObject* dict) override;
    const std::vector<ImWchar>& getCharacters() const { return _chars; }

private:

    std::vector<ImWchar>  _chars;

};

class mvFontRange : public mvAppItem
{

public:

    explicit mvFontRange(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void handleSpecificRequiredArgs(PyObject* dict) override;
    const std::array<ImWchar, 3>& getRange() const { return _range; }

private:

    int _min = 0x0370;
    int _max = 0x03ff;
    std::array<ImWchar, 3> _range = { 0x0370, 0x03ff, 0 };

};

class mvFontRangeHint : public mvAppItem
{

public:

    explicit mvFontRangeHint(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void handleSpecificRequiredArgs(PyObject* dict) override;
    int getHint() const { return _hint; }

private:

    int _hint = 0;

};