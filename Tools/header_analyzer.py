import os
from dearpygui.core import *
from dearpygui.simple import *

include_directories = []


class IncludeNode:

    count = 0
    level = 0
    levels = {0: 0}
    root = ""

    def __init__(self, name, level):
        self.raw_name = name
        self.name = name + "##" + str(IncludeNode.count)
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
                if split_line[1] not in get_item_configuration("Ignore")["items"]:
                    add_file_if_new(split_line[1])
                    self.links.append(IncludeNode(split_line[1], self.level+1))

    def start_tree(self):
        IncludeNode.root = self.raw_name
        self.create_ui_node(self.name)

    def create_ui_node(self, parent):

        y = IncludeNode.levels[self.level]
        IncludeNode.levels[self.level] += 80

        with node(self.name, x_pos=250*self.level, y_pos=y, parent="Editor"):
            if self.level > 0:
                with node_attribute(self.name + "-in"):
                    add_text("from")
            if len(self.links) > 0:
                with node_attribute(self.name + "-out", output=True):
                    add_text("Includes:")

        add_node_link("Editor", parent + "-out", self.name + "-in")

        for i in range(0, len(self.links)):
            self.links[i].create_ui_node(self.name)


def callback(sender):
    delete_item("Editor", children_only=True)
    IncludeNode.count = 0
    IncludeNode.level = 0
    IncludeNode.levels = {0: 0}

    if sender is not None:
        selection = get_value(sender)
        file = get_item_configuration(sender)["items"][selection]
    else:
        file = IncludeNode.root

    inode = IncludeNode(file, 0)
    inode.start_tree()


def ignore():

    file = get_value("Files")
    if file is not None:
        items = get_item_configuration("Files")["items"]
        ignore_files = get_item_configuration("Ignore")["items"]
        ignore_files.append(items[file])
        configure_item("Ignore", items=ignore_files)
        del items[file]
        configure_item("Files", items=items)
    callback(None)


def add_file_if_new(file):

    items = get_item_configuration("Files")["items"]
    new_items = [file]
    for item in items:
        if item != file:
            new_items.append(item)

    configure_item("Files", items=new_items)


def unignore():
    file = get_value("Ignore")
    if file is not None:
        items = get_item_configuration("Ignore")["items"]
        files = get_item_configuration("Files")["items"]
        files.append(items[file])
        configure_item("Files", items=files)
        del items[file]
        configure_item("Ignore", items=items)
    callback(None)


def search_directory():

    header_files = []
    cpp_files = []
    other_files = []
    include_directories.clear()
    for dirName, subdirList, fileList in os.walk(get_value("Root")):
        include_directories.append(dirName + "/")
        for fname in fileList:
            if fname.endswith(".h"):
                header_files.append(fname)
            elif fname.endswith(".cpp"):
                cpp_files.append(fname)
            else:
                other_files.append(fname)

    configure_item("Headers", items=header_files)
    configure_item("Cpp", items=cpp_files)
    configure_item("Other", items=other_files)
    show_item("All Files")


with window("All Files", x_pos=75, y_pos=65, show=False, no_resize=True, autosize=True):
    add_listbox("Headers", width=200, num_items=15, callback=callback)
    add_same_line()
    add_listbox("Cpp", width=200, num_items=15, callback=callback)
    add_same_line()
    add_listbox("Other", width=200, num_items=15, callback=callback)

with window("Ignore Options", x_pos=75, y_pos=400, show=False, no_resize=True, autosize=True):

    with group("g2"):
        add_button("Ignore File", callback=ignore)
        add_same_line()
        add_button("Unignore File", callback=unignore)
        add_listbox("Ignore", width=200, num_items=15)
        add_same_line()
        add_listbox("Files", width=200, num_items=15)

with window("Main"):

    with menu_bar("mb"):
        with menu("Settings"):
            add_menu_item("List Files", callback=lambda: show_item("All Files"))
            add_menu_item("Ignore Settings", callback=lambda: show_item("Ignore Options"))

        with menu("Tools"):
            add_menu_item("Show Logger", callback=show_logger)
            add_menu_item("Show About", callback=show_about)
            add_menu_item("Show Metrics", callback=show_metrics)
            add_menu_item("Show Documentation", callback=show_documentation)
            add_menu_item("Show Debug", callback=show_debug)
            add_menu_item("Show Style Editor", callback=show_style_editor)

    add_input_text("Root", default_value="C:\\dev\\DearPyGui\\DearPyGui\\")
    add_same_line()
    add_button("Initialize", callback=search_directory)

    with node_editor("Editor"):
        pass

set_main_window_title("Header Analyzer")
start_dearpygui(primary_window="Main")

