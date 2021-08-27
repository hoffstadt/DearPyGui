| [Basics](#basics) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Developer-Tools) |
|----|
||

## Basics
_Dear PyGui_ constains several useful developer tools which can help debug _Dear PyGui_ applications. They can be seen below:

```python
import dearpygui.dearpygui as dpg
import dearpygui.logger as dpg_logger

logger = dpg_logger.mvLogger()

dpg.show_documentation()
dpg.show_style_editor()
dpg.show_debug()
dpg.show_about()
dpg.show_metrics()
dpg.show_font_manager()
dpg.show_item_registry()

dpg.start_dearpygui() 
```

Results:

![](https://github.com/hoffstadt/DearPyGui/blob/assets/examples_wiki_0.8.x/builtin_Dev_tools.PNG)