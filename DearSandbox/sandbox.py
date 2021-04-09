from dearpygui.core import *
from dearpygui.simple import *

def printvalue(sender,data):
    print("calling back if the input value is changed")



with window("test"):
    add_input_int2('start',
                   callback=printvalue
                   )
    add_input_float2('float',
                     callback=printvalue)
show_logger()
print(get_dearpygui_version())
start_dearpygui()