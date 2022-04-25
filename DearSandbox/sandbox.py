import dearpygui.dearpygui as dpg

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, tag="custom font"):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())


x_data = [0.0, 1.0, 2.0, 4.0, 5.0]
y_data = [0.0, 10.0, 20.0, 40.0, 50.0]

def custom_series_callback(sender, app_data, user_data):
    
    transformed_x = app_data[4]
    transformed_y = app_data[5]
    dpg.delete_item(sender, children_only=True, slot=2)
    dpg.push_container_stack(sender)
    dpg.set_item_label("ctt", str(app_data[0])+"\t"+str(app_data[1]))
    if app_data[2] < transformed_x[2]+15 and app_data[2] > transformed_x[2]-15:
        dpg.draw_circle((app_data[2], app_data[3]), 15, fill=(255, 255, 255, 255))
        dpg.configure_item("cs", tooltip=True)
    else:
        dpg.configure_item("cs", tooltip=False)
    for i in range(0, len(transformed_x)):
        dpg.draw_text((transformed_x[i]+15, transformed_y[i]-15), str(i), size=20)
        dpg.draw_circle((transformed_x[i], transformed_y[i]), 15, fill=(50+i*5, 50+i*50, 0, 255))
    dpg.pop_container_stack()

with dpg.window(label="Testing") as win:
    dates = [1546300800,1546387200,1546473600]
    opens = [1284.7,1319.9,1318.7]
    highs = [1284.75,1320.6,1327,1330.8]
    lows = [1282.85,1315,1318.7]
    closes = [1283.35,1315.3,1326.1]

    with dpg.plot(label="Candle Series", height=-1, width=-1):
        dpg.add_plot_legend()
        xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="Day", time=False)
        with dpg.plot_axis(dpg.mvYAxis, label="USD"):
            with dpg.custom_series(x_data, y_data, 2, label="Custom", callback=custom_series_callback, tag="cs"):
                dpg.add_button(label="Custom Tooltip", tag="ctt")
            dpg.fit_axis_data(dpg.top_container_stack())

dpg.show_metrics()
dpg.set_primary_window(win, True)
dpg.show_viewport()
while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame()  

dpg.destroy_context()