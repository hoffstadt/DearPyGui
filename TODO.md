# Porting DPG

## HOW TO CREATE DEARPYGUI'S PYTHON FILES
If you have new params/widgets go to the root directory and run:
```
./build_release.sh
```
In any case (either you have new params/widgets or you just modified some internal stuff) go to your app folder and run:
```
pip install ../dearpygui/
```
(We guess that they have the same parent folder)

### TODO Generic
- [ ] Generic:
    - [ ] No downsampling in PyGui, probably problems with recognizing axes limits
    - [ ] Fix `void ImGui::Shutdown(): Assertion '(g.IO.BackendPlatformUserData == __null) && "Forgot to shutdown Platform backend?"' failed.` error on shutdown.
    - [ ] Enable control mods for plots
    - [ ] Check if what you did with DragPoint value is okay or not
    - [x] Find out how py files are created
    - [x] Remove GetKeysConstants because it's deeply linked to new IO (also, I put it, so it can be removed without any worries)
    - [ ] Make it work wihout obsolete functions (ImGui and ImPlot)
    - [ ] Update demo with new functions
- [ ] Bugfix:
    - [x] Crash when opening table in demo: `void ImGui::TableSetColumnEnabled(int, bool): Assertion 'table->Flags & ImGuiTableFlags_Hideable' failed.`
    - [x] Crash when opening some plots in demo:
        - [x] Line series: `void ImPlot::SetupLegend(ImPlotLocation, ImPlotLegendFlags): Assertion '((gp.CurrentPlot != nullptr && !gp.CurrentPlot->SetupLocked) || (gp.CurrentSubplot != nullptr && gp.CurrentPlot == nullptr)) && "Setup needs to be called after BeginPlot or BeginSubplots and before any setup locking functions (e.g. PlotX)!"' failed.` (Already investigated a little bit, probably some deep memory leak error)
        - [x] Query: `void ImPlot::SetupAxis(ImAxis, const char*, ImPlotAxisFlags): Assertion '(gp.CurrentPlot != nullptr && !gp.CurrentPlot->SetupLocked) && "Setup needs to be called after BeginPlot and before any setup locking functions (e.g. PlotX)!"' failed.`
    - [ ] While scrolling on plots before start tracing: `bool ImPlot::BeginPlot(const char*, const ImVec2&, ImPlotFlags): Assertion '(gp.CurrentPlot == nullptr) && "Mismatched BeginPlot()/EndPlot()!"' failed.`
    - [x] When adding mvTool_Metrics it gives `T& ImVector<T>::operator[](int) [with T = ImVec2]: Assertion 'i >= 0 && i < Size' failed.`
    - mvTool_Metrics doesn't show the "Presentation" plot anymore.
    - [ ] Crash when pressing button up/down in the `mvTool_ItemRegistry`, probably a problem with function MoveItemUp/Down in `mvItemRegistry.cpp`
- Not sure:
    - [ ] ImPlotItemFlags where should be applied? Is it okay to apply it at custom series?
    - [ ] Expose `GetMouseClickedCount()` to Python?


