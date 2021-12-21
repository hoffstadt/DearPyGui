#pragma once

#include <vector>
#include "mvToolWindow.h"

namespace Marvel {

    class mvAppItem;

    class mvLayoutWindow final : public mvToolWindow
    {

    public:

        mvLayoutWindow();

        [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_ITEM_REGISTRY_UUID; }
        [[nodiscard]] const char* getTitle() const override { return "Item Registry"; }

    protected:

        void drawWidgets() override;

    public:

        void renderTreeNode(mvRef<mvAppItem>& item);
        void renderRootCategory(const char* category, std::vector<mvRef<mvAppItem>>& roots);

        mvRef<mvAppItem> _itemref = nullptr;
        mvUUID m_selectedItem = 0;
        bool m_dirtyNodes = true;
        int m_selectedId = -1;
        ImGuiTextFilter _imguiFilter;
        bool _startFiltering = false;
        bool _slots = false;
    };

}
