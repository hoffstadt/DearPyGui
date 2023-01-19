#pragma once

#include <array>
#include <imnodes.h>
#include "mvItemRegistry.h"

void apply_local_theming(mvAppItem* item);
void cleanup_local_theming(mvAppItem* item);

class mvTheme : public mvAppItem
{

public:

    explicit mvTheme(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override { assert(false); }
    void setSpecificType(int specificType) { _specificType = specificType; }
    void setSpecificEnabled(int enabled) { _specificEnabled = enabled; }

    void push_theme_components();
    void pop_theme_components();

public:

    int _specificType = (int)mvAppItemType::All;
    bool _specificEnabled = true;

};

class mvThemeColor : public mvAppItem
{

public:

    explicit mvThemeColor(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override { assert(false); }
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    void push_theme_color();
    void pop_theme_color();

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

    void setLibType(mvLibType libType) { _libType = libType; }

private:

    std::shared_ptr<std::array<float, 4>> _value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    ImGuiCol _targetColor = 0;
    mvLibType _libType = mvLibType::MV_IMGUI;

};

class mvThemeComponent : public mvAppItem
{

public:

    explicit mvThemeComponent(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    void push_theme_items();
    void pop_theme_items();

public:

    int                      _specificType = (int)mvAppItemType::All;
    bool                     _specificEnabled = true;
    std::shared_ptr<mvThemeComponent>* _specificComponentPtr = nullptr;
    std::shared_ptr<mvThemeComponent>* _specificDisabledComponentPtr = nullptr;
    std::shared_ptr<mvThemeComponent>  _oldComponent = nullptr;

};

class mvThemeStyle : public mvAppItem
{

public:

    explicit mvThemeStyle(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override { assert(false); }
    void handleSpecificPositionalArgs(PyObject* dict) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    void push_theme_style();
    void pop_theme_style();

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

    void setLibType(mvLibType libType) { _libType = libType; }

private:

    std::shared_ptr<std::array<float, 4>> _value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, -1.0f, 0.0f, 0.0f});
    ImGuiCol _targetStyle = 0;
    mvLibType _libType = mvLibType::MV_IMGUI;


};