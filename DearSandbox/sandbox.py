import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
import dearpygui.demo as demo

with cxt.font_registry():
    fonty1 = dpg.add_font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True)
    fonty2 = dpg.add_font("../../Resources/Retron2000.ttf", 13)

with cxt.theme() as theme1:
    dpg.add_theme_color(21, (255, 255, 0))

main_texture_container = dpg.generate_uuid()

dpg.add_texture_container(id=main_texture_container)
dpg.add_static_texture(100, 100, [], parent=main_texture_container, file=dpg.mvFontAtlas, id=dpg.mvFontAtlas)

#demo.show_demo()

with cxt.window(label="Testing font") as targeting:

    dpg.add_button(label = "Change font1", callback=lambda:dpg.set_item_font(targeting, fonty1))
    dpg.add_button(label = "Change font2", callback=lambda:dpg.set_item_font(targeting, fonty2))
    dpg.add_button(label = "Change theme", callback=lambda s:dpg.set_item_theme(s, theme1))

smpl.start_dearpygui()