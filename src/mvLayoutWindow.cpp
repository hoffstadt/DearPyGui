#include "mvLayoutWindow.h"
#include <imnodes.h>
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvFontItems.h"
#include "mvThemes.h"
#include <misc/cpp/imgui_stdlib.h>

static void
DebugItem(const char* label, const char* item) {
    ImGui::Text("%s", label);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
}

static void
DebugItem(const char* label, float value) {
    ImGui::Text("%s", label);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%g", value);
}

static void
DebugItem(const char* label, mvVec2 value) {
    ImGui::Text("%s", label);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "(%g, %g)", value.x, value.y);
}

static void
DebugItem(const char* label, bool value) {
    DebugItem(label, value? "True" : "False");
}

static void InfoHeader(const char* label) {
    ImGui::NewLine();
    ImGui::Text("%s", label);
    ImGui::Separator();
}

mvLayoutWindow::mvLayoutWindow()
{
    m_windowflags = ImGuiWindowFlags_NoSavedSettings;
}

void mvLayoutWindow::renderRootCategory(const char* category, std::vector<std::shared_ptr<mvAppItem>>& roots)
{

    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (roots.empty() ? ImGuiTreeNodeFlags_Leaf : 0);

    ImGui::PushID(&roots);

    if (_expandToSelected)
    {
        for (auto& root : roots)
        {
            if (root->uuid == m_selectedItem || _itemsToExpand.count(root->uuid) > 0)
            {
                ImGui::SetNextItemOpen(true);
                break;
            }
        }
    }

    const auto expanded = ImGui::TreeNodeEx(category, node_flags);

    if (!roots.empty())
    {
        ImGui::SameLine();
        ImGui::TextColored({0.7f, 0.7f, 0.7f, 1.0f}, "(%zd)", roots.size());
    }

    if (expanded)
    {
        for (auto& root : roots)
            renderTreeNode(root);
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void mvLayoutWindow::renderTreeNode(std::shared_ptr<mvAppItem>& item)
{

    // build up flags for current node
    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick
        | ((item->uuid == m_selectedItem) ? ImGuiTreeNodeFlags_Selected : 0)
        | (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER ? 0 : ImGuiTreeNodeFlags_Leaf);

    // render this node
    ImGui::PushID(item.get());
    std::string labelToShow;
    // Remember whether we have a custom name for this item, like a label or a tag.
    // Otherwise we'll have to show its type name instead, which is not as descriptive.
    bool is_labeled = true;
    if (!item->config.alias.empty())
        labelToShow = item->config.alias;
    else if (!item->config.specifiedLabel.empty())
        labelToShow = item->config.specifiedLabel;
    else
    {
        is_labeled = false;
        labelToShow = DearPyGui::GetEntityTypeString(item->type);
        constexpr const char* prefix = "mvAppItemType::";
        // This is a cumbersome way to get string length, but it's the only way
        // that provides a constexpr (note: strlen() is not a constexpr).
        // Well, we could also do a `sizeof(prefix) - 1`...
        constexpr size_t prefix_len = std::char_traits<char>::length(prefix);
        if (labelToShow.compare(0, prefix_len, prefix) == 0)
            labelToShow.erase(0, prefix_len);
    }


    if (!_imguiFilter.PassFilter(labelToShow.c_str()) && _startFiltering)
    {
        ImGui::PopID();
        return;
    }

    bool expandThisNode = (_expandToSelected && _itemsToExpand.count(item->uuid) > 0);
    if (expandThisNode)
    {
        ImGui::SetNextItemOpen(true);
    }

    // TODO: add a flag to GetEntityDesciptionFlags for this
    bool is_bindable = (item->type == mvAppItemType::mvTheme ||
                        item->type == mvAppItemType::mvItemHandlerRegistry ||
                        item->type == mvAppItemType::mvFont );

    // An unused/orhpaned bindable? (note that we might be holding 1 ref to item)
    bool is_lonely = (is_bindable && item.use_count() <= (1 + (item == _itemref? 1 : 0)));

    // An extra check for some elements that can have global refs *not* shared
    // via shared_ptr.
    // - the global theme is the only one that has config.show == true
    // - the global font is the only one having _default == true
    if (is_lonely && (
        (item->type == mvAppItemType::mvTheme && item->config.show) ||
        (item->type == mvAppItemType::mvFont && (static_cast<mvFont*>(item.get()))->_default)))
    {
        // It's bound as a global entity and therefore is not lonely
        is_lonely = false;
    }

    if (is_lonely)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 1.0f, 1.0f, 0.5f});
    }

    if (is_labeled)
    {
        // We're keeping alpha because some sub-trees might be translucent
        auto alpha = ImGui::GetStyleColorVec4(ImGuiCol_Text).w;
        ImGui::PushStyleColor(ImGuiCol_Text, {0.7f, 0.7f, 1.0f, alpha});
    }

    // Render the node itself
    const auto expanded = ImGui::TreeNodeEx(labelToShow.c_str(), node_flags);

    if (expandThisNode)
    {
        ImGui::SetScrollHereX();
        ImGui::SetScrollHereY();
    }

    if (is_labeled)
        ImGui::PopStyleColor();

    if (ImGui::IsItemHovered())
        highlightItemRect(item.get());

    if (item->uuid == m_selectedItem)
        _startFiltering = true;
        
    // processing for selecting node
    if (ImGui::IsItemClicked())
    {
        m_selectedItem = item->uuid;
        _itemref = item;
    }

    if (is_lonely)
    {
        ImGui::SameLine();
        ImGui::Text("(not bound)");
    }

    if (!(DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER))
    {
        if (is_lonely)
            ImGui::PopStyleColor();
        if(expanded)
            ImGui::TreePop();
        ImGui::PopID();
        if (item->uuid == m_selectedItem)
            _startFiltering = false;
        return;
    }

    if (expanded)
    {

        int i = 0;
        for (auto& childrenSet : item->childslots)
        {
            if (_slots)
            {
                std::string title = "Child Slot: " + std::to_string(i++);
                // Only expand a slot if there's something more to expand within it
                if (expandThisNode)
                {
                    for (auto& child : childrenSet)
                    {
                        if (child && _itemsToExpand.count(child->uuid) > 0)
                        {
                            ImGui::SetNextItemOpen(true);
                            break;
                        }
                    }
                }

                ImGui::PushStyleColor(ImGuiCol_Text, {0.7f, 0.7f, 0.7f, 1.0f});
                const auto slot_expanded = ImGui::TreeNodeEx(title.c_str(), childrenSet.empty() ? ImGuiTreeNodeFlags_Leaf : 0);
                ImGui::PopStyleColor();
                if (slot_expanded)
                {
                    for (auto& children : childrenSet)
                        if(children)
                            renderTreeNode(children);
                    ImGui::TreePop();
                }
            }
            else
            {
                for (auto& children : childrenSet)
                    if(children)
                        renderTreeNode(children);
            }
        }
        ImGui::TreePop();
    }

    if (is_lonely)
        ImGui::PopStyleColor();

    ImGui::PopID();

    if (item->uuid == m_selectedItem)
        _startFiltering = false;

}

void mvLayoutWindow::drawWidgets()
{
        
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    bool pickerEnabled = _picker;
    if (pickerEnabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonHovered));
    }
    if (ImGui::Button("Pick..."))
    {
        _picker = !_picker;
    }
    if (pickerEnabled)
    {
        ImGui::PopStyleColor();
    }
    if (_picker)
    {
        mvUUID hoveredItem = getHoveredItem();
        if (hoveredItem)
            highlightItemRect(GetItem(*GContext->itemRegistry, hoveredItem));

        // When picker is enabled, swallow the first click we get, and look at
        // what is hovered - but don't abuse our own window.
        if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            // Pretend that nothing happened :)
            g.IO.MouseDownDuration[0] = g.IO.MouseDownDurationPrev[0] = -1.0f;
            g.IO.MouseDown[0] = g.IO.MouseClicked[0] = false;
            // Show that hovered item
            jumpToItem(hoveredItem);
            _picker = false;
        }

    }

    ImGui::SameLine();

    if (ImGui::Button("Focused"))
    {
        if (!jumpToItem(GContext->focusedItem))
            showError("No focused item.");
    }

    ImGui::SameLine();

    const char* hint = "tag or uuid";
    ImVec2 hint_size = ImGui::CalcTextSize(hint);
    ImGui::SetNextItemWidth(hint_size.x + 2*style.FramePadding.x);
    // This input, while it doesn't need a label, *must* use a non-empty ID due to ImGui
    // restrictions.  That's why we put an explicit ID here.
    if (ImGui::InputTextWithHint("###uuid-search", hint, &_search_tag, ImGuiInputTextFlags_AutoSelectAll|ImGuiInputTextFlags_EnterReturnsTrue)
        && _search_tag.find_first_not_of(' ') != std::string::npos)
    {
        mvUUID uuid = GetIdFromAlias(*GContext->itemRegistry, _search_tag);
        if (!uuid)
        {
            try
            {
                size_t converted = 0;
                uuid = std::stoull(_search_tag, &converted);
                // Make sure we've used all non-whitespace chars, i.e. the input is a valid
                // number and not something like "123die"
                if (_search_tag.find_first_not_of(' ', converted) != std::string::npos)
                {
                    // Invalid input - reset it back to 0
                    uuid = 0;
                }
            }
            catch (...)
            {
                uuid = 0;
            }
        }
        if (!jumpToItem(uuid))
            showError("Item not found.");
    }

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    if (_itemref == nullptr)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 1.0f, 1.0f, 0.5f});
    }

    if (ImGui::ArrowButton("Move Up", ImGuiDir_Up))
    {
        mvSubmitCallback([&]()
            {
                mvPySafeLockGuard lk(GContext->mutex);
                MoveItemUp(*GContext->itemRegistry, m_selectedItem);
            });
    }

    ImGui::SameLine();
    if (ImGui::ArrowButton("Move Down", ImGuiDir_Down))
    {
        mvSubmitCallback([&]()
            {
                mvPySafeLockGuard lk(GContext->mutex);
                MoveItemDown(*GContext->itemRegistry, m_selectedItem);
            });
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete"))
    {
        mvSubmitCallback([&]()
            {
                mvPySafeLockGuard lk(GContext->mutex);
                DeleteItem(*GContext->itemRegistry, m_selectedItem, false);
                m_selectedItem = 0;
                resetSelectedItem();
            });
    }
    ImGui::SameLine();
    if (ImGui::Button("Show") && _itemref != nullptr)
    {
        _itemref->config.show = true;
        _itemref->info.shownLastFrame = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide") && _itemref != nullptr)
    {
        _itemref->config.show = false;
        _itemref->info.hiddenLastFrame = true;
    }

    if (_itemref == nullptr)
    {
        ImGui::PopStyleColor();
        ImGui::PopItemFlag();
    }

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::SameLine();
    ImGui::Checkbox("Show Slots###layout", &_slots);

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 0));
    if (ImGui::BeginTable("", 2, ImGuiTableFlags_Resizable|ImGuiTableFlags_NoSavedSettings|ImGuiTableFlags_BordersInnerV|ImGuiTableFlags_SizingFixedFit|ImGuiTableFlags_NoKeepColumnsVisible))
    {
        ImGui::TableSetupColumn("");
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);

        ImGui::TableNextRow();
            // left side
            ImGui::TableNextColumn();

                // We want the left side to initially be no narrower than this line
                ImVec2 spacer_size = ImGui::CalcTextSize("Type:mvAppItemType::mvWindowAppItem");
                // The left size will initially have a scrollbar, so let's add some space;
                // let's also account for item spacing between "type:" and its value (item spacing x),
                // and add more padding on the right (let's use frame padding for now).
                spacer_size.x += style.ScrollbarSize + style.ItemSpacing.x + style.FramePadding.x;
                // No need for vertical offset
                spacer_size.y = 0;
                // Adding some fake contents that the table will use for auto-fitting
                // 1st column (note that the child window below is not used for fitting:
                // it itself inherits width from the column).
                ImGui::Dummy(spacer_size);

                // We only render item details if there's a selected item OR if
                // we've been able to select another item (i.e. there's at least
                // something available in the item tree).
                if (_itemref != nullptr || resetSelectedItem())
                {
                    ImGui::BeginChild("###layoutwindow", ImVec2(0, 0));
                        static char ts[6] = "True";
                        static char fs[6] = "False";

                        std::string width = std::to_string(_itemref->config.width);
                        std::string height = std::to_string(_itemref->config.height);

                        ImGui::PushID(_itemref.get());
                            DebugItem("Label:", _itemref->config.specifiedLabel.c_str());
                            DebugItem("ID:", std::to_string(_itemref->uuid).c_str());
                            DebugItem("Alias:", _itemref->config.alias.c_str());
                            DebugItem("Type:", DearPyGui::GetEntityTypeString(_itemref->type));

                            ImGui::Spacing();
                            ImGui::Spacing();

                            DebugItem("Show:", _itemref->config.show);
                            DebugItem("Enabled:", _itemref->config.enabled);

                            DebugItem("Pos:", _itemref->state.pos);
                            DebugItem("Width:", width.c_str());
                            DebugItem("Height:", height.c_str());

                            DebugItem("Rect:", _itemref->state.rectMin);
                            ImGui::SameLine();
                            DebugItem("-", _itemref->state.rectMax);
                            DebugItem("Size:", _itemref->state.rectSize);

                            ImGui::Spacing();
                            ImGui::Spacing();

                            DebugItem("Container:", DearPyGui::GetEntityDesciptionFlags(_itemref->type) & MV_ITEM_DESC_CONTAINER ? ts : fs);
                            DebugItem("Location:", std::to_string(_itemref->info.location).c_str());
                            DebugItem("Filter:", _itemref->config.filter.c_str());

                            DebugItem("Tracked:", _itemref->config.tracked);
                            DebugItem("Track Offset:", _itemref->config.trackOffset);

                            ImGui::Spacing();
                            ImGui::Spacing();

                            DebugItem("Callback:", _itemref->config.callback ? ts : fs);
                            DebugItem("User Data:", *(_itemref->config.user_data) ? ts : fs);
                            DebugItem("Drop Callback:", _itemref->config.dropCallback ? ts : fs);
                            DebugItem("Drag Callback:", _itemref->config.dragCallback ? ts : fs);
                            DebugItem("Payload Type:", _itemref->config.payloadType.c_str());

                            renderTypeSpecificInfo();

                            InfoHeader("Bindings");
                            // TODO: make it a hyperlink to the theme
                            DebugItem("Theme Bound:", _itemref->theme ? ts : fs);
                            // TODO: make it a hyperlink to the font
                            DebugItem("Font Bound:", _itemref->font ? ts : fs);
                            // TODO: make it a hyperlink to the handlers
                            DebugItem("Handlers Bound:", _itemref->handlerRegistry ? ts : fs);

                            int applicableState = DearPyGui::GetApplicableState(_itemref->type);
                            InfoHeader("State");
                            if (applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", IsItemVisible(_itemref->state, 1));
                            if (applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", IsItemHovered(_itemref->state, 1));
                            if (applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", IsItemActive(_itemref->state, 1));
                            if (applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", IsItemFocused(_itemref->state, 1));
                            if (applicableState & MV_STATE_CLICKED)
                            {
                                DebugItem("Item Left Clicked:", IsItemLeftClicked(_itemref->state, 1));
                                DebugItem("Item Right Clicked:", IsItemRightClicked(_itemref->state, 1));
                                DebugItem("Item Middle Clicked:", IsItemMiddleClicked(_itemref->state, 1));
                            }
                            if (applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", IsItemEdited(_itemref->state, 1));
                            if (applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", IsItemActivated(_itemref->state, 1));
                            if (applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", IsItemDeactivated(_itemref->state, 1));
                            if (applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", IsItemDeactivatedAfterEdit(_itemref->state, 1));
                            if (applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", IsItemToogledOpen(_itemref->state, 1));

                        ImGui::PopID();
                    ImGui::EndChild();
                }

            // right side
            ImGui::TableNextColumn();
                _imguiFilter.Draw();
                _startFiltering = false;

                if (_expandToSelected)
                {
                    mvAppItem* parent = _itemref->info.parentPtr;
                    while (parent)
                    {
                        _itemsToExpand.insert(parent->uuid);
                        parent = parent->info.parentPtr;
                    }
                }

                ImGui::BeginChild("TreeChild", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                    renderRootCategory("Windows", GContext->itemRegistry->windowRoots);
                    renderRootCategory("Themes", GContext->itemRegistry->themeRegistryRoots);
                    renderRootCategory("Template Registries", GContext->itemRegistry->itemTemplatesRoots);
                    renderRootCategory("Staging Containers", GContext->itemRegistry->stagingRoots);
                    renderRootCategory("Texture Registries", GContext->itemRegistry->textureRegistryRoots);
                    renderRootCategory("Font Registries", GContext->itemRegistry->fontRegistryRoots);
                    renderRootCategory("Item Handler Registries", GContext->itemRegistry->itemHandlerRegistryRoots);
                    renderRootCategory("Handler Registries", GContext->itemRegistry->handlerRegistryRoots);
                    renderRootCategory("Value Registries", GContext->itemRegistry->valueRegistryRoots);
                    renderRootCategory("Colormap Registries", GContext->itemRegistry->colormapRoots);
                    renderRootCategory("File Dialogs", GContext->itemRegistry->filedialogRoots);
                    renderRootCategory("Viewport Menubars", GContext->itemRegistry->viewportMenubarRoots);
                    renderRootCategory("Viewport Drawlists", GContext->itemRegistry->viewportDrawlistRoots);
                ImGui::EndChild();

                // No longer need to expand the tree
                _expandToSelected = false;
                _itemsToExpand.clear();

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
    renderErrorMessage();
}

void mvLayoutWindow::renderThemeComponentInfo(mvAppItem* item)
{
    if (!item || item->type != mvAppItemType::mvThemeComponent)
        return;
    auto comp = static_cast<mvThemeComponent*>(item);
    DebugItem("Enabled State:", comp->_specificEnabled);
    mvAppItemType target_type = (mvAppItemType)comp->_specificType;
    DebugItem("Applies to:", (target_type == mvAppItemType::All)? "All item types" : DearPyGui::GetEntityTypeString(target_type));
}

void mvLayoutWindow::renderBindCount()
{
    auto count_str = std::to_string(_itemref.use_count() - 2);
    DebugItem("Bound to:", count_str.c_str());
    ImGui::SameLine();
    ImGui::Text("items");
}

void mvLayoutWindow::renderTypeSpecificInfo()
{
    switch (_itemref->type)
    {
    case mvAppItemType::mvTheme:
        {
            InfoHeader("Theme");
            // Most themes will have False here, so let's not clutter the UI for them
            if (_itemref->config.show)
            {
                DebugItem("Global Theme:", true);
            }
            renderBindCount();
        }
        break;

    case mvAppItemType::mvThemeComponent:
        {
            InfoHeader("Theme");
            renderThemeComponentInfo(_itemref.get());
        }
        break;

    case mvAppItemType::mvThemeColor:
        {
            InfoHeader("Theme");
            auto item = static_cast<mvThemeColor*>(_itemref.get());
            renderThemeComponentInfo(item->info.parentPtr);

            // TODO: add accessors for lib type and color index
            // switch (item->_libType)

            // OMG.  This is horrible.  We might be better off having a typed
            // accessor for this, too.
            auto color = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Color:");
            ImGui::SameLine();

            // Making it fixed-width so that prefixes do not show up - they look
            // messy in that little space the layout window typically has.
            float text_width = ImGui::CalcTextSize("255").x;
            const ImGuiStyle& style = ImGui::GetStyle();
            float total_width = (text_width <= 0.0f)? -1.0f : 4*(text_width + 2*style.FramePadding.x + style.ItemInnerSpacing.x) + ImGui::GetFrameHeight();
            ImGui::SetNextItemWidth(total_width);

            ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 0.0f, 1.0f, 1.0f});
            ImGui::ColorEdit4("##color", &(*color)[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoLabel);
            ImGui::PopStyleColor();
        }
        break;

    case mvAppItemType::mvThemeStyle:
        {
            InfoHeader("Theme");
            auto item = static_cast<mvThemeStyle*>(_itemref.get());
            renderThemeComponentInfo(item->info.parentPtr);

            // TODO: add accessors for lib type and style index
            // switch (item->_libType)

            // OMG.  This is horrible.  We might be better off having a typed
            // accessor for this, too.
            auto value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Value:");
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 0.0f, 1.0f, 1.0f});
            float value_buf[2] = {(*value)[0], (*value)[1]};
            if (ImGui::DragFloat2("##style", value_buf, 1.0f, 0.0f, 100.0f, "%g"))
            {
                (*value)[0] = value_buf[0];
                (*value)[1] = value_buf[1];
            }
            ImGui::PopStyleColor();
        }
        break;

    case mvAppItemType::mvItemHandlerRegistry:
        {
            InfoHeader("Handler Registry");
            renderBindCount();
        }
        break;

    case mvAppItemType::mvFont:
        {
            InfoHeader("Font");
            renderBindCount();
        }
        break;
    }
}

void mvLayoutWindow::highlightItemRect(mvAppItem* item)
{
    auto rectMin = item->state.rectMin;
    auto rectMax = item->state.rectMax;
    if (rectMin.x == 0.0f && rectMin.y == 0.0f && rectMax.x == 0.0f && rectMax.y == 0.0f)
    {
        // This only works for items positioned relative to the viewport, such as
        // windows and popups.  Some other items, like table columns or child windows,
        // will get into this branch too.

        auto pos = item->state.pos;

        // Going through the container chain, adjusting `pos` according to window positions
        // that we find across the way to the root window.
        mvAppItem* parent = item->info.parentPtr;
        while (parent) {
            // TODO: if mvTable ever starts saving its position, add it here as well.
            // Item positions are relative to the table when scrolling in the table is
            // enabled, because in that case it creates an internal child window.
            if (parent->type == mvAppItemType::mvWindowAppItem || parent->type == mvAppItemType::mvChildWindow)
                pos = pos + parent->state.pos - parent->state.scrollPos;

            parent = parent->info.parentPtr;
        }

        ImGui::GetForegroundDrawList()->AddRect(pos, pos + item->state.rectSize, IM_COL32(255, 0, 0, 255));
    }
    else
    {
        // Regular widgets that have rectMin/rectMax go here
        ImGui::GetForegroundDrawList()->AddRect(rectMin, rectMax, IM_COL32(255, 255, 0, 255));
    }
}

bool mvLayoutWindow::jumpToItem(mvUUID item)
{
    auto itemRef = GetRefItem(*GContext->itemRegistry, item);
    if (!itemRef)
        return false;
    m_selectedItem = item;
    _itemref = itemRef;
    _expandToSelected = (item != 0);
    return true;
}

mvUUID mvLayoutWindow::getHoveredItem()
{
    // Surely this should belong to mvItemRegistry, but let it live here for the moment.
    // The layout window will probably be the only user of this function.

    // For simplicity, we'll only check those registries that can actually capture
    // hovered state.  Note that they are listed here in the reverse order of their
    // rendering - this way we'll get the last item that got hovered; see notes in
    // findHoveredInSubTree for more info.
    std::vector<std::shared_ptr<mvAppItem>>* categories[] = {
        &GContext->itemRegistry->viewportDrawlistRoots,
        &GContext->itemRegistry->viewportMenubarRoots,
        &GContext->itemRegistry->windowRoots,
        &GContext->itemRegistry->filedialogRoots,
    };
    for (auto category : categories)
    {
        mvUUID hoveredItem = findHoveredInCategory(*category);
        if (hoveredItem)
            return hoveredItem;
    }
    return 0;
}

mvUUID mvLayoutWindow::findHoveredInCategory(std::vector<std::shared_ptr<mvAppItem>>& roots)
{
    for (auto& root : roots)
    {
        mvUUID hoveredItem = findHoveredInSubTree(root.get());
        if (hoveredItem)
            return hoveredItem;
    }
    return 0;
}

mvUUID mvLayoutWindow::findHoveredInSubTree(mvAppItem* parent)
{
    // Sometimes there might be null pointers in the widgets tree.
    if (!parent)
        return 0;

    // Since mvLayoutWindow is rendered at the beginning of a frame, we only
    // retain items state from the previous frame, therefore it's already
    // 1 frame old - telling IsItemHovered to look back that far.
    bool hovered = IsItemHovered(parent->state, 1);

    // If item is hoverable but not hovered, it's definitely not in the chain of hovered
    // items.  Let's drop the rest of sub-tree.
    // 
    // Note: the way DPG tests for hovered state of windows - not specifying ImGuiHoveredFlags_ChildWindows -
    // as soon as a child window is hovered, its parent is reported as "not hovered".
    // That's why we can't skip non-hovered windows at all, otherwise we'll miss the
    // beginning of the hovered chain.
    if (!hovered && (DearPyGui::GetApplicableState(parent->type) & MV_STATE_HOVER) &&
            parent->type != mvAppItemType::mvWindowAppItem && parent->type != mvAppItemType::mvChildWindow)
        return 0;

    // Now, we have three possible cases:
    // 
    //  - The item is not hoverable.  It may still contain some hovered items in
    //    its subtree, so we check the children; worst case we'll return 0.
    // 
    //  - The item is hoverable (and therefore hovered - see the check above),
    //    and there's a hovered child inside.  We'll search deeper because the
    //    current item is somewhere in the middle of the chain.
    // 
    //  - The item is hovered and it has no hovered children.  This means it's
    //    finally the leaf, that end of the hovered chain that we're looking for.

    // Since slots and children are typicaly rendered in the first-to-last order,
    // we're mainly interested in the *last* child that got the hovered status.
    // This means we have to do reverse iteration, and have to resort to ugly
    // iterators instead of a typical range-for loop.
    for (auto itChildSet = std::crbegin(parent->childslots); itChildSet != std::crend(parent->childslots); ++itChildSet)
    {
        for (auto itChild = std::crbegin(*itChildSet); itChild != std::crend(*itChildSet); ++itChild)
        {
            mvUUID hoveredItem = findHoveredInSubTree(itChild->get());
            if (hoveredItem)
                return hoveredItem;
        }
    }
    // Finally this is the deepest item - or else something totally not hoverable.
    return hovered? parent->uuid : 0;
}

bool mvLayoutWindow::resetSelectedItem()
{
    std::array registries {
        &GContext->itemRegistry->windowRoots,
        &GContext->itemRegistry->themeRegistryRoots,
        &GContext->itemRegistry->itemTemplatesRoots,
        &GContext->itemRegistry->stagingRoots,
        &GContext->itemRegistry->textureRegistryRoots,
        &GContext->itemRegistry->fontRegistryRoots,
        &GContext->itemRegistry->itemHandlerRegistryRoots,
        &GContext->itemRegistry->handlerRegistryRoots,
        &GContext->itemRegistry->valueRegistryRoots,
        &GContext->itemRegistry->colormapRoots,
        &GContext->itemRegistry->filedialogRoots,
        &GContext->itemRegistry->viewportMenubarRoots,
        &GContext->itemRegistry->viewportDrawlistRoots,
    };

    for (auto registry : registries)
    {
        if (!registry->empty())
        {
            _itemref = (*registry)[0];
            m_selectedItem = _itemref->uuid;
            _expandToSelected = true;
            return true;
        }
    }

    // Just in case it's still holding a ref to an item that no longer exists
    // (and thus keeping the item object from deletion).
    _itemref = nullptr;
    m_selectedItem = 0;
    return false;
}

void mvLayoutWindow::showError(const char* message)
{
    _error_message = message;
    ImGui::OpenPopup("error");
}

void mvLayoutWindow::renderErrorMessage()
{
    if (ImGui::BeginPopup("error"))
    {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s", _error_message.c_str());
        ImGui::EndPopup();
    }
}
