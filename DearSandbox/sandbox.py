import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import dearpygui.themes as themes
from dearpygui.datagrid import mvDataGrid

dpg.enable_docking()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)

demo.show_demo()


datagrid_data = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12],
    ]

datagrid = mvDataGrid(4, datagrid_data)

with dpg.window(label="tutorial"):
    dpg.add_button(label="Press me")
    datagrid.submit()


dpg.setup_viewport()
dpg.start_dearpygui()