### TODO ImGui
- [x] [V 1.83](https://github.com/ocornut/imgui/releases/tag/v1.83)
    - [x] Tables: Expose `TableSetColumnEnabled()` in public api. (in the issue is explained that it's just Hide/Show)
- [ ] [V 1.84](https://github.com/ocornut/imgui/releases/tag/v1.84)
    - [ ] IO: Added `io.AddFocusEvent()` api for backend to tell when host window has gained/lost focus.
    - [ ] Disabled: Add `BeginDisabled()/EndDisabled()`.
    - [x] Tables: Added `ImGuiTableColumnFlags_Disabled`
    - [ ] Tables: Added `ImGuiTableColumnFlags_NoHeaderLabel` to request TableHeadersRow()
    - [ ] Check: Internals: (for custom widgets): because disabled items now sets HoveredID, if you want custom widgets to not react as hovered when disabled, in the majority of use cases it is preferable to check the "hovered" return value of ButtonBehavior() rather than HoveredId == id.
- [ ] [V 1.85](https://github.com/ocornut/imgui/releases/tag/v1.85)
    - [ ] Debug: New StackTool.
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
    - [ ] InputText: added ImGuiInputTextFlags_EscapeClearsAll and io.ConfigInputTextEnterKeepActive. Added Shift+Click style selection. Improvements for numerical inputs for IME mode sending full-width characters. Other fixes.
    - [ ] Internals: added wip internal APIs to allow handling input/shorting routing and key ownership. Things will be moved into public APIs over time, including a Shortcut() function that magically handle input routing.
    - [ ] Layout: Obsoleted using SetCursorPos() / SetCursorScreenPos() to extend parent window/cell boundaries. (Check what it actually changes)
    - [ ] Changed signature of ImageButton() function. (Already added a comment there, we need to "fix" the new ID and the frame padding)
    - [ ] Removed the bizarre legacy default argument for TreePush(const void* ptr = NULL). Must always pass a pointer value explicitly, NULL/nullptr is ok but require cast, e.g. TreePush((void*)nullptr); If you used TreePush() replace with TreePush((void*)NULL); (Just check this)
    - [ ] Commented out redirecting functions/enums names that were marked obsolete in 1.77 and 1.79 (Follow what written in the link)
- [x] [V 1.89.1](https://github.com/ocornut/imgui/releases/tag/v1.89.1)
- [ ] [V 1.89.2](https://github.com/ocornut/imgui/releases/tag/v1.89.2)
    - [ ] Misc: added GetItemID() in public API. (Check if this can be useful)
- [ ] [V 1.89.3](https://github.com/ocornut/imgui/releases/tag/v1.89.3)
    - [x] SeparatorText(): Added SeparatorText() widget.
    - [ ] Inputs, Scrolling: Made horizontal scroll wheel and horizontal scroll direction consistent across backends/os. (check)
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
    - [ ] IsItemHovered: Added ImGuiHoveredFlags_ForTooltip as a shortcut for pulling flags from style.HoverFlagsForTooltipMouse or style.HoverFlagsForTooltipNav depending on active inputs. style.HoverFlagsForTooltipMouse defaults to ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayShort. style.HoverFlagsForTooltipNav defaults to ImGuiHoveredFlags_NoSharedDelay | ImGuiHoveredFlags_DelayNormal.
    - [ ] IsItemHovered: Tweaked default value of style.HoverDelayNormal from 0.30 to 0.40, Tweaked default value of style.HoverDelayShort from 0.10 to 0.15.
    - [ ] IsWindowHovered: Added support for ImGuiHoveredFlags_Stationary
- [ ] [V 1.89.8](https://github.com/ocornut/imgui/releases/tag/v1.89.8)
    - [ ] IO: Obsoleted io.ClearInputCharacters() (added in 1.47) as it now ambiguous and often incorrect/misleading considering the existence of a higher-level input queue. This is automatically cleared by io.ClearInputsKeys().
    - [ ] ImDrawData: CmdLists[] array is now owned, changed from ImDrawList** to ImVector<ImDrawList*>. Majority of users shouldn't be affected, but you cannot compare to NULL nor reassign manually anymore. Instead use AddDrawList(). Allocation count are identical. 
    - [ ] Fonts: ImFontConfig::OversampleH now defaults to 2 instead of 3, since the quality increase is largely minimal.
    - [ ] IO: Added io.ClearEventsQueue() to clear incoming inputs events. 
    - [ ] Many modifications to Clipper
    - [ ] IO: Exposed io.PlatformLocaleDecimalPoint to configure decimal point ('.' or ',') for languages needing it.
- [ ] [V 1.90](https://github.com/ocornut/imgui/releases/tag/v1.90)
    - [ ] BeginChild(): Upgraded bool border = false parameter to ImGuiChildFlags flags = 0
    - [ ] BeginChild(): Added child-flag ImGuiChildFlags_AlwaysUseWindowPadding as a replacement for the window-flag ImGuiWindowFlags_AlwaysUseWindowPadding
    - [ ] BeginChildFrame()/EndChildFrame(): removed functions in favor of using BeginChild() with the ImGuiChildFlags_FrameStyle flag. Kept inline redirection function (will obsolete). Those functions were merely PushStyleXXX/PopStyleXXX helpers and custom versions are easy to create. (The removal isn't so much motivated by needing to add the feature in BeginChild(), but by the necessity to avoid BeginChildFrame() signature mismatching BeginChild() signature and features.) (probably already done)
    - [x] Removed IM_OFFSETOF() macro in favor of using offsetof() available in C++11.
    - [ ] ListBox, Combo: Changed signature of "name getter" callback in old one-liner ListBox()/Combo() apis
    - [ ] Commented out obsolete redirecting enums/functions that were marked obsolete two years ago:
        - [ ] ImDrawCornerFlags_XXX -> use ImDrawFlags_RoundCornersXXX names. Read 1.82 changelog for details + grep commented names in sources.
        - [ ] Commented out runtime support for hardcoded ~0 or 0x01..0x0F rounding flags values for AddRect()/AddRectFilled()/PathRect()/AddImageRounded(). -> Use ImDrawFlags_RoundCornersXXX flags. Read 1.82 changelog for details.
    - [ ] (Windows) BeginChild(): Added ImGuiChildFlags_ResizeX and ImGuiChildFlags_ResizeY to allow resizing child windows from the bottom/right border (toward layout direction). 
- [ ][V 1.90.1](https://github.com/ocornut/imgui/releases/tag/v1.90.1)
    - [ ] imgui_freetype: commented out ImGuiFreeType::BuildFontAtlas() obsoleted in 1.81. Prefer using #define IMGUI_ENABLE_FREETYPE or see commented code for manual runtime calls.
    - [ ] Tabs: Added ImGuiTabItemFlags_NoAssumedClosure to enable app to react on closure attempt, without having to draw an unsaved document marker (ImGuiTabItemFlags_UnsavedDocument sets _NoAssumedClosure automatically).
    - [ ] Debug Tools: Added io.ConfigDebugIsDebuggerPresent option. 
    - [ ] Added DebugFlashStyleColor() to identify a style color. Added to Style Editor.

### TODO ImPlot
Take a look at implot.cpp to have a better changelog.

- [ ] [V 0.13](https://github.com/epezent/implot/releases/tag/v0.13)
    - [ ] Query: Implement new DragRect (now commented)
    - [ ] Axis: custom tick label formatting via ImPlotFormatter callbacks passed to SetupAxisFormat
    - [ ] TagX and TagY have been added and allow you to add custom labels to axes. These use either the axis' formatter, or a custom provided string. You can combine these with other tools like DragLine to create custom interactivity.
    - [ ] DragPoint and DragLine no longer take a string ID. Instead they now expect an int ID, and as a result no longer display the optional label. Instead, use TagX/Y and Annotation to add labels too these tools if needed.
    - [ ] additional options for drag tools via ImPlotDragToolFlags
    - [ ] Bar plots can now be plotted in groups and stacked.
- [ ] [V 0.14](https://github.com/epezent/implot/releases/tag/v0.14)
    - [ ] Each of ImPlot's PlotX functions now takes an optional ImPlotXFlags parameter.
    - [ ] Change in plots (https://github.com/epezent/implot/releases/tag/v0.14#:~:text=Besides%20that%2C%20items%20flags%20brings%20about%20a%20few%20other%20API%20breaking%20changes%20users%20should%20be%20aware%20of%3A)
    - [ ] Previously, users could define time and log scales with ImPlotAxisFlags_Log, and ImPlotAxisFlags_Time. These flags have been replaced with a more general system using a new the setup function SetupAxisScale
    - [ ] You can now constrain axes limits so that users can't pan beyond a min or max value, or zoom beyond a min or max width/height
    - [ ] You can now customize the supported types by defining IMPLOT_CUSTOM_NUMERIC_TYPES at compile time to define your own type list (probably not interesting for us)
    - [x] Line plots now honor ImGui's AntiAliasedLines and AntiAliasedLinesUseTex. That's right, ImPlot now uses texture based AA!
    - [ ] Legend entries can be sorted using ImPlotLegendFlags_Sort
- [x] [V 0.15](https://github.com/epezent/implot/releases/tag/v0.15)
- [x] [V 0.16](https://github.com/epezent/implot/releases/tag/v0.16) 


### Completed Column ✓
- [x] mo_mouse_pos -> no_mouse_text