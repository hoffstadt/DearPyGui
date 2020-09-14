from dearpygui.dearpygui import *

# callbacks
def hideMenu(sender, data):
    hide_item("Tools")

def showMenu(sender, data):
    show_item("Tools")

def changeCallback(sender, data):
    callbackName=get_item_callback("Show Docs")
    print(callbackName)
    if callbackName == "showDocs":
        set_item_callback("Show Docs", "showMetrics")
    else:
        set_item_callback("Show Docs", "showDocs")

def showDocs(sender, data):
    show_documentation()

def showMetrics(sender, data):
    show_metrics()

def addThemes(sender, data):
    add_menu("Themes", parent="MenuBar")
    end()
    add_color_picker4("Color Selector", data_source="color1", parent="Themes")
    add_color_edit4("Color Item", data_source="color1")
    show_item("Delete Themes")
    hide_item("Add Themes")

def deleteThemes(sender, data):
    delete_item("Themes")
    delete_item("Color Item")
    show_item("Add Themes")
    hide_item("Delete Themes")

add_menu_bar("MenuBar")

add_menu("Show/Hide")
add_menu_item("Show Tools", callback=showMenu)
add_menu_item("Hide Tools", callback=hideMenu)
add_menu_item('Change "Show Docs" Callback', callback=changeCallback)
add_tooltip('Change "Show Docs" Callback', "tooltip1")
add_text('this will change the "show Docs" callback to a show metrics callback')
end_tooltip()
add_menu("Empty Menu")
add_menu_item("Nothing")
end()
end()

add_menu("Tools")
add_menu_item("Show Docs", callback=showDocs)
end()
add_menu("Add/Remove")
add_menu_item("Add Themes", callback=addThemes)
add_menu_item("Delete Themes", callback=deleteThemes)
hide_item("Delete Themes")
end()

end()

add_text("This menu bar demonstrates:")
add_text('standard menu bar, menus, and menu items', bullet=True)
add_text('adding menus to menus', bullet=True)
add_text('showing and hiding the "Tools menu"', bullet=True)
add_text("changing the callback of an already existing menu item", bullet=True)
add_text("adding and deleting menus, menu items, app widgets from a menu item", bullet=True)
add_text("placing a widget into the menu that controlling another widget on the body of the app", bullet=True)
add_spacing(count = 50)

start_dearpygui()
