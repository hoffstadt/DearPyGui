from dearpygui.dearpygui import *

show_metrics()
show_logger()
set_log_level(0) # to show thread pool creation/destruction

# callbacks
def long_callback2(sender, data):
    for i in range(0, 50000000):
        pass
    log_info("Done with process from " + data)
    return 42

def return_from_long_callback(sender, data):
    log_info("Data returned to main thread: " + str(data))

def long_asyncronous_callback(sender, data):
    run_async_function(long_callback2, "some_data", return_handler=return_from_long_callback)

def long_callback(sender, data):
    for i in range(0, 50000000):
        pass
    log_info("Done with long process")

add_text("Tips")
add_text("Long operations can cause the GUI to freeze until the operation is complete.", bullet=True)
add_text("If you click 'Start Long Process', you should see the frame rate freeze in the metrics window.", bullet=True)
add_text("The solution is to run the long process asyncronously on a separate thread.", bullet=True)
add_text("If you click 'Start Long Asyncronous Process', you should see the frame rate should not be affected.", bullet=True)
add_text("To run an asyncronous function, use 'run_async_function'.", bullet=True)
add_text("Currently, dearpygui functions can't be ran inside the asyncronous function.", bullet=True)
add_text("If you need to return something from the asyncronous function, just return it like normal"
         " and use the 'return_handler' keyword. The return handler is another function that is called (on the main thread)"
         " when the asyncronous function is finished and the data argument is whatever the function "
         "returned.", wrap=500, bullet=True)


add_spacing()
add_separator()
add_spacing()

add_button("Start Long Process", callback=long_callback)
add_button("Start Long Asyncronous Process", callback=long_asyncronous_callback)

start_dearpygui()