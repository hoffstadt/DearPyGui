import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import dearpygui.themes as themes
from dearpygui.datagrid import mvDataGrid

dpg.enable_docking()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)

demo.show_demo()

datagrid_data = []
rows = 1000
columns = 10
for i in range(rows):
    new_row = []
    for j in range(columns):
        new_row.append(j)
    datagrid_data.append(new_row)

datagrid = mvDataGrid(columns, datagrid_data)

with dpg.window(label="tutorial", width=500, height=500):
    dpg.add_text("test hovering", label="test hovering")
    datagrid.submit()

dpg.setup_viewport()
dpg.start_dearpygui()
