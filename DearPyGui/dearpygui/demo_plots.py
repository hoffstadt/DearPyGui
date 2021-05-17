import dearpygui.core as dpg
import dearpygui.contexts as cxt

def show_demo():

    with cxt.collapsing_header(label="Plots"):
        
        with cxt.tree_node(label="Help"):

            dpg.add_text("Plotting User Guide")
            dpg.add_text("Left click and drag within the plot area to pan X and Y axes.", bullet=True)
            dpg.add_text("Left click and drag on an axis to pan an individual axis.", bullet=True, indent=20)
            dpg.add_text("Scoll in the plot area to zoom both X and Y axes.", bullet=True)
            dpg.add_text("Scroll on an axis to zoom an individual axis.", bullet=True, indent=20)
            dpg.add_text("Right click and drag to box select data.", bullet=True)
            dpg.add_text("Hold Alt to expand box selection horizontally.", bullet=True, indent=20)
            dpg.add_text("Hold Shift to expand box selection vertically.", bullet=True, indent=20)
            dpg.add_text("Left click while box selecting to cancel the selection.", bullet=True, indent=20)
            dpg.add_text("Double left click to fit all visible data.", bullet=True)
            dpg.add_text("Double left click on an axis to fit the individual axis", bullet=True, indent=20)
            dpg.add_text("Double right click to open the plot context menu.", bullet=True)
            dpg.add_text("Click legend label icons to show/hide plot items.", bullet=True)
            dpg.add_text("IMPORTANT: By default, anti-aliased lines are turned OFF.", bullet=True)