import dearpygui.core as dpg
import dearpygui.contexts as cxt


class mvLogger:

    def __init__(self):
        
        self.log_level = 0
        self._auto_scroll = True
        self.filter_id = None
        self.window_id = dpg.add_window(label="mvLogger")

        with cxt.group(horizontal=True, parent=self.window_id):
            dpg.add_checkbox(label="Auto-scroll", default_value=True, callback=lambda sender:self.auto_scroll(dpg.get_value(sender)))
            dpg.add_button(label="Clear", callback=lambda: dpg.delete_item(self.filter_id, children_only=True))

        dpg.add_input_text(label="Filter", callback=lambda sender: dpg.set_value(self.filter_id, dpg.get_value(sender)), 
                    parent=self.window_id)
        self.child_id = dpg.add_child(parent=self.window_id, autosize_x=True, autosize_y=True)
        self.filter_id = dpg.add_filter_set(parent=self.child_id)

    def auto_scroll(self, value):
        self._auto_scroll = value

    def _log(self, message, level):

        if level < self.log_level:
            return

        color = [255, 255, 255, 255]

        if level == 0:
            message = "[TRACE]\t\t" + message
            color = [0, 255, 0, 255]
        elif level == 1:
            message = "[DEBUG]\t\t" + message
            color = [64, 128, 255, 255]
        elif level == 2:
            message = "[INFO]\t\t" + message
        elif level == 3:
            message = "[WARNING]\t\t" + message
            color = [255, 255, 0, 255]
        elif level == 4:
            message = "[ERROR]\t\t" + message
            color = [255, 0, 0, 255]
        elif level == 5:
            message = "[CRITICAL]\t\t" + message
            color = [255, 0, 0, 255]

        new_log = dpg.add_text(message, parent=self.filter_id, filter_key=message)
        dpg.set_theme_color(dpg.mvThemeCol_Text_Text, color, item=new_log)
        if self._auto_scroll:
            scroll_max = dpg.get_y_scroll_max(self.child_id)
            dpg.set_y_scroll(self.child_id, -1.0)

    def log(self, message):
        self._log(message, 0)

    def log_debug(self, message):
        self._log(message, 1)

    def log_info(self, message):
        self._log(message, 2)

    def log_warning(self, message):
        self._log(message, 3)

    def log_error(self, message):
        self._log(message, 4)

    def log_critical(self, message):
        self._log(message, 5)

    def clear_log(self):
        pass
