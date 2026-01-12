import dearpygui.dearpygui as dpg

dpg.create_context()

results = []

def callback(sender, app_data, user_data):
    results.append(f"{user_data}: {app_data}")
    print(f"Callback - {user_data}: {app_data}")

with dpg.window(label="DragRange2 Test", width=500, height=400):
    dpg.add_text("Test: DragIntRange2 and DragFloatRange2")

    # Float range
    float_range = dpg.add_drag_float_range(
        label="Float Range",
        default_value=[0.25, 0.75],
        min_value=0.0,
        max_value=1.0,
        speed=0.01,
        callback=callback,
        user_data="float_range"
    )

    # Int range
    int_range = dpg.add_drag_int_range(
        label="Int Range",
        default_value=[10, 90],
        min_value=0,
        max_value=100,
        callback=callback,
        user_data="int_range"
    )

    dpg.add_separator()

    # Test get_value
    def test_get_value():
        fv = dpg.get_value(float_range)
        iv = dpg.get_value(int_range)
        print(f"get_value float_range: {fv}")
        print(f"get_value int_range: {iv}")
        assert isinstance(fv, list) and len(fv) == 2, f"Float range should return [min, max], got {fv}"
        assert isinstance(iv, list) and len(iv) == 2, f"Int range should return [min, max], got {iv}"
        print("get_value() test PASSED")

    dpg.add_button(label="Test get_value()", callback=lambda: test_get_value())

    # Test set_value
    def test_set_value():
        dpg.set_value(float_range, [0.1, 0.9])
        dpg.set_value(int_range, [20, 80])
        fv = dpg.get_value(float_range)
        iv = dpg.get_value(int_range)
        print(f"After set_value - float: {fv}, int: {iv}")
        assert abs(fv[0] - 0.1) < 0.01 and abs(fv[1] - 0.9) < 0.01, f"Float set_value failed: {fv}"
        assert iv == [20, 80], f"Int set_value failed: {iv}"
        print("set_value() test PASSED")

    dpg.add_button(label="Test set_value()", callback=lambda: test_set_value())

    # With clamped and no_input flags
    dpg.add_separator()
    dpg.add_text("With flags:")

    dpg.add_drag_float_range(
        label="Clamped",
        default_value=[0.3, 0.7],
        min_value=0.0,
        max_value=1.0,
        clamped=True
    )

    dpg.add_drag_int_range(
        label="No Input",
        default_value=[25, 75],
        min_value=0,
        max_value=100,
        no_input=True
    )

dpg.create_viewport(title='DragRange2 Test', width=600, height=500)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
