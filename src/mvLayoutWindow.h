#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
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
    void highlightItemRect(mvAppItem* item);
    bool jumpToItem(mvUUID item);
    void showError(const char* message);
    mvUUID getHoveredItem();
    mvUUID findHoveredInSubTree(mvAppItem* parent);
    mvUUID findHoveredInCategory(std::vector<std::shared_ptr<mvAppItem>>& roots);
    // Sets _itemref to whatever item it can find, like the first window or
    // some other root (e.g. if there are no windows).  Returns true if _itemref
    // is valid (non-null).  Can only return false if there are no any items at all.
    bool resetSelectedItem();
    void renderTypeSpecificInfo();
    void renderThemeComponentInfo(mvAppItem* item);
    void renderBindCount();
    void renderErrorMessage();

    std::shared_ptr<mvAppItem> _itemref = nullptr;
    mvUUID m_selectedItem = 0;
    bool _expandToSelected = false;
    std::unordered_set<mvUUID> _itemsToExpand;
    ImGuiTextFilter _imguiFilter;
    bool _startFiltering = false;
    bool _slots = false;
    bool _picker = false;
    std::string _search_tag;
    std::string _error_message;
};