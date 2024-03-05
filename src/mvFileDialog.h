#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

#pragma warning(push, 0) 
#include <ImGuiFileDialog.h>
#pragma warning(pop)

class mvFileDialog final : public mvAppItem
{

public:

    explicit mvFileDialog(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    void drawPanel();

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

    ImGuiFileDialog& getDialog();
    void             markDirty() { _dirtySettings = true; }
    PyObject*        getInfoDict();
    bool             getContinueValue() const { return *_value; }

public:

    std::shared_ptr<bool>     _value = std::make_shared<bool>(false);
    bool             _disabled_value = false;
    ImGuiFileDialog  _instance;
    bool             _dirtySettings = true;
    std::string      _filters;
    std::string      _defaultPath;
    std::string      _defaultFilename = ".";
    int              _fileCount = 0;
    bool             _modal = false;
    bool             _directory = false;
    mvVec2           _min_size = { 100.0f, 100.0f };
    mvVec2           _max_size = { 30000.0f, 30000.0f };
	mvPyObjectStrict _cancelCallback = nullptr;
};