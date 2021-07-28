from dearpygui.dearpygui import *

def test():
    print('visible')

with window(label="Test",id =999,  width=400, show=True):
    add_visible_handler(parent=999, callback=test)

start_dearpygui()