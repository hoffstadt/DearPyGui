import dearpygui.core as dpg
import dearpygui.contexts as cxt

def show_demo():

    with cxt.collapsing_header(label="Tables"):

        with cxt.tree_node(label="Basic"):

            # basic usage of the table api
            with cxt.table(header_row=False):

                # use add_table_column to add columns to the table,
                # table columns use slot 0
                dpg.add_table_column()
                dpg.add_table_column()
                dpg.add_table_column()

                # add_table_next_column will jump to the next row
                # once it reaches the end of the columns
                # table next column use slot 1
                for i in range(0, 4):
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")
                        if not (i == 3 and j == 2):
                            dpg.add_table_next_column()

        with cxt.tree_node(label="Borders, background") as section:

            with cxt.table(header_row=False, row_background=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True) as table_id:

                dpg.add_table_column(label="Header 1")
                dpg.add_table_column(label="Header 2")
                dpg.add_table_column(label="Header 3")

                for i in range(0, 5):
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")
                        if not (i == 4 and j == 2):
                            dpg.add_table_next_column()

                dpg.add_checkbox(label="row_background", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, row_background=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_innerH", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerH=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_innerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_outerH", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerH=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_outerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="header_row", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, header_row=dpg.get_value(sender)))

        with cxt.tree_node(label="Resizable, stretch"):

            with cxt.table(header_row=False, resizable=True,
                        borders_outerH=True, borders_innerV=True, borders_outerV=True) as table_id:

                dpg.add_table_column(label="Header 1")
                dpg.add_table_column(label="Header 2")
                dpg.add_table_column(label="Header 3")

                for i in range(0, 5):
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")
                        if not (i == 4 and j == 2):
                            dpg.add_table_next_column()

            dpg.add_checkbox(label="borders_innerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
            dpg.add_checkbox(label="borders_outerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
            dpg.add_checkbox(label="resizable", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=dpg.get_value(sender)))

        with cxt.tree_node(label="Resizable, fixed"):

            dpg.add_text("Only available if scrollX/scrollY are disabled and stretch columns are not used")
            with cxt.table(header_row=False, policy=dpg.mvTable_SizingFixedFit, resizable=True, no_host_extendX=False, 
                        borders_innerV=True, borders_outerV=True,borders_outerH=True) as table_id:

                dpg.add_table_column(label="Header 1")
                dpg.add_table_column(label="Header 2")
                dpg.add_table_column(label="Header 3")

                for i in range(0, 5):
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")
                        if not (i == 4 and j == 2):
                            dpg.add_table_next_column()

            dpg.add_checkbox(label="no_host_extendX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendX=dpg.get_value(sender)))

        with cxt.tree_node(label="Resizable, mixed"):

            with cxt.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                        resizable=True, no_host_extendX=False, hideable=True, 
                        borders_innerV=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):

                dpg.add_table_column(label="AAA", width_fixed=True)
                dpg.add_table_column(label="BBB", width_fixed=True)
                dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)

                for i in range(0, 5):
                    for j in range(0, 3):
                        if j == 2:
                            dpg.add_text(f"Stretch {i}, {j}")
                        else:
                            dpg.add_text(f"Fixed {i}, {j}")
                        if not (i == 4 and j == 2):
                            dpg.add_table_next_column()

            with cxt.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                        resizable=True, no_host_extendX=False, hideable=True, 
                        borders_innerV=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):

                dpg.add_table_column(label="AAA", width_fixed=True)
                dpg.add_table_column(label="BBB", width_fixed=True)
                dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)
                dpg.add_table_column(label="DDD", width_stretch=True, init_width_or_weight=0.0)

                for i in range(0, 5):
                    for j in range(0, 4):
                        if j == 2 or j == 3:
                            dpg.add_text(f"Stretch {i},{j}")
                        else:
                            dpg.add_text(f"Fixed {i}, {j}")
                        if not (i == 4 and j == 3):
                            dpg.add_table_next_column()

        with cxt.tree_node(label="Padding"):

            with cxt.table(header_row=False, resizable=True,
                        hideable=True, reorderable=True, borders_outerV=True, borders_innerH=True) as table_id:

                dpg.add_table_column(label="One")
                dpg.add_table_column(label="Two")
                dpg.add_table_column(label="three")

                for i in range(0, 5):
                    for j in range(0, 3):
                        dpg.add_button(label=f"Hello {i}, {j}", width=-1)
                        if not (i == 4 and j == 2):
                            dpg.add_table_next_column()

            dpg.add_checkbox(label="pad_outerX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, pad_outerX=dpg.get_value(sender)))
            dpg.add_checkbox(label="no_pad_outerX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_pad_outerX=dpg.get_value(sender)))
            dpg.add_checkbox(label="no_pad_innerX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_pad_innerX=dpg.get_value(sender)))
            dpg.add_checkbox(label="borders_outerV", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
            dpg.add_checkbox(label="borders_innerV", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
            dpg.add_checkbox(label="header_row", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, header_row=dpg.get_value(sender)))

        with cxt.tree_node(label="Reorderable, hideable, with headers"):

            with cxt.table(header_row=True, resizable=True,
                        hideable=True, reorderable=True) as table_id:

                dpg.add_table_column(label="One")
                dpg.add_table_column(label="Two")
                dpg.add_table_column(label="three")

                for i in range(0, 5):
                    for j in range(0, 3):
                        dpg.add_text(f"Hello {i}, {j}")
                        if not (i == 4 and j == 2):
                            dpg.add_table_next_column()

            dpg.add_checkbox(label="hideable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, hideable=dpg.get_value(sender)))
            dpg.add_checkbox(label="reorderable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, reorderable=dpg.get_value(sender)))
            dpg.add_checkbox(label="resizable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=dpg.get_value(sender)))

        with cxt.tree_node(label="Outer Size"):

            with cxt.table(header_row=False, no_host_extendX=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, context_menu_in_body=True, row_background=True,
                        policy=dpg.mvTable_SizingFixedFit, height=150) as table_id:
                
                dpg.add_table_column(label="One")
                dpg.add_table_column(label="Two")
                dpg.add_table_column(label="three")

                for i in range(0, 10):
                    for j in range(0, 3):
                        dpg.add_text(f"Cell {i}, {j}")
                        if not (i == 9 and j == 2):
                            dpg.add_table_next_column()

            dpg.add_checkbox(label="no_host_extendX", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendX=dpg.get_value(sender)))
            dpg.add_checkbox(label="no_host_extendY", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendY=dpg.get_value(sender)))


            dpg.add_text("Using explicit size:")
            with cxt.table(header_row=False, no_host_extendX=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, context_menu_in_body=True, row_background=True,
                        policy=dpg.mvTable_SizingFixedFit, height=300, width=300):
                
                dpg.add_table_column(label="One")
                dpg.add_table_column(label="Two")
                dpg.add_table_column(label="three")

                for i in range(0, 6):
                    for j in range(0, 3):
                        dpg.add_text(f"Cell {i}, {j}")
                        if not (i == 9 and j == 2):
                            dpg.add_table_next_column()

        with cxt.tree_node(label="Scrolling, Clipping"):

            # without clipping
            dpg.add_text("Without Clipper")
            with cxt.table(header_row=True, no_host_extendX=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, context_menu_in_body=True, row_background=True,
                        policy=dpg.mvTable_SizingFixedFit, height=300,
                        scrollY=True):

                dpg.add_table_column(label="1")
                dpg.add_table_column(label="2")
                dpg.add_table_column(label="3")

                for i in range(0, 100):
                    with cxt.table_row():
                        dpg.add_input_int(label=" ", step=0)
                        dpg.add_button(label=f"Cell {i}, 1")
                        dpg.add_text(f"Cell {i}, 2")

            # with clipping
            dpg.add_text("Using Clipper")
            with cxt.table(header_row=True, no_host_extendX=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, context_menu_in_body=True, row_background=True,
                        policy=dpg.mvTable_SizingFixedFit, height=300,
                        scrollY=True):

                dpg.add_table_column(label="1")
                dpg.add_table_column(label="2")
                dpg.add_table_column(label="3")

                for i in range(0, 100):
                    with cxt.clipper():
                        with cxt.table_row():
                            dpg.add_input_int(label=" ", step=0)
                            dpg.add_button(label=f"Cell {i}, 1")
                            dpg.add_text(f"Cell {i}, 2")
            dpg.add_checkbox(label="resizable", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=dpg.get_value(sender)))

            # Freezing rows/columns
            dpg.add_text("Freezing rows/columns")
            with cxt.table(header_row=True, borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, row_background=True, height=300, freeze_rows=1, freeze_columns=1,
                        scrollY=True, scrollX=True, policy=dpg.mvTable_SizingFixedFit):

                dpg.add_table_column(label="1", width=50)
                dpg.add_table_column(label="2", width=50)
                dpg.add_table_column(label="3", width=50)
                dpg.add_table_column(label="4", width=50)
                dpg.add_table_column(label="5", width=50)
                dpg.add_table_column(label="6", width=50)
                dpg.add_table_column(label="7", width=50)

                for i in range(0, 100):
                        with cxt.table_row():
                            dpg.add_text(f"Cell {i}, 0")
                            dpg.add_button(label=f"Cell {i}, 1")
                            dpg.add_text(f"Cell {i}, 2")
                            dpg.add_text(f"Cell {i}, 3")
                            dpg.add_text(f"Cell {i}, 4")
                            dpg.add_text(f"Cell {i}, 5")
                            dpg.add_text(f"Cell {i}, 6")

        with cxt.tree_node(label="Sorting"):

            def sort_callback(sender, app_data):

                children = dpg.get_item_info(sender)["children"][1]
                    
                oldList = []
                col1 = []
                col2 = []
                i = 0
                j = 0
                while i < len(children)-5:
                    row = []

                    col1.append(children[i])
                    col2.append(children[i+2])

                    row.append(children[i])
                    row.append(children[i+1])
                    row.append(children[i+2])
                    row.append(children[i+3])
                    row.append(children[i+4])
                    row.append(children[i+5])
                    row.append(j)
                    oldList.append(row)
                    i+=6
                    j+=1
                        
                col1values = dpg.get_values(col1)
                col2values = dpg.get_values(col2)

                def col1_sorter(e):
                    return col1values[e[6]]
                def col2_sorter(e):
                    return col2values[e[6]]

                reverse = False
                if app_data[0][1] < 0:
                    reverse = True

                if app_data[0][0] == dpg.get_item_info(sender)["children"][0][0]:
                    oldList.sort(key=col1_sorter, reverse=reverse)
                elif app_data[0][0] == dpg.get_item_info(sender)["children"][0][1]:
                    oldList.sort(key=col2_sorter, reverse=reverse)

                single_list = []
                for row in oldList:
                    for cell in range(0, len(row)-1):
                        single_list.append(row[cell])
                        
                dpg.reorder_items(sender, 1, single_list)

            with cxt.table(header_row=True, no_host_extendX=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, context_menu_in_body=True, row_background=True,
                        policy=dpg.mvTable_SizingFixedFit, height=500, sortable=True, callback=sort_callback,
                        scrollY=True) as table_id:

                dpg.add_table_column(label="One")
                dpg.add_table_column(label="Two")
                dpg.add_table_column(label="Three")

                for i in range(0, 1000):
                        dpg.add_input_int(label=" ", step=0)
                        dpg.add_table_next_column()
                        dpg.add_text(f"Cell {i}, 1")
                        dpg.add_table_next_column()
                        dpg.add_checkbox(label=f"Cell {i}, 2")
                        if i != 999:
                            dpg.add_table_next_column()

            dpg.add_checkbox(label="sort_multi", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, sort_multi=dpg.get_value(sender)))
            dpg.add_checkbox(label="sort_tristate", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, sort_tristate=dpg.get_value(sender)))

        with cxt.tree_node(label="Sizing Policy"):

            with cxt.table(header_row=False, no_host_extendX=True,
                        borders_innerH=True, borders_outerH=True, borders_innerV=True,
                        borders_outerV=True, context_menu_in_body=True, row_background=True) as table_id:
                
                dpg.add_table_column()
                dpg.add_table_column()
                dpg.add_table_column()

                for i in range(0, 8):
                    dpg.add_text("Oh dear")
                    dpg.add_table_next_column()
                dpg.add_text("Oh dear")


            dpg.add_checkbox(label="resizable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendX=dpg.get_value(sender)))
            dpg.add_checkbox(label="no_host_extendY", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendY=dpg.get_value(sender)))
