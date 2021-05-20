import dearpygui.core as dpg
import dearpygui.contexts as cxt

def show_demo():

    with cxt.collapsing_header(label="Layouts"):
        with cxt.tree_node(label="Ordered pack style"):
            dpg.add_button(label="Button 1")
            dpg.add_button(label="Button 2")
            dpg.add_button(label="Button 3")

        with cxt.tree_node(label="Out of order pack style"):
            with cxt.child(id="out of order pack parent", width=100, height=50):
                pass
            dpg.add_button(id="out of order button 1", label="Button 1")
            dpg.add_button(label="Button 2")
            dpg.add_button(label="Button 3", before="out of order button 1")
            dpg.add_button(label="Button 4", parent="out of order pack parent")

        with cxt.tree_node(label="Containers"):
            with cxt.tree_node(label="Groups"):
                dpg.add_text("Groups are used to control child items placement, width, and provide a hit box for this like finding if the set of items are hovered")
                with cxt.group(id="group 1", horizontal=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.group(width=150):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.group():
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
