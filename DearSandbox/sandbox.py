import dearpygui._dearpygui as idpg
import dearpygui.dearpygui as dpg
dpg.set_init_file()

columns = 10
rows = 100000 
clipper = True
usePool = True
useStaging = False

if usePool:

    with dpg.item_pool():
        dpg.add_item_set(dpg.mvTableRow, rows)
        dpg.add_item_set(dpg.mvSelectable, rows*columns)


if useStaging:

    dpg.set_staging_mode(True)
    with dpg.staging_container(id="staged"):
        with dpg.clipper():
            for i in range(rows):
                with dpg.table_row(label=str(i)):
                    for j in range(columns):
                        dpg.add_selectable(label=f"Cell {i}, {j}")
    dpg.set_staging_mode(False)

with dpg.window(label="Profiling"):

    with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                   borders_innerH=True, borders_outerH=True, borders_innerV=True,
                   borders_outerV=True, context_menu_in_body=True, row_background=True,
                   policy=dpg.mvTable_SizingFixedFit, height=-1,
                   scrollY=True, id="table"):

        for i in range(columns):
            dpg.add_table_column(label=str(i))

        if clipper and useStaging:
            dpg.set_item_children("table", "staged", 1)

        elif clipper:
            with dpg.clipper():
                for i in range(rows):
                    dpg.push_container_stack(idpg.add_table_row())
                    for j in range(columns):
                        idpg.add_selectable(label=f"Cell {i}, {j}")
                    dpg.pop_container_stack()

        else:
            for i in range(0, rows):
                with dpg.table_row(label=str(i)):
                    for j in range(columns):
                        dpg.add_selectable(label=f"Cell {i}, {j}")

#dpg.show_metrics()
dpg.start_dearpygui()
