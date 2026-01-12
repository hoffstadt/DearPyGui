# Draft Comment for Issue #76

---

I've been working on a `SliderFloatRange2`/`SliderIntRange2` implementation and wanted to share it for feedback before formalizing a PR.

**Implementation approach:**

- **Template-based `SliderBehaviorRangeT<>`** following the existing `SliderBehaviorT` pattern
- **Reuses `ScaleRatioFromValueT`/`ScaleValueFromRatioT`** for value conversion (no duplication of that logic)
- **`GetStateStorage()->GetIntRef()`** for per-widget active handle tracking (no static variables)
- **Keyboard/gamepad navigation**: Up/Down switches between handles, Left/Right adjusts the active handle value
- **Ctrl+Click text input** via `TempInputScalar` (edits whichever handle is closer to click)
- **Single `SliderScalarRange2`** handles all data types; `SliderFloatRange2`/`SliderIntRange2` are thin wrappers

**Interaction model:** Uses click-to-position like standard sliders (not relative dragging like `DragFloatRange2`). On click, the nearest handle is selected and follows the mouse. Handles cannot cross each other.

**Code size:** ~250 lines for the behavior template + widget function, with float/int sharing the same code path.

**What it doesn't do:**
- Middle-bar dragging to move both handles together (could be added)
- Vertical slider variant (straightforward to add with axis flag)

Reference implementation: https://github.com/Entrpi/imgui/blob/docking/imgui_widgets_range.cpp

Would this direction be acceptable, or would you prefer a different interaction model (e.g., relative dragging)? Happy to adjust based on feedback.

---

**Notes for review:**
- The reference file shows the implementation without any extensions
- Tested with DearPyGui bindings - keyboard nav and Ctrl+Click work correctly
- Logarithmic scale support included
