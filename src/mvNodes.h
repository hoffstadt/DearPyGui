#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

class mvNode : public mvAppItem
{

public:

    explicit mvNode(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    int getId() const { return _id; }

public:

    int _id = 0;
    bool _draggable = true;

};

class mvNodeAttribute : public mvAppItem
{

    enum class AttributeType {
        mvAttr_Input = 0L,
        mvAttr_Output,
        mvAttr_Static
    };

public:

    explicit mvNodeAttribute(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    int getId() const { return _id; }

private:

    int             _id = 1;
    AttributeType   _attrType = AttributeType::mvAttr_Input;
    ImNodesPinShape _shape = ImNodesPinShape_CircleFilled;
    std::string     _category = "general";

};

class mvNodeLink : public mvAppItem
{

public:

    explicit mvNodeLink(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void customAction(void* data = nullptr) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void getSpecificConfiguration(PyObject* dict) override;

    int getId() const { return _id0; }
    int getId1() const { return _id1; }
    int getId2() const { return _id2; }

public:

    int _id0 = 0;
    int _id1 = 0;
    int _id2 = 0;

    mvUUID _id1uuid = 0;
    mvUUID _id2uuid = 0;
};

class mvNodeEditor : public mvAppItem
{

public:

    explicit mvNodeEditor(mvUUID uuid);
    ~mvNodeEditor();

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void onChildRemoved(std::shared_ptr<mvAppItem> item);

    std::vector<mvUUID> getSelectedNodes() const;
    std::vector<mvUUID> getSelectedLinks() const;
    void clearNodes() { _clearNodes = true; }
    void clearLinks() { _clearLinks = true; }

private:

    ImGuiWindowFlags _windowflags = ImGuiWindowFlags_NoSavedSettings;
    std::vector<int> _selectedNodes;
    std::vector<int> _selectedLinks;

    bool _clearNodes = false;
    bool _clearLinks = false;

    mvPyObjectStrict      _delinkCallback = nullptr;
    ImNodesEditorContext* _context = nullptr;

    bool                   _minimap = false;
    ImNodesMiniMapLocation _minimapLocation= ImNodesMiniMapLocation_TopLeft;

};