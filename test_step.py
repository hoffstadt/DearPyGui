import dearpygui.dearpygui as dpg

dpg.create_context()

def print_value(sender, app_data, user_data):
    print(f"{user_data}: {app_data}")

with dpg.window(label="Step/Snapping Test", width=500, height=400):
    dpg.add_text("Float Range Sliders with different step values:")

    # No snapping
    dpg.add_slider_float_range(
        label="No step (smooth)",
        default_value=[0.2, 0.8],
        min_value=0.0,
        max_value=1.0,
        callback=print_value,
        user_data="No step"
    )

    # Snap to 0.1
    dpg.add_slider_float_range(
        label="step=0.1",
        default_value=[0.2, 0.8],
        min_value=0.0,
        max_value=1.0,
        step=0.1,
        callback=print_value,
        user_data="step=0.1"
    )

    # Snap to 0.25
    dpg.add_slider_float_range(
        label="step=0.25",
        default_value=[0.25, 0.75],
        min_value=0.0,
        max_value=1.0,
        step=0.25,
        callback=print_value,
        user_data="step=0.25"
    )

    dpg.add_separator()
    dpg.add_text("Int Range Sliders with different step values:")

    # No snapping
    dpg.add_slider_int_range(
        label="No step (smooth)",
        default_value=[10, 90],
        min_value=0,
        max_value=100,
        callback=print_value,
        user_data="Int: No step"
    )

    # Snap to 5
    dpg.add_slider_int_range(
        label="step=5",
        default_value=[10, 90],
        min_value=0,
        max_value=100,
        step=5,
        callback=print_value,
        user_data="Int: step=5"
    )

    # Snap to 10
    dpg.add_slider_int_range(
        label="step=10",
        default_value=[20, 80],
        min_value=0,
        max_value=100,
        step=10,
        callback=print_value,
        user_data="Int: step=10"
    )

dpg.create_viewport(title='Step/Snapping Test', width=600, height=500)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
