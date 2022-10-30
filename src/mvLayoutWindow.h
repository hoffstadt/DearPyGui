#pragma once

#include <vector>
#include <memory>
#include "mvToolWindow.h"

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

    void renderTreeNode(std::shared_ptr<mvAppItem>& item);
    void renderRootCategory(const char* category, std::vector<std::shared_ptr<mvAppItem>>& roots);

    std::shared_ptr<mvAppItem> _itemref = nullptr;
    mvUUID m_selectedItem = 0;
    bool m_dirtyNodes = true;
    int m_selectedId = -1;
    ImGuiTextFilter _imguiFilter;
    bool _startFiltering = false;
    bool _slots = false;
};