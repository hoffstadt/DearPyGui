import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import dearpygui_ext.themes as themes
from dearpygui_ext.datagrid import mvDataGrid
import warnings

warnings.simplefilter('always', DeprecationWarning)

dpg.enable_docking()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

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
    datagrid.submit()

dpg.create_viewport()
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
