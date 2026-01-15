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

All loaded fonts glyphs are rendered into a single texture atlas. Rendering occurs
on-the-fly as characters are encountered in UI strings (item labels, input contents, etc. -
anything that gets displayed in the viewport).

You can use the Fonts Manager tool
:py:func:`show_font_manager <dearpygui.dearpygui.show_font_manager>`
to browse your fonts and understand what's going on if you have an issue.

Font Loading Instructions
-------------------------

To add your own fonts, you must first create a font registry to
add fonts to. Next, add fonts to the registry.

For a font to take effect, it must be either bound globally with :py:func:`bind_font <dearpygui.dearpygui.bind_font>`
or bound to a particular item with :py:func:`bind_item_font <dearpygui.dearpygui.bind_item_font>`.
In the latter case, the font is "inherited" by all child item if it is bound to
a container. To remove font binding and revert to the default font (or inherit from
the parent item), call `bind_font`/`bind_item_font` again, passing 0 in the `font`
argument.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    # add a font registry
    with dpg.font_registry():
        # first argument ids the path to the .ttf or .otf file
        default_font = dpg.add_font("NotoSerifCJKjp-Medium.otf", 20)
        second_font = dpg.add_font("NotoSerifCJKjp-Medium.otf", 10)

    dpg.bind_font(default_font)

    with dpg.window(label="Font Example", height=200, width=200):
        dpg.add_button(label="Default font")
        b2 = dpg.add_button(label="Secondary font")
        dpg.add_button(label="default")

        # set font of specific widget
        dpg.bind_item_font(b2, second_font)

    dpg.show_font_manager()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Loading Specific Unicode Characters
-----------------------------------

.. note:: 
    This section previously described how to load specific characters from font file
    so that they are displayed in the UI correctly. Since DPG version 2.3, character
    glyphs are rendered automatically and there is no need to specify them in advance.
    This section is no longer relevant, and the functions `add_font_chars`, `add_font_range`,
    and `add_font_range_hint` are obsolete and do nothing if you call them.

Remapping Characters
-----------------------------------

For convenience, you can remap a character to a different character code and use
this replacement in your text strings. In particular, it might be useful with icon
fonts. Here is an example of how to do such remapping.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.font_registry():
        with dpg.font("NotoSerifCJKjp-Medium.otf", 20) as font1:

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


