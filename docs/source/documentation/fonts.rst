Fonts
=====

DPG embeds a copy of 'ProggyClean.ttf' (by Tristan Grimmer),
a 13 pixels high, pixel-perfect font used by default. ProggyClean does not scale smoothly,
therefore it is recommended that you load your own file when using DPG in an
application aiming to look nice and wanting to support multiple resolutions.

You do this by loading external .TTF/.OTF files. 
In the `Assets <https://github.com/hoffstadt/DearPyGui/tree/master/assets>`_
folder you can find an example of a otf font.

Readme First
------------

All loaded fonts glyphs are rendered into a single texture atlas ahead of time.
Adding/Removing/Modifying fonts will cause the font atlas to be rebuilt.

You can use the style editor
:py:func:`show_font_manager <dearpygui.dearpygui.show_font_manager>`
to browse your fonts and understand what's going on if you have an issue.

Font Loading Instructions
-------------------------

To add your own fonts, you must first create a font registry to
add fonts to. Next, add fonts to the registry. By default only basic latin
and latin supplement glyphs are added (0x0020 - 0x00FF).

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    # add a font registry
    with dpg.font_registry():
        # first argument ids the path to the .ttf or .otf file
        default_font = dpg.add_font("NotoSerifCJKjp-Medium.otf", 20)
        second_font = dpg.add_font("NotoSerifCJKjp-Medium.otf", 10)

    with dpg.window(label="Font Example", height=200, width=200):
        dpg.add_button(label="Default font")
        b2 = dpg.add_button(label="Secondary font")
        dpg.add_button(label="default")

        # set font of specific widget
        dpg.bind_font(default_font)
        dpg.bind_item_font(b2, second_font)

    dpg.show_font_manager()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Loading Specific Unicode Characters
-----------------------------------

There are several ways to add specific characters from a font file.
You can use range hints, ranges, and specific characters. You can also remap characters.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.font_registry():
        with dpg.font("NotoSerifCJKjp-Medium.otf", 20) as font1:

            # add the default font range
            dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)

            # helper to add range of characters
            #    Options:
            #        mvFontRangeHint_Japanese
            #        mvFontRangeHint_Korean
            #        mvFontRangeHint_Chinese_Full
            #        mvFontRangeHint_Chinese_Simplified_Common
            #        mvFontRangeHint_Cyrillic
            #        mvFontRangeHint_Thai
            #        mvFontRangeHint_Vietnamese
            dpg.add_font_range_hint(dpg.mvFontRangeHint_Japanese)

            # add specific range of glyphs
            dpg.add_font_range(0x3100, 0x3ff0)

            # add specific glyphs
            dpg.add_font_chars([0x3105, 0x3107, 0x3108])

            # remap や to %
            dpg.add_char_remap(0x3084, 0x0025)

    dpg.show_font_manager()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Where to find unicode character codes?
--------------------------------------

`Unicode Characters <https://en.wikipedia.org/wiki/List_of_Unicode_characters>`_


