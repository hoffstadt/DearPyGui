import dearpygui.dearpygui as dpg
import time

dpg.create_context()
dpg.create_viewport()
dpg.configure_app(docking=True, docking_space=True, init_file="settings.ini")
dpg.setup_dearpygui()


class Mtd_Log_View():

    def __init__(self, show: bool = True, level: int = 0, max_line: int = 10000):
        """__init__ AI is creating summary for __init__

        [extended_summary]

        Args:
            show (bool, optional): [Window show]. Defaults to False.
            level (int, optional): [Verbose level]. Defaults to 0.
            max_line (int, optional): [Define the number of line in the log view]. Defaults to 10000.
        """

        self.log_level = level
        self._auto_scroll = True
        self.show = show

        self.window_id = dpg.add_window(label="Log View",
                                        pos=(0, 0),
                                        width=1280,
                                        height=720,
                                        tag="Log_View",
                                        show=self.show)

        self.count = 0

        self.error_count = 0
        self.warning_count = 0
        self.info_count = 0
        self.trace_count = 0
        self.debug_count = 0

        self.flush_count = max_line
        self._filter_table_id = dpg.generate_uuid()

        with dpg.menu_bar(parent=self.window_id):
            with dpg.menu(label="File"):
                dpg.add_menu_item(label="Export Log", enabled=False)
                dpg.add_separator()
                dpg.add_menu_item(label="Clear Log", callback=self.clear_log)
            with dpg.menu(label="Option"):
                dpg.add_menu_item(label="Test Entry", callback=self.test_log)

        with dpg.group(horizontal=True, parent=self.window_id):
            dpg.add_button(label=f"Error ({self.error_count})", tag="ErrorBtn")
            dpg.add_button(label=f"Warning ({self.warning_count})", tag="WarningBtn")
            dpg.add_button(label=f"Info ({self.info_count})", tag="InfoBtn")
            dpg.add_button(label=f"Trace ({self.trace_count})", tag="TraceBtn")
            dpg.add_button(label=f"Debug ({self.debug_count})", tag="DebugBtn")

            dpg.add_input_text(label="Filter (Inc, -Exc)",
                               width=-150,
                               user_data=self._filter_table_id,
                               callback=lambda s, a, u: dpg.set_value(u, dpg.get_value(s)))

        self.table_id = dpg.add_table(header_row=True,
                                      row_background=True,
                                      borders_innerH=True,
                                      borders_outerH=True,
                                      borders_innerV=True,
                                      borders_outerV=True,
                                      resizable=True,
                                      delay_search=True,
                                      hideable=True,
                                      reorderable=True,
                                      freeze_columns=1,
                                      height=-1,
                                      scrollY=True,
                                      policy=dpg.mvTable_SizingFixedFit,
                                      parent=self.window_id)

        dpg.add_table_column(label="", width=-100, parent=self.table_id)
        dpg.add_table_column(label="No", width=-100, parent=self.table_id)
        dpg.add_table_column(label="Time", width=-100, parent=self.table_id)
        dpg.add_table_column(label="Code", width=-100, parent=self.table_id)
        dpg.add_table_column(label="Description", width=-100, parent=self.table_id)
        dpg.add_table_column(label="Status", width=-100, parent=self.table_id)

        with dpg.theme() as self.code_theme:
            with dpg.theme_component(0):
                dpg.add_theme_color(dpg.mvThemeCol_Text, (64, 128, 255, 255))

        with dpg.theme() as self.trace_theme:
            with dpg.theme_component(0):
                dpg.add_theme_color(dpg.mvThemeCol_Text, (0, 255, 114, 255))

        with dpg.theme() as self.debug_theme:
            with dpg.theme_component(0):
                dpg.add_theme_color(dpg.mvThemeCol_Text, (64, 255, 240, 255))

        with dpg.theme() as self.info_theme:
            with dpg.theme_component(0):
                dpg.add_theme_color(dpg.mvThemeCol_Text, (255, 255, 255, 255))

        with dpg.theme() as self.warning_theme:
            with dpg.theme_component(0):
                dpg.add_theme_color(dpg.mvThemeCol_Text, (255, 210, 0, 255))

        with dpg.theme() as self.error_theme:
            with dpg.theme_component(0):
                dpg.add_theme_color(dpg.mvThemeCol_Text, (255, 0, 0, 255))

    def auto_scroll(self, value):
        self._auto_scroll = value

    def _log(self, message, level):

        if level < self.log_level:
            return

        self.count += 1

        if self.count > self.flush_count:
            self.clear_log()

        theme = self.info_theme
        message = message

        elapsed = time.time()
        times = time.strftime("%H:%M:%S", time.gmtime(elapsed))

        if level == 0:
            status = "Trace"
            theme = self.trace_theme
        elif level == 1:
            status = "Debug"
            theme = self.debug_theme
        elif level == 2:
            status = "Info"
            theme = self.info_theme
        elif level == 3:
            status = "Warning"
            theme = self.warning_theme
        elif level == 4:
            status = "Error"
            theme = self.error_theme

        with dpg.table_row(filter_key=message, parent=self.table_id):

            dpg.add_text("")

            new_number = dpg.generate_uuid()
            dpg.add_text(self.count, tag=new_number)
            dpg.bind_item_theme(new_number, self.info_theme)

            new_time = dpg.generate_uuid()
            dpg.add_text(times, tag=new_time)
            dpg.bind_item_theme(new_time, self.info_theme)

            new_code = dpg.generate_uuid()
            dpg.add_text("", tag=new_code)
            dpg.bind_item_theme(new_code, self.info_theme)

            new_message = dpg.generate_uuid()
            dpg.add_text(message, tag=new_message)
            dpg.bind_item_theme(new_message, theme)

            new_status = dpg.generate_uuid()
            dpg.add_text(status, tag=new_status)
            dpg.bind_item_theme(new_status, theme)

    def log_trace(self, message):
        self._log(message, 0)
        self.trace_count += 1
        dpg.set_item_label("TraceBtn", label=f"Trace ({self.trace_count})")

    def log_debug(self, message):
        self._log(message, 1)
        self.debug_count += 1
        dpg.set_item_label("DebugBtn", label=f"Debug ({self.debug_count})")

    def log_info(self, message):
        self._log(message, 2)
        self.info_count += 1
        dpg.set_item_label("InfoBtn", label=f"Info ({self.info_count})")

    def log_warning(self, message):
        self._log(message, 3)
        self.warning_count += 1
        dpg.set_item_label("WarningBtn", label=f"Warning ({self.warning_count})")

    def log_error(self, message):
        self._log(message, 4)
        self.error_count += 1
        dpg.set_item_label("ErrorBtn", label=f"Error ({self.error_count})")

    def clear_log(self):
        dpg.delete_item(self.table_id, children_only=True, slot=1)

        self.count = 0

        self.error_count = 0
        dpg.set_item_label("ErrorBtn", label=f"Error ({self.error_count})")
        self.warning_count = 0
        dpg.set_item_label("WarningBtn", label=f"Warning ({self.warning_count})")
        self.info_count = 0
        dpg.set_item_label("InfoBtn", label=f"Info ({self.info_count})")
        self.trace_count = 0
        dpg.set_item_label("TraceBtn", label=f"Trace ({self.trace_count})")
        self.debug_count = 0
        dpg.set_item_label("DebugBtn", label=f"Debug ({self.debug_count})")

    def test_log(self):
        for i in range(0, 100):
            self.log_trace(f"trace message {i}")
            self.log_debug(f"debug message {i}")
            self.log_info(f"info message {i}")
            self.log_warning(f"warning message {i}")
            self.log_error(f"error message {i}")


log_view = Mtd_Log_View()

# main loop
dpg.show_viewport()
dpg.start_dearpygui()

dpg.destroy_context()
