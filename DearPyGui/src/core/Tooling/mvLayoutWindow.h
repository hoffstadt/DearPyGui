#pragma once

#include "mvToolWindow.h"

namespace Marvel {

    class mvAppItem;

    class mvLayoutWindow : public mvToolWindow
    {

    public:

        mvLayoutWindow();

        mvUUID getUUID() const override { return MV_TOOL_ITEM_REGISTRY_UUID; }
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

        mvUUID m_selectedItem = 0;
        bool m_showLabels = false;
        bool m_nodeView = false;
        bool m_dirtyNodes = true;
        int m_selectedId = -1;

    };

}
