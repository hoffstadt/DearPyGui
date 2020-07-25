from dearpygui import *
from gaugeworx import *
import sys

set_thread_count(4)
set_log_level(0)
show_logger()
show_metrics()

add_button("Trigger", callback="manager_command_launcher")
add_button("Prepare", callback="manager_command_launcher")
add_button("Inspect", callback="inspect_command_launcher")
add_button("Custom Inspect", callback="manager_command_launcher")
add_button("Review", callback="manager_command_launcher")
add_button("Results", callback="results_callback")
add_button("Ref", callback="ref_callback")

def ref_callback(sender, data):
    print(sys.getrefcount(sender))
    print(sys.getrefcount(data))
    print(sys.gettotalrefcount())

def manager_command_launcher(sender, data):
    run_async_function("manager_command", sender, return_handler="manager_command_return")

def inspect_command_launcher(sender, data):
    run_async_function("manager_command", sender)

def manager_command(sender, data):

    if data == "Trigger":
        reset()
        delete_gauges()
        delete_toleranced_items()

        add_preparing_item("Data Mover", "Data Mover")
        add_preparing_item("Run Out Reducer", "Run Out Reducer")

        add_ball_gauge("Root", 0.002, 0.0001, "RootProcedure")
        add_ball_gauge("Crest", 0.002, 0.0001, "CrestProcedure")
        add_ball_gauge("Taper", 0.002, 0.0001, "TaperProcedure")
        add_ball_gauge("Lead", 0.005, 0.0001, "LeadProcedure")
        add_ball_gauge("Height", 0.005, 0.0001, "HeightProcedure")

        add_toleranced_item("Taper", 2.0, 0.057, 0.057)
        add_toleranced_item("ThreadPitch", 0.125, 0.002, 0.002, True)
        add_toleranced_item("ThreadHeight", 0.03, 0.0005, 0.0005)
        add_toleranced_item("PitchDiameter", 0.0184, 0.0005, 0.0005)
        add_toleranced_item("Weiner", 0.0184, 0.0005, 0.0005)

        trigger_collector()

    elif data == "Prepare":
        prepare_data()

    elif data == "Inspect":
        inspect_data()

    elif data == "Review":
        review_data()

    elif data == "Custom Inspect":
        add_measurement("Weiner", 42, 0)
        add_measurement("Weiner", 43, 1)
        add_measurement("Weiner", 44, 2)

    return data

def manager_command_return(sender, data):
    print("Returned from ", data)

def results_callback(sender, data):

    print(get_measurement("Taper", 2))
    report = get_final_report()

    for item in report:
        print(item + ":\t" + str(report[item]))
        
start_dearpygui()