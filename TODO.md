# Porting DPG

### TODO Generic
- [ ] Generic:
    - [x] Fix `void ImGui::Shutdown(): Assertion '(g.IO.BackendPlatformUserData == __null) && "Forgot to shutdown Platform backend?"' failed.` error on shutdown.
    - [x] Enable control mods for plots!
    - [x] Check if what you did with DragPoint value is okay or not
    - [x] Find out how py files are created
    - [x] Remove GetKeysConstants because it's deeply linked to new IO (also, I put it, so it can be removed without any worries)
    - [x] Make it work wihout obsolete functions (ImGui and ImPlot)
    - [ ] Update demo with new functions
    - [x] Improve demo with multiple axes
    - [x] Antialiasing activated?
    - [ ] Disable keyboard navigation
    - [ ] Implement TagX/Y also on secondary axes
- [ ] Bugfix:
    - [x] Crash on tests teardown
    - [x] Crash when opening table in demo: `void ImGui::TableSetColumnEnabled(int, bool): Assertion 'table->Flags & ImGuiTableFlags_Hideable' failed.`
    - [x] Crash when opening some plots in demo:
        - [x] Line series: `void ImPlot::SetupLegend(ImPlotLocation, ImPlotLegendFlags): Assertion '((gp.CurrentPlot != nullptr && !gp.CurrentPlot->SetupLocked) || (gp.CurrentSubplot != nullptr && gp.CurrentPlot == nullptr)) && "Setup needs to be called after BeginPlot or BeginSubplots and before any setup locking functions (e.g. PlotX)!"' failed.` (Already investigated a little bit, probably some deep memory leak error)
        - [x] Query: `void ImPlot::SetupAxis(ImAxis, const char*, ImPlotAxisFlags): Assertion '(gp.CurrentPlot != nullptr && !gp.CurrentPlot->SetupLocked) && "Setup needs to be called after BeginPlot and before any setup locking functions (e.g. PlotX)!"' failed.`
    - [x] While scrolling on plots before start tracing: `bool ImPlot::BeginPlot(const char*, const ImVec2&, ImPlotFlags): Assertion '(gp.CurrentPlot == nullptr) && "Mismatched BeginPlot()/EndPlot()!"' failed.`
    - [x] When adding mvTool_Metrics it gives `T& ImVector<T>::operator[](int) [with T = ImVec2]: Assertion 'i >= 0 && i < Size' failed.`
    - [x] mvTool_Metrics doesn't show the "Presentation" plot anymore.
    - [ ] Crash when pressing button up/down in the `mvTool_ItemRegistry`, probably a problem with function MoveItemUp/Down in `mvItemRegistry.cpp`
- Not sure:
    - [ ] Expose `GetMouseClickedCount()` to Python?


