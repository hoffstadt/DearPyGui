"""Test script for mixed_state_checkbox widget."""

import sys

sys.path.insert(0, '../output')  # Use built output

import dearpygui.dearpygui as dpg


def on_change(sender, app_data, user_data):
    states = {-1: "mixed", 0: "unchecked", 1: "checked"}
    print(f"Checkbox changed to: {app_data} ({states.get(app_data, 'unknown')})")

dpg.create_context()

with dpg.window(label="Mixed State Checkbox Test", width=400, height=300):
    dpg.add_text("Mixed state checkbox test:")
    dpg.add_text("Values: -1=mixed, 0=unchecked, 1=checked")
    dpg.add_separator()

    # Start in mixed state
    cb1 = dpg.add_mixed_state_checkbox(
        label="Mixed (start at -1)",
        default_value=-1,
        callback=on_change,
    )

    # Start unchecked
    cb2 = dpg.add_mixed_state_checkbox(
        label="Unchecked (start at 0)",
        default_value=0,
        callback=on_change,
        mixed_click_value=1
    )

    # Start checked
    cb3 = dpg.add_mixed_state_checkbox(
        label="Checked (start at 1)",
        default_value=1,
        callback=on_change,
        mixed_click_value=0
    )

    dpg.add_separator()

    def set_all_mixed():
        dpg.set_value(cb1, -1)
        dpg.set_value(cb2, -1)
        dpg.set_value(cb3, -1)

    def print_values():
        print(f"cb1={dpg.get_value(cb1)}, cb2={dpg.get_value(cb2)}, cb3={dpg.get_value(cb3)}")

    dpg.add_button(label="Set all to mixed (-1)", callback=set_all_mixed)
    dpg.add_button(label="Print values", callback=print_values)

dpg.create_viewport(title="Mixed State Checkbox Test", width=450, height=350)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
