#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

    class mvNodeEditor : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeEditor, add_node_editor)
        MV_DEFAULT_PARENTS

        MV_CREATE_COMMAND(get_selected_nodes);
        MV_CREATE_COMMAND(get_selected_links);
        MV_CREATE_COMMAND(clear_selected_nodes);
        MV_CREATE_COMMAND(clear_selected_links);

        MV_START_COMMANDS
            MV_ADD_COMMAND(get_selected_nodes);
            MV_ADD_COMMAND(get_selected_links);
            MV_ADD_COMMAND(clear_selected_nodes);
            MV_ADD_COMMAND(clear_selected_links);
        MV_END_COMMANDS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvMenuBar),
            MV_ADD_CHILD(mvAppItemType::mvNode),
            MV_ADD_CHILD(mvAppItemType::mvNodeLink),
            MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
            MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler) ,
            MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
            MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
        MV_END_CHILDREN

    public:

        explicit mvNodeEditor(mvUUID uuid);
        ~mvNodeEditor();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void onChildRemoved(mvRef<mvAppItem> item) override;

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

        PyObject* _delinkCallback = nullptr;
        imnodes::EditorContext* _context = nullptr;
        
    };

}
