#pragma once

#include "mvItemRegistry.h"
#include <vector>

class mvTableCell : public mvAppItem
{
public:

    explicit mvTableCell(mvUUID uuid);
    void draw(ImDrawList* drawlist, float x, float y) override;

};

class mvTableColumn : public mvAppItem
{

public:

    explicit mvTableColumn(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

public:

    ImGuiTableColumnFlags _flags = 0;
    float _init_width_or_weight = 0.0f;
    ImGuiID _id = 0u;

};

class mvTableRow : public mvAppItem
{

public:

    explicit mvTableRow(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}

private:

    int _id = 0;
};

class mvTable : public mvAppItem
{

public:

    explicit mvTable(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void onChildAdd(std::shared_ptr<mvAppItem> item);
    void onChildRemoved(std::shared_ptr<mvAppItem> item);
    void onChildrenRemoved();

    // values
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

public:

    ImGuiTextFilter _imguiFilter;

    int  _columns = 0;
    int  _rows = 0;
    int  _inner_width = 0;
    int  _freezeRows = 0;
    int  _freezeColumns = 0;
    ImGuiTableFlags _flags = 0;
    bool _tableHeader = true;
    bool _useClipper = false;

    float            _scrollX = 0.0f;
    float            _scrollY = 0.0f;
    float            _scrollMaxX = 0.0f;
    float            _scrollMaxY = 0.0f;
    bool             _scrollXSet = false;
    bool             _scrollYSet = false;

    std::vector<bool> _columnColorsSet;
    std::vector<bool> _rowColorsSet;
    std::vector<bool> _rowSelectionColorsSet;
    std::vector<std::vector<bool>> _cellColorsSet;

    std::vector<ImU32> _columnColors;
    std::vector<ImU32> _rowColors;
    std::vector<ImU32> _rowSelectionColors;
    std::vector<std::vector<ImU32>> _cellColors;

    struct SortSpec
    {
        mvUUID column;
        int direction;
    };

};