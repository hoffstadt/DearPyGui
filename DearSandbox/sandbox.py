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
    
    transformed_x = app_data[0]
    transformed_y = app_data[1]
    dpg.delete_item(sender, children_only=True, slot=2)
    dpg.push_container_stack(sender)
    for i in range(0, len(transformed_x)):
        #print(transformed_x[i], transformed_y[i])
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
            #dpg.add_candle_series(dates, opens, closes, lows, highs, label="GOOGL", time_unit=dpg.mvTimeUnit_Day)
            dpg.add_custom_series(x_data, y_data, 2, label="Custom", callback=custom_series_callback)
            dpg.fit_axis_data(dpg.top_container_stack())
        #dpg.fit_axis_data(xaxis)

dpg.set_primary_window(win, True)
dpg.show_viewport()
while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame()  

dpg.destroy_context()