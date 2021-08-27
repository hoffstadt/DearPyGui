| [Basics](#basics) - [Readme First](#readme-first) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Fonts) |
|-|
| [Font Loading Instructions](#font-loading-instructions) - [Loading Specific Unicode Characters](#loading-specific-unicode-characters) - [Where to find the unicode character codes?](#where-to-find-unicode-character-codes) |

## Basics
Dear ImGui (thus Dear PyGui also) embeds a copy of 'ProggyClean.ttf' (by Tristan Grimmer),
a 13 pixels high, pixel-perfect font used by default. ProggyClean does not scale smoothly, therefore it is recommended that you load your own file when using Dear PyGui in an application aiming to look nice and wanting to support multiple resolutions.

You do this by loading external .TTF/.OTF files. 
In the [Resources/](https://github.com/hoffstadt/DearPyGui/tree/master/Resources) folder you can find an example of a otf font.

## Readme First

- All loaded fonts glyphs are rendered into a single texture atlas ahead of time. Adding/Removing/Modifying fonts will cause the font atlas to be rebuilt.

- You can use the style editor `show_font_manager` from the *simple* module to browse your fonts and understand what's going on if you have an issue.

## Font Loading Instructions
To add your own fonts, you must first create a font registry to add fonts to. Next, add fonts to the registry. By default only basic latin and latin supplement glyphs are added (0x0020 - 0x00FF).

```python
import dearpygui.dearpygui as dpg

# add a font registry
with dpg.font_registry():
    
    # add font (set as default for entire app)
    dpg.add_font("path/to/font/file/CoolFont.otf", 20, default_font=True)

    # add second font
    dpg.add_font("path/to/font/file/AnotherCoolFont.ttf", 13, id="secondary_font")

with dpg.window(label="Font Example"):
    dpg.add_button(label="Default font")
    dpg.add_button(label="Secondary font")
    
    # set font of specific widget
    dpg.set_item_font(dpg.last_item(), "secondary_font")

dpg.start_dearpygui()
```

## Loading Specific Unicode Characters
There are several ways to add specific characters from a font file. You can use range hints, ranges, and specific characters. You can also remap characters. See Below:
```python
import dearpygui.dearpygui as dpg

with dpg.font_registry():
    
    with dpg.font("path/to/font/file/CoolFont.otf", 20, default_font=True):

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

dpg.start_dearpygui()
```

## Where to find unicode character codes?
[Unicode Characters](https://en.wikipedia.org/wiki/List_of_Unicode_characters)


