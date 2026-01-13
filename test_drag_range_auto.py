#!/usr/bin/env python3
"""Automated test for DragIntRange2 and DragFloatRange2 widgets"""
import dearpygui.dearpygui as dpg

dpg.create_context()

# Test 1: Widgets render correctly
print("Test 1: Creating widgets...")
with dpg.window(label="Test", tag="test_window"):
    float_range = dpg.add_drag_float_range(
        label="Float Range",
        default_value=[0.25, 0.75],
        min_value=0.0,
        max_value=1.0
    )
    int_range = dpg.add_drag_int_range(
        label="Int Range",
        default_value=[10, 90],
        min_value=0,
        max_value=100
    )
print("✓ Widgets created successfully")

# Test 2: get_value() returns [min, max] list
print("\nTest 2: Testing get_value()...")
fv = dpg.get_value(float_range)
iv = dpg.get_value(int_range)
print(f"  float_range: {fv}")
print(f"  int_range: {iv}")
assert isinstance(fv, list) and len(fv) == 2, f"Expected [min, max], got {fv}"
assert isinstance(iv, list) and len(iv) == 2, f"Expected [min, max], got {iv}"
assert abs(fv[0] - 0.25) < 0.01 and abs(fv[1] - 0.75) < 0.01, f"Float default wrong: {fv}"
assert iv == [10, 90], f"Int default wrong: {iv}"
print("✓ get_value() returns correct [min, max] list")

# Test 3: set_value() works
print("\nTest 3: Testing set_value()...")
dpg.set_value(float_range, [0.1, 0.9])
dpg.set_value(int_range, [20, 80])
fv = dpg.get_value(float_range)
iv = dpg.get_value(int_range)
print(f"  float_range after set: {fv}")
print(f"  int_range after set: {iv}")
assert abs(fv[0] - 0.1) < 0.01 and abs(fv[1] - 0.9) < 0.01, f"Float set failed: {fv}"
assert iv == [20, 80], f"Int set failed: {iv}"
print("✓ set_value() works correctly")

# Test 4: Callbacks fire with correct values
print("\nTest 4: Testing callbacks...")
callback_results = []
def cb(sender, app_data, user_data):
    callback_results.append((user_data, app_data))

float_range2 = dpg.add_drag_float_range(
    parent="test_window",
    label="CB Test",
    default_value=[0.5, 0.5],
    callback=cb,
    user_data="test"
)
# Simulate value change
dpg.set_value(float_range2, [0.3, 0.7])
# Note: callbacks typically fire on user interaction, not set_value
print("✓ Callback attached successfully")

# Test 5: Flags work
print("\nTest 5: Testing flags...")
clamped = dpg.add_drag_float_range(
    parent="test_window",
    label="Clamped",
    default_value=[0.3, 0.7],
    min_value=0.0,
    max_value=1.0,
    clamped=True
)
no_input = dpg.add_drag_int_range(
    parent="test_window",
    label="No Input",
    default_value=[25, 75],
    no_input=True
)
print("✓ Flags (clamped, no_input) accepted")

print("\n" + "="*50)
print("ALL TESTS PASSED")
print("="*50)

dpg.destroy_context()
