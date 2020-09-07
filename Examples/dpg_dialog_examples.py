from dearpygui.dearpygui import *

add_text("Tips")
add_text("These are some of the standard dialogs used by Dear PyGui.", bullet=True)
add_text("Logger        - standard logger", bullet=True)
add_text("About         - about window", bullet=True)
add_text("Metrics       - useful for debugging.", bullet=True)
add_text("Documentation - tips, command, and searchable documentation.", bullet=True)
add_text("Debug         - useful for debugging", bullet=True)

add_spacing()
add_seperator()
add_spacing()

add_button("Show Logger", callback=show_logger)
add_button("Show About", callback=show_about)
add_button("Show Metrics", callback=show_metrics)
add_button("Show Documentation", callback=show_documentation)
add_button("Show Debug", callback=show_debug)

start_dearpygui()
