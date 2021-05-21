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

        with cxt.tree_node(label="Absolute Position Placment"):
            pass

        with cxt.tree_node(label="Containers"):
            with cxt.tree_node(label="Tree Nodes"):
                with cxt.tree_node(label="Tree Node (selectable)", selectable=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.tree_node(label="Tree Node (bullet)", bullet=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
            with cxt.tree_node(label="Groups"):
                dpg.add_text("Groups are used to control child items placement, width, and provide a hit box for things like is the set of items are hovered, ect...")
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
            with cxt.tree_node(label="Child windows"):
                dpg.add_text("Child windows are basically embedded windows and provide much more structure and control of the containing items than groups.")
                with cxt.child(autosize_x=True, height=130, menubar=True):
                    with cxt.menu_bar():
                        dpg.add_menu(label="Menu Options")
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.child(border=False, width=100, height=100):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.child(width=100, height=150, horizontal_scrollbar=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                    dpg.add_button(label="Button 4", width=150)
                    dpg.add_button(label="Button 5")
                    dpg.add_button(label="Button 6")
                dpg.add_same_line()
                with cxt.child(width=100, height=110):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
            with cxt.tree_node(label="Collapsing Headers"):
                with cxt.collapsing_header(label="Collapsing Header"):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.collapsing_header(label="Collapsing Header (close)", closable=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.collapsing_header(label="Collapsing Header (bullet)", bullet=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
                with cxt.collapsing_header(label="Collapsing Header (leaf)", leaf=True):
                    dpg.add_button(label="Button 1")
                    dpg.add_button(label="Button 2")
                    dpg.add_button(label="Button 3")
            with cxt.tree_node(label="Tab Bar"):
                with cxt.tab_bar(reorderable=True):
                    with cxt.tab(label="Tab 1"):
                        dpg.add_button(label="Tab 1 Button 1")
                        dpg.add_button(label="Tab 1 Button 2")
                        dpg.add_button(label="Tab 1 Button 3")
                    with cxt.tab(label="Tab 2"):
                        dpg.add_button(label="Tab 2 Button 1")
                        dpg.add_button(label="Tab 2 Button 2")
                        dpg.add_button(label="Tab 2 Button 3")

        with cxt.tree_node(label="Simple Layouts"):
            dpg.add_text("Containers can be nested for more layout options")
            with cxt.tree_node(label="Page Layout"):
                with cxt.child(width=500, height=320):
                    with cxt.menu_bar():
                        dpg.add_menu(label="Menu Options")
                    with cxt.child(autosize_x=True, height=95):
                        with cxt.group(horizontal=True):
                            dpg.add_button(label="Header 1", width=75, height=75)
                            dpg.add_button(label="Header 2", width=75, height=75)
                            dpg.add_button(label="Header 3", width=75, height=75)
                    with cxt.child(autosize_x=True, height=175):
                        with cxt.group(horizontal=True, width=0):
                            with cxt.child(width=102, height=150):
                                with cxt.tree_node(label="Nav 1"):
                                    dpg.add_button(label="Button 1")
                                with cxt.tree_node(label="Nav 2"):
                                    dpg.add_button(label="Button 2")
                                with cxt.tree_node(label="Nav 3"):
                                    dpg.add_button(label="Button 3")
                            with cxt.child(width=300, height=150):
                                dpg.add_button(label="Button 1")
                                dpg.add_button(label="Button 2")
                                dpg.add_button(label="Button 3")
                            with cxt.child(width=50, height=150):
                                dpg.add_button(label="B1", width=25, height=25)
                                dpg.add_button(label="B2", width=25, height=25)
                                dpg.add_button(label="B3", width=25, height=25)
                    with cxt.group(horizontal=True):
                        dpg.add_button(label="Footer 1", width=175)
                        dpg.add_text("Footer 2")
                        dpg.add_button(label="Footer 3", width=175)
