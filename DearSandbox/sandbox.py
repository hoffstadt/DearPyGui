import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import dearpygui_ext.themes as themes

dpg.create_context()
dpg.enable_docking()
dpg.create_viewport()
dpg.setup_dearpygui()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, tag="custom font"):
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

with dpg.window(label="tutorial", width=500, height=500, show=False):
    dpg.add_button(label="Press me", callback=lambda:dpg.toggle_viewport_fullscreen())
    datagrid.submit()

# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame()  

dpg.destroy_context()
