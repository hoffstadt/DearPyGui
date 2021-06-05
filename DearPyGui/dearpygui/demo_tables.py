import dearpygui.core as dpg
import dearpygui.contexts as cxt

def show_demo():

    with cxt.collapsing_header(label="Tables"):

        with cxt.tree_node(label="Basic"):

            # basic usage of the table api
            with table(header_row=False):

                # use add_table_column to add columns to the table
                add_table_column()
                add_table_column()
                add_table_column()

                # add_table_next_column will jump to the next row
                # once it reaches the end of the columns
                for i in range(0, 4):
                    for j in range(0, 3):
                        add_text(f"Row{i} Column{j}")
                        if not (i == 3 and j == 2):
                            add_table_next_column()