### TODO ImGui
- [x] [V 1.83](https://github.com/ocornut/imgui/releases/tag/v1.83)
    - [x] Tables: Expose `TableSetColumnEnabled()` in public api. (in the issue is explained that it's just Hide/Show)
- [ ] [V 1.84](https://github.com/ocornut/imgui/releases/tag/v1.84)
    - [ ] IO: Added `io.AddFocusEvent()` api for backend to tell when host window has gained/lost focus.
    - [ ] Disabled: Add `BeginDisabled()/EndDisabled()`.
    - [x] Tables: Added `ImGuiTableColumnFlags_Disabled`
    - [x] Tables: Added `ImGuiTableColumnFlags_NoHeaderLabel` to request TableHeadersRow()
    - [ ] Check: Internals: (for custom widgets): because disabled items now sets HoveredID, if you want custom widgets to not react as hovered when disabled, in the majority of use cases it is preferable to check the "hovered" return value of ButtonBehavior() rather than HoveredId == id.
- [ ] [V 1.85](https://github.com/ocornut/imgui/releases/tag/v1.85)
    - [x] Debug: New StackTool.
    - [ ] IsWindowFocused: Added ImGuiFocusedFlags_NoPopupHierarchy flag allowing to exclude child popups from the tested windows when combined with ImGuiFocusedFlags_ChildWindows.
    - [ ] IsWindowHovered: Added ImGuiHoveredFlags_NoPopupHierarchy flag allowing to exclude child popups from the tested windows when combined with ImGuiHoveredFlags__ChildWindows.
    - [ ] IO: Added io.WantCaptureMouseUnlessPopupClose alternative to io.WantCaptureMouse
- [ ] [V 1.86](https://github.com/ocornut/imgui/releases/tag/v1.86)
    - [x] Added GetMouseClickedCount() function, returning the number of successive clicks (so IsMouseDoubleClicked(ImGuiMouseButton_Left) is same as GetMouseClickedCount(ImGuiMouseButton_Left) == 2, but it allows testing for triple clicks and more).
- [ ] [V 1.87](https://github.com/ocornut/imgui/releases/tag/v1.87)
Probably all the IO "issues" are deeply linked between them
    - [ ] Reworked IO mouse input API: Added io.AddMousePosEvent(), io.AddMouseButtonEvent(), io.AddMouseWheelEvent() functions, obsoleting writing directly to io.MousePos, io.MouseDown[], io.MouseWheel, etc.
    - [ ] Reworked IO keyboard input API: Added io.AddKeyEvent() function, obsoleting writing directly to io.KeyMap[], io.KeysDown[] arrays.
    - [ ] Reworked IO nav/gamepad input API and unifying inputs sources: Added full range of ImGuiKey_GamepadXXXX enums (e.g. ImGuiKey_GamepadDpadUp, ImGuiKey_GamepadR2) to use with io.AddKeyEvent(), io.AddKeyAnalogEvent().
        - [ ] Added io.AddKeyAnalogEvent() function, obsoleting writing directly to io.NavInputs[] arrays.
    - [ ] IO: Added event based input queue API, which now trickles events to support low framerates. Previously the most common issue case (button presses in low framerates) was handled by backend. This is now handled by core automatically for all kind of inputs.
- [ ] [V 1.88](https://github.com/ocornut/imgui/releases/tag/v1.88)
    - [ ] Internals: calling ButtonBehavior() without calling ItemAdd() now requires a KeepAliveID() call. This is because the KeepAliveID() call was moved from GetID() to ItemAdd() (probably nothing to do)
    - [ ] IO: Added io.SetAppAcceptingEvents() to set a master flag for accepting key/mouse/characters events (default to true). Useful if you have native dialog boxes that are interrupting your application loop/refresh, and you want to disable events being queued while your app is frozen.
- [ ] [V 1.89](https://github.com/ocornut/imgui/releases/tag/v1.89)
    - [ ] IsItemHovered: added ImGuiHoveredFlags_DelayNormal and ImGuiHoveredFlags_DelayShort for delayed hover test (work on items that have no persistent identifier e.g. Text items).
    - [ ] InputText: added io.ConfigInputTextEnterKeepActive
    - [ ] Internals: added wip internal APIs to allow handling input/shorting routing and key ownership. Things will be moved into public APIs over time, including a Shortcut() function that magically handle input routing.
    - [ ] Layout: Obsoleted using SetCursorPos() / SetCursorScreenPos() to extend parent window/cell boundaries. (Check what it actually changes)
    - [x] Changed signature of ImageButton() function.
- [x] [V 1.89.1](https://github.com/ocornut/imgui/releases/tag/v1.89.1)
- [ ] [V 1.89.2](https://github.com/ocornut/imgui/releases/tag/v1.89.2)
    - [ ] Misc: added GetItemID() in public API. (Check if this can be useful)
- [x] [V 1.89.3](https://github.com/ocornut/imgui/releases/tag/v1.89.3)
    - [x] SeparatorText(): Added SeparatorText() widget.
- [ ] [V 1.89.4](https://github.com/ocornut/imgui/releases/tag/v1.89.4)
    - [x] Tooltips: Added 'bool' return value to BeginTooltip() for API consistency.
    - [ ] Moved the optional "courtesy maths operators" implementation from imgui_internal.h in imgui.h. (important, must be checked)
- [ ] [V 1.89.5](https://github.com/ocornut/imgui/releases/tag/v1.89.5)
    - [ ] IO: Added io.AddMouseSourceEvent() and ImGuiMouseSource enum.
- [ ] [V 1.89.6](https://github.com/ocornut/imgui/releases/tag/v1.89.6)
    - [ ] Check little breaking changes in link
- [ ] [V 1.89.7](https://github.com/ocornut/imgui/releases/tag/v1.89.7)
    - [ ] Moved io.HoverDelayShort/io.HoverDelayNormal to style.HoverDelayShort/style.HoverDelayNormal (maybe need to change TooltipConfig activation_delay and other attributes)
    - [ ] Overlapping items
        - [ ] Obsoleted SetItemAllowOverlap(): it didn't and couldn't work reliably since 1.89 (2022-11-15), and relied on ambiguously defined design. Use SetNextItemAllowOverlap() before item instead.
        - [ ] Added SetNextItemAllowOverlap() (called before an item) as a replacement for using SetItemAllowOverlap() (called after an item). This is roughly equivalent to using the legacy SetItemAllowOverlap() call (public API) + ImGuiButtonFlags_AllowOverlap (internal).
    - [ ] Tooltips: Added SetItemTooltip() and BeginItemTooltip() helper functions. They are shortcuts for the common idiom of using IsItemHovered().
    - [ ] IsItemHovered: Added ImGuiHoveredFlags_Stationary to require mouse being stationary when hovering a new item. Added style.HoverStationaryDelay (~0.15 sec). Once the mouse has been stationary once the state is preserved for same item.
    - [ ] IsItemHovered: Added ImGuiHoveredFlags_ForTooltip as a shortcut for pulling flags from defaults.HoverFlagsForTooltipMouse or style.HoverFlagsForTooltipNav depending on active inputs.
    - [ ] IsWindowHovered: Added support for ImGuiHoveredFlags_Stationary
- [ ] [V 1.89.8](https://github.com/ocornut/imgui/releases/tag/v1.89.8)
    - [ ] IO: Obsoleted io.ClearInputCharacters() (added in 1.47) as it now ambiguous and often incorrect/misleading considering the existence of a higher-level input queue. This is automatically cleared by io.ClearInputsKeys().
    - [ ] IO: Added io.ClearEventsQueue() to clear incoming inputs events. 
    - [ ] Many modifications to Clipper
    - [x] IO: Exposed io.PlatformLocaleDecimalPoint to configure decimal point ('.' or ',') for languages needing it.
- [ ] [V 1.90](https://github.com/ocornut/imgui/releases/tag/v1.90)
    - [x] Removed IM_OFFSETOF() macro in favor of using offsetof() available in C++11.
- [ ][V 1.90.1](https://github.com/ocornut/imgui/releases/tag/v1.90.1)
    - [ ] imgui_freetype: commented out ImGuiFreeType::BuildFontAtlas() obsoleted in 1.81. Prefer using #define IMGUI_ENABLE_FREETYPE or see commented code for manual runtime calls.
    - [ ] Tabs: Added ImGuiTabItemFlags_NoAssumedClosure to enable app to react on closure attempt, without having to draw an unsaved document marker (ImGuiTabItemFlags_UnsavedDocument sets _NoAssumedClosure automatically).
    - [ ] Debug Tools: Added io.ConfigDebugIsDebuggerPresent option. 
    - [ ] Added DebugFlashStyleColor() to identify a style color. Added to Style Editor.

### TODO ImPlot
Take a look at implot.cpp to have a better changelog.

- [ ] [V 0.13](https://github.com/epezent/implot/releases/tag/v0.13)
    - [x] Query: Implement new DragRect
        - [ ] Find a way to give to those Query Rects the DragToolFlags (if possible)
    - [x] Axis: custom tick label formatting via ImPlotFormatter callbacks passed to SetupAxisFormat
    - [x] String: Implement string formatter
    - [x] TagX and TagY have been added and allow you to add custom labels to axes. These use either the axis' formatter, or a custom provided string. You can combine these with other tools like DragLine to create custom interactivity.
    - [x] DragPoint and DragLine no longer take a string ID. Instead they now expect an int ID, and as a result no longer display the optional label. Instead, use TagX/Y and Annotation to add labels too these tools if needed.
    - [x] additional options for drag tools via ImPlotDragToolFlags
    - [x] Bar plots can now be plotted in groups and stacked.
- [ ] [V 0.14](https://github.com/epezent/implot/releases/tag/v0.14)
    - [ ] Each of ImPlot's PlotX functions now takes an optional ImPlotXFlags parameter.
        - [ ] Implement generic `ImPlotItemFlags` for every type of Plot (also the ones for `custom_series` don't work)
        - [x] Implement specific flags for every kind of plot
        - [ ] Add ImPlotFlags_CanvasOnly flag.. it's more problematic than expected
    - [x] Implement axis ImPlotAxisFlags_Log with SetupAxisScale
    - [x] Implement axis ImPlotAxisFlags_Time with SetupAxisScale (and modify demo)
    - [x] You can now constrain axes limits so that users can't pan beyond a min or max value, or zoom beyond a min or max width/height
    - [ ] You can now customize the supported types by defining IMPLOT_CUSTOM_NUMERIC_TYPES at compile time to define your own type list (probably not interesting for us)
    - [x] Line plots now honor ImGui's AntiAliasedLines and AntiAliasedLinesUseTex. That's right, ImPlot now uses texture based AA!
    - [x] Legend entries can be sorted using ImPlotLegendFlags_Sort
- [x] [V 0.15](https://github.com/epezent/implot/releases/tag/v0.15)
- [x] [V 0.16](https://github.com/epezent/implot/releases/tag/v0.16) 


### Completed Column ✓
- [x] mo_mouse_pos -> no_mouse_text


### Non working functions in demo
- [ ] in histogram series min and max range should not be 0 and 1, but the min/max of the values
- [ ] check density flag of histogram series
- [ ] custom axis is missing

- [ ] Unsaved document flag is missing
- [ ] Hold button to repeat missing (see ImGUI's PushButtonRepeat)
- [ ] Flag "width fit preview" missing for combo (dpg.add_combo)
- [ ] Flag "mvColorEdit_AlphaPreviewXXX" does not seem to be working
