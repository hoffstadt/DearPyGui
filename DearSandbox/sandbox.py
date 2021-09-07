import dearpygui.dearpygui as dpg

def _hsv_to_rgb(h, s, v):
    if s == 0.0: return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (255*v, 255*t, 255*p)
    if i == 1: return (255*q, 255*v, 255*p)
    if i == 2: return (255*p, 255*v, 255*t)
    if i == 3: return (255*p, 255*q, 255*v)
    if i == 4: return (255*t, 255*p, 255*v)
    if i == 5: return (255*v, 255*p, 255*q)

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

with dpg.theme() as global_theme:

    with dpg.theme_component(0):
        dpg.add_theme_color(dpg.mvThemeCol_Text, [0, 255, 0])

    with dpg.theme_component(dpg.mvButton):
        dpg.add_theme_color(dpg.mvThemeCol_Button, _hsv_to_rgb(2/7.0, 0.6, 0.6))
        dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, _hsv_to_rgb(2/7.0, 0.8, 0.8))
        dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, _hsv_to_rgb(2/7.0, 0.7, 0.7))
        dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 2*5)
        dpg.add_theme_style(dpg.mvStyleVar_FramePadding, 2*3, 2*3)

with dpg.theme() as specific_theme:

    with dpg.theme_component(0):
        dpg.add_theme_color(dpg.mvThemeCol_Button, [255, 0, 0])

with dpg.window(label="tutorial", width=500, height=500) as win:

    dpg.add_button(label="Press me 1")
    dpg.add_button(label="Press me 2")

    with dpg.child(height=-50) as child1:
        dpg.add_button(label="Press me 3")
        b1 = dpg.add_button(label="Press me 4")
        dpg.add_input_int(label="input int")
    dpg.add_button(label="Press me 5")

dpg.bind_item_theme(child1, global_theme)
dpg.bind_item_theme(b1, specific_theme)

dpg.create_viewport()
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()