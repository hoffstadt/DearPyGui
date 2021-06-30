import os
import dearpygui.dearpygui as dpg

include_directories = []

editor_id = dpg.generate_uuid()
headers_id = dpg.generate_uuid()
cpp_id = dpg.generate_uuid()
other_id = dpg.generate_uuid()
all_files_id = dpg.generate_uuid()
ignore_id = dpg.generate_uuid()
files_id = dpg.generate_uuid()
root_id = dpg.generate_uuid()


class IncludeNode:

    count = 0
    level = 0
    levels = {0: 0}
    root = ""

    def __init__(self, name, level):
        self.raw_name = name
        self.name = name
        self.links = []
        self.level = level
        IncludeNode.levels[level] = 0

        IncludeNode.count += 1

        found = False
        for directory in include_directories:
            if os.path.exists(directory + name):
                self.file_path = directory + name
                found = True
                break
        if not found:
            print("could not find file: ", name)

        self.parse_node()

    def parse_node(self):

        lines = []
        with open(self.file_path, "r") as file:
            lines = file.readlines()

        for line in lines:
            if line.__contains__('#include "'):
                split_line = line.split('"')
                if split_line[1] not in dpg.get_item_configuration(ignore_id)["items"]:
                    add_file_if_new(split_line[1])
                    self.links.append(IncludeNode(split_line[1], self.level+1))

    def start_tree(self):
        IncludeNode.root = self.raw_name
        self.create_ui_node(0)

    def create_ui_node(self, parent):

        y = IncludeNode.levels[self.level]
        IncludeNode.levels[self.level] += 80

        with dpg.node(label=self.name, pos=(250*self.level, y), parent=editor_id):
            if self.level > 0:
                with dpg.node_attribute(label=self.name + "-in") as node_from_id:
                    dpg.add_text("from")
            if len(self.links) > 0:
                with dpg.node_attribute(label=self.name + "-out", attribute_type=dpg.mvNode_Attr_Output) as node_out_id:
                    dpg.add_text("Includes:")

            if parent != 0:
                dpg.add_node_link(parent, node_from_id, parent=editor_id)

        for i in range(0, len(self.links)):
            self.links[i].create_ui_node(node_out_id)


def callback(sender):
    dpg.delete_item(editor_id, children_only=True)
    IncludeNode.count = 0
    IncludeNode.level = 0
    IncludeNode.levels = {0: 0}

    if sender is not None:
        selection = dpg.get_value(sender)
        file = selection
    else:
        file = IncludeNode.root

    inode = IncludeNode(file, 0)
    inode.start_tree()


def ignore():

    file = dpg.get_value(files_id)
    if file is not None:
        items = dpg.get_item_configuration(files_id)["items"]
        ignore_files = dpg.get_item_configuration(ignore_id)["items"]
        ignore_files.append(file)
        dpg.configure_item(ignore_id, items=ignore_files)
        index = -1
        found_index = 0
        for item in items:
            index += 1
            if item == file:
                found_index = index
                break
        del items[found_index]
        dpg.configure_item(files_id, items=items)
    callback(None)


def add_file_if_new(file):

    items = dpg.get_item_configuration(files_id)["items"]
    new_items = [file]
    for item in items:

        if item != file:
            new_items.append(item)

    dpg.configure_item(files_id, items=new_items)


def unignore():

    file = dpg.get_value(ignore_id)
    if file is not None:
        items = dpg.get_item_configuration(ignore_id)["items"]
        files = dpg.get_item_configuration(files_id)["items"]
        files.append(file)
        dpg.configure_item(files_id, items=files)
        index = -1
        found_index = 0
        for item in items:
            index += 1
            if item == file:
                found_index = index
                break
        del items[found_index]
        dpg.configure_item(ignore_id, items=items)
    callback(None)


def search_directory():

    header_files = []
    cpp_files = []
    other_files = []
    include_directories.clear()
    for dirName, subdirList, fileList in os.walk(dpg.get_value(root_id)):
        include_directories.append(dirName + "/")
        for fname in fileList:
            if fname.endswith(".h"):
                header_files.append(fname)
            elif fname.endswith(".cpp"):
                cpp_files.append(fname)
            else:
                other_files.append(fname)

    dpg.configure_item(headers_id, items=header_files)
    dpg.configure_item(cpp_id, items=cpp_files)
    dpg.configure_item(other_id, items=other_files)
    dpg.show_item(all_files_id)

#-----------------------------------------------------------------------------
# User Interface
#-----------------------------------------------------------------------------

# main window
with dpg.window(label="Main") as main_id:

    with dpg.menu_bar():
        with dpg.menu(label="Settings"):
            dpg.add_menu_item(label="List Files", callback=lambda: dpg.show_item(all_files_id))
            dpg.add_menu_item(label="Ignore Settings", callback=lambda: dpg.show_item(ignore_options_id))

        with dpg.menu(label="Tools"):
            dpg.add_menu_item(label="Show About", callback=lambda:dpg.show_tool(dpg.mvTool_About))
            dpg.add_menu_item(label="Show Metrics", callback=lambda:dpg.show_tool(dpg.mvTool_Metrics))
            dpg.add_menu_item(label="Show Documentation", callback=lambda:dpg.show_tool(dpg.mvTool_Doc))
            dpg.add_menu_item(label="Show Debug", callback=lambda:dpg.show_tool(dpg.mvTool_Debug))
            dpg.add_menu_item(label="Show Item Registry", callback=lambda:dpg.show_tool(dpg.mvTool_ItemRegistry))

    dpg.add_input_text(label="Root", default_value="C:\\dev\\DearPyGui\\DearPyGui\\", id=root_id)
    dpg.add_same_line()
    dpg.add_button(label="Initialize", callback=search_directory)

    dpg.add_node_editor(label="Editor", id=editor_id)

# files window
with dpg.window(label="All Files", pos=(75, 65), show=False, no_resize=True, autosize=True, id=all_files_id):

    dpg.add_listbox(label="Headers", width=200, num_items=15, callback=callback, id=headers_id)
    dpg.add_same_line()
    dpg.add_listbox(label="Cpp", width=200, num_items=15, callback=callback, id=cpp_id)
    dpg.add_same_line()
    dpg.add_listbox(label="Other", width=200, num_items=15, callback=callback, id=other_id)

# ignore options window
with dpg.window(label="Ignore Options", pos=(75, 400), show=False, no_resize=True, autosize=True) as ignore_options_id:

    with dpg.group():
        dpg.add_button(label="Ignore File", callback=ignore)
        dpg.add_same_line()
        dpg.add_button(label="Unignore File", callback=unignore)
        dpg.add_listbox(label="Ignore", width=200, num_items=15, id=ignore_id)
        dpg.add_same_line()
        dpg.add_listbox(label="Files", width=200, num_items=15, id=files_id)

dpg.setup_viewport()
dpg.set_viewport_title("Header Analyzer")
dpg.set_primary_window(main_id, True)
dpg.start_dearpygui()

