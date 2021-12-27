#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

    class mvNodeEditor : public mvAppItem
    {

    public:

        explicit mvNodeEditor(mvUUID uuid);
        ~mvNodeEditor();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void onChildRemoved(mvRef<mvAppItem> item);

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
