import dearpygui.dearpygui as dpg
from dearpygui_ext.simple_table import mvSimpleTable
from dearpygui_ext.logger import mvLogger

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()


datagrid_data = []
rows = 100
columns = 4
for i in range(rows):
    new_row = []
    for j in range(columns):
        new_row.append(i)
    datagrid_data.append(new_row)

dpg.show_item_registry()

datagrid = mvSimpleTable(columns, datagrid_data)

with dpg.window(label="tutorial", width=500, height=500):
    dpg.add_button(label="clear", callback=lambda: datagrid.clear())
    dpg.add_button(label="add_row", callback=lambda: datagrid.add_row([58, 47, 6]))
    dpg.add_button(label="delete_row_3", callback=lambda: datagrid.delete_row(3))
    datagrid.submit()

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
