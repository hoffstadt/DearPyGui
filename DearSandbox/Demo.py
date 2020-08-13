from dearpygui import *

show_debug()
show_logger()
add_menu_bar("MenuBar")
add_menu("FirstMenu")
add_menu_item("Create Second Menu", callback="addSecondMenu")
add_menu_item("Delete Second Menu", callback="deleteSecondMenu")
end_menu()
end_menu_bar()
add_button('Check is "SecondMenu" visible', callback="checkVisibility")

def addSecondMenu(sender, data):
    add_menu("SecondMenu", parent="MenuBar")
    end_menu()


def deleteSecondMenu(sender, data):
    delete_item("SecondMenu")


def checkVisibility(sender, data):
    log_debug(is_item_visible("SecondMenu"))


start_dearpygui()