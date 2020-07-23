from marvel import *

show_logger()
show_debug()

add_button("click me 9 times in row for crash", callback = 'runFun')

def runFun(Sender, data):
    display_warning_message("this is my love letter to you")

def display_warning_message(message):
    add_window("Warning Message",300, 100, autosize = True, title_bar = False)
    add_text(message)
    add_button("OK", callback = "deleteMessageWindow")

def deleteMessageWindow(sender, data):
    delete_item("Warning Message")

start_marvel()