import dearpygui._dearpygui as idpg
import dearpygui.dearpygui as dpg
import time
dpg.set_init_file()

class mvTimer:

    def __init__(self):

        self._timer_mark = time.perf_counter()

    def mark(self, message):

        old_time = self._timer_mark
        self._timer_mark = time.perf_counter()
        print(message, self._timer_mark - old_time)

columns = 10
rows = 500000


timer = mvTimer()

with dpg.item_pool():
    dpg.add_item_set(dpg.mvCheckbox, rows*columns+1)

timer.mark("Time to create pool:\t")

with dpg.template_registry(id="temp"):
    dpg.add_checkbox(label="new label", default_value=True)

dpg.bind_template_registry("temp")
dpg.configure_item_registry(skip_optional_args=True)

with dpg.window(label="Profiling"):

    with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                   borders_innerH=True, borders_outerH=True, borders_innerV=True,
                   borders_outerV=True, context_menu_in_body=True, row_background=True,
                   policy=dpg.mvTable_SizingFixedFit, height=-1,
                   scrollY=True, id="table"):

        for i in range(columns):
            dpg.add_table_column(label=str(i))

        with dpg.clipper():
            for i in range(rows):
                dpg.push_container_stack(dpg.add_table_row())
                for j in range(columns):
                    dpg.add_checkbox(label=f"Cell {i}, {j}")
                dpg.pop_container_stack()

timer.mark("Time to create selectables:\t")
#dpg.show_metrics()
dpg.start_dearpygui()
