/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

    MV_REGISTER_WIDGET(mvNodeEditor, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvNodeEditor : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeEditor, add_node_editor)
        MV_DEFAULT_PARENTS
        MV_NO_CONSTANTS

        MV_CREATE_COMMAND(get_selected_nodes);
        MV_CREATE_COMMAND(get_selected_links);
        MV_CREATE_COMMAND(clear_selected_nodes);
        MV_CREATE_COMMAND(clear_selected_links);

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_VISIBLE |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

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
