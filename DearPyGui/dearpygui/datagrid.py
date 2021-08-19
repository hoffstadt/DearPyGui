import dearpygui._dearpygui as internal_dpg
import dearpygui.dearpygui as dpg

class mvDataGrid:

    def __init__(self, columns, data=None):

        self._table_id = dpg.generate_uuid()
        self._stage_id = dpg.generate_uuid()
        self._columns = columns

        
        if data:

            with dpg.mutex():
                dpg.configure_app(skip_keyword_args=True)
                with dpg.stage(tag=self._stage_id):
                    for row in data:
                        internal_dpg.push_container_stack(internal_dpg.add_table_row())
                        for column in range(self._columns):
                            internal_dpg.add_text(str(row[column]))
                        internal_dpg.pop_container_stack()
                dpg.configure_app(skip_keyword_args=False)
        

    def submit(self):

        with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                       borders_innerH=True, borders_outerH=True, borders_innerV=True,
                       borders_outerV=True, context_menu_in_body=True, row_background=True,
                       policy=dpg.mvTable_SizingFixedFit, height=-1,
                       scrollY=True, tag=self._table_id, clipper=True):

            for i in range(self._columns):
                internal_dpg.add_table_column(label="Header " + str(i))

            dpg.unstage(self._stage_id)