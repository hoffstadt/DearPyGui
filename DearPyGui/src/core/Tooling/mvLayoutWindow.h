#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvAppItem;

    class mvLayoutWindow : public mvToolWindow
    {

    public:

        mvLayoutWindow();

        const char* getName() const override { return "mvItemRegistry"; }
        const char* getTitle() const override { return "Item Registry"; }

    protected:

        void drawWidgets() override;

    private:

        // node view rendering
        void renderNode(mvAppItem* item);

        bool renderParentNode(mvAppItem* item, int nodeId, int slotId);
        bool renderChildAttr(mvAppItem* item, int slot, int slotId);
        void renderChildNodes(mvAppItem* item, int slot, int& link, int& node, int startAttrId, int parentAttrId);

        void renderTreeNode(mvAppItem* item);

        std::string m_selectedItem;
        bool m_showLabels = false;
        bool m_nodeView = false;
        bool m_dirtyNodes = true;
        int m_selectedId = -1;

    };

}
