from dearpygui.core import *
from dearpygui.simple import *

# callbacks
def ClearTable(sender, data):
    clear_table("Table##widget")

def DeleteRow(sender, data):
    delete_row("Table##widget", 6)

def DeleteCol(sender, data):
    delete_column("Table##widget", 1)

def AddRow(sender, data):
    add_row("Table##widget", ["new1", "new2", "new3", 53])

def AddCol(sender, data):
    add_column("Table##widget", "New Column", ["new1", "new2", "new3", "new4"])

def InsertRow(sender, data):
    insert_row("Table##widget", 5, ["inew1", "inew2", "inew3", "inew4"])

def InsertCol(sender, data):
    insert_column("Table##widget", 1, "Inserted Column", ["inew1", "inew2", "inew3", "inew4"])

# tables
add_button("Delete row 6", callback=DeleteRow)
add_button("Delete col 1", callback=DeleteCol)
add_button("Add row ", callback=AddRow)
add_button("Add col ", callback=AddCol)
add_button("Insert row 5", callback=InsertRow)
add_button("Insert col 1 ", callback=InsertCol)
add_button("Clear Table ", callback=ClearTable)
add_table("Table##widget", ["Column 1", "Column 2", "Column 3", "Column 4"])

tabledata = []
for i in range(0, 10):
    row = []
    for j in range(0, 4):
        row.append("Item" + str(i) + "-" + str(j))
    tabledata.append(row)

set_value("Table##widget", tabledata)

start_dearpygui()