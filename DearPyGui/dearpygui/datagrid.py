import dearpygui._dearpygui as internal_dpg
import dearpygui.dearpygui as dpg

class mvDataGrid:

    def __init__(self, columns, data=None):

        self._table_id = dpg.generate_uuid()
        self._columns = columns
        self._data = data
        self._selection_color1 = [255, 0, 0, 75]
        self._selection_color2 = [0, 10, 255, 75]

    def show_column(self, column):
        col = dpg.get_item_children(self._table_id, 0)[column]
        dpg.show_item(col)

    def hide_column(self, column):
        col = dpg.get_item_children(self._table_id, 0)[column]
        dpg.hide_item(col)

    def select_row(self, row):
        dpg.highlight_table_row(self._table_id, row, self._selection_color1)

    def unselect_row(self, row):
        dpg.unhighlight_table_row(self._table_id, row)

    def select_column(self, column):
        dpg.highlight_table_column(self._table_id, column, self._selection_color2)

    def unselect_column(self, column):
        dpg.unhighlight_table_column(self._table_id, column)

    def select_cell(self, row, column):
        dpg.highlight_table_cell(self._table_id, row, column, self._selection_color1)

    def unselect_cell(self, row, column):
        dpg.unhighlight_table_cell(self._table_id, row, column)

    def set_row_color(self, row, color):
        dpg.set_table_row_color(self._table_id, row, color)

    def unset_row_color(self, row):
        dpg.unset_table_row_color(self._table_id, row)

    def submit(self):

        with dpg.group():
            dpg.add_button(label="select row 2", callback=lambda: self.select_row(2))
            dpg.add_button(label="unselect row 2", callback=lambda: self.unselect_row(2))
            dpg.add_button(label="select column 5", callback=lambda: self.select_column(5))
            dpg.add_button(label="unselect column 1", callback=lambda: self.unselect_column(1))
            dpg.add_button(label="select cell 4,3", callback=lambda: self.select_cell(4, 3))
            dpg.add_button(label="unselect cell 4,3", callback=lambda: self.unselect_cell(4, 3))
            dpg.add_button(label="set row 6 color", callback=lambda: self.set_row_color(6, [0, 255, 0, 100]))
            dpg.add_button(label="unset row 6 color", callback=lambda: self.unset_row_color(6))

        dpg.add_same_line()

        with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                       borders_innerH=True, borders_outerH=True, borders_innerV=True,
                       borders_outerV=True, context_menu_in_body=True, row_background=True,
                       policy=dpg.mvTable_SizingFixedFit, height=-1,
                       scrollY=True, id=self._table_id, hideable=True, clipper=True):

            for i in range(self._columns):
                internal_dpg.add_table_column(label="Header " + str(i))

            for i in range(len(self._data)):
                internal_dpg.push_container_stack(internal_dpg.add_table_row())
                for j in range(self._columns):
                    internal_dpg.add_text(str(self._data[i][j]))
                internal_dpg.pop_container_stack()