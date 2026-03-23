"""Runs the demo application using the locally built DearPyGui in the output directory."""

import sys

sys.path.insert(0, '../output')  # Use built output

import dearpygui.dearpygui as dpg
import dearpygui.demo as demo

dpg.create_context()
dpg.create_viewport(title='Custom Title', width=600, height=600)

demo.show_demo()

dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
