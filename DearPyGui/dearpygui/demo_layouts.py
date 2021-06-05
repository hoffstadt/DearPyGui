import dearpygui.core as dpg
import dearpygui.simple as sdpg
import dearpygui.contexts as cxt
import dearpygui.common as cmn

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

        with cxt.tree_node(id="Absolute Position Placment"):
            dpg.add_button(label="Set Button 2 Pos", callback=lambda: dpg.set_item_pos(B2, x=50, y=125))
            dpg.add_button(label="Reset Button 2 Pos", callback=lambda: dpg.reset_pos(B2))
            dpg.add_button(label="Button 1", pos=[50,50], width=75, height=75)
            B2 = dpg.add_button(label="Button 2", width=75, height=75)
            dpg.add_button(label="Button 3")
        
        with cxt.tree_node(id="Grid Layout using Table API"):
            dpg.add_text("Tables can be used to layout items in an equally spaced grid pattern.")
            dpg.add_text("See tables section for more detail on tables.")
            dpg.add_checkbox(label="resizable", callback=cmn._config, user_data="layout_demo_table")
            dpg.add_checkbox(label="borders_innerH", callback=cmn._config, user_data="layout_demo_table", default_value=True)
            dpg.add_checkbox(label="borders_outerH", callback=cmn._config, user_data="layout_demo_table", default_value=True)
            dpg.add_checkbox(label="borders_innerV", callback=cmn._config, user_data="layout_demo_table", default_value=True)
            dpg.add_checkbox(label="borders_outerV", callback=cmn._config, user_data="layout_demo_table", default_value=True)
            with cxt.table(id="layout_demo_table", header_row=False, borders_innerH=True, borders_outerH=True, borders_innerV=True, borders_outerV=True):
                dpg.add_table_column()
                dpg.add_table_column()
                dpg.add_table_column()
                dpg.add_button(label="Button 1")
                dpg.add_table_next_column()
                dpg.add_button(label="Button 2")
                dpg.add_table_next_column()
                dpg.add_button(label="Button 3")
                dpg.add_table_next_column()
                dpg.add_table_next_column()
                dpg.add_table_next_column()
                dpg.add_button(label="Button 4")
                dpg.add_table_next_column()
                dpg.add_button(label="Button 5")

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
                with cxt.group(horizontal=True):
                    dpg.add_checkbox(label="autosize_x", callback=cmn._config, user_data="demo_layout_child")
                    dpg.add_checkbox(label="autosize_y", callback=cmn._config, user_data="demo_layout_child")
                    dpg.add_checkbox(label="menubar", callback=cmn._config, user_data="demo_layout_child")
                    dpg.add_checkbox(label="no_scrollbar", callback=cmn._config, user_data="demo_layout_child")
                    dpg.add_checkbox(label="horizontal_scrollbar", callback=cmn._config, user_data="demo_layout_child")
                    dpg.add_checkbox(label="border", default_value=True, callback=cmn._config, user_data="demo_layout_child")
                with cxt.child(id="demo_layout_child", width=200, height=200):
                    with cxt.menu_bar():
                        with cxt.menu(label="Menu"):
                            pass
                    for i in range(0, 20):
                        dpg.add_text(default_value="A pretty long sentence if you really think about it. It's also pointless. we need this to be even longer")
                with cxt.child(autosize_x=True, height=130, menubar=True):
                    with cxt.menu_bar():
                        dpg.add_menu(label="Menu Options")
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
                with cxt.tab_bar(reorderable=True)as tab_bar:
                    with cxt.tab(label="Tab 1"):
                        dpg.add_button(label="Tab 1 Button 1")
                        dpg.add_button(label="Tab 1 Button 2")
                        dpg.add_button(label="Tab 1 Button 3")
                    with cxt.tab(label="Tab 2"):
                        dpg.add_button(label="Tab 2 Button 1")
                        dpg.add_button(label="Tab 2 Button 2")
                        dpg.add_button(label="Tab 2 Button 3")
                    dpg.add_tab_button(label="Tab Button", callback=lambda: dpg.add_tab(parent=tab_bar))

        with cxt.tree_node(label="Simple Layouts"):
            dpg.add_text("Containers can be nested for advanced layout options")
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
