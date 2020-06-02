import sandbox

# create some menus
sandbox.addMenuBar("MenuBar")
sandbox.addMenu("MenuBar", "File")
sandbox.addMenu("File", "Callbacks")
sandbox.addMenuItem("Callbacks", "callback 1")
sandbox.addMenuItem("Callbacks", "callback 2")
sandbox.addMenuItem("Callbacks", "callback 3")
sandbox.endMenu("Callbacks")
sandbox.endMenu("File")
sandbox.addMenu("MenuBar", "Edit")
sandbox.endMenu("Edit")
sandbox.endMenuBar("MenuBar")

sandbox.addSpacing("", 10)
sandbox.addInputText("", "Testing", hint="a hint")
sandbox.addSpacing("", 10)

# creating tabs
sandbox.addTabBar("", "TabBar1")
sandbox.addTab("TabBar1", "Tab1")
sandbox.addInputText("Tab1", "Testing1")
sandbox.addSameLine("Tab1")
sandbox.addInputText("Tab1", "Testing2")
sandbox.endTab("Tab1")
sandbox.addTab("TabBar1", "Tab2")
sandbox.addInputText("Tab2", "Testing3")
sandbox.addSpacing("Tab2", count=10)
sandbox.addInputText("Tab2", "Testing4")
sandbox.endTab("Tab2")
sandbox.endTabBar("TabBar1")

# setting call backs
sandbox.setItemCallback("Testing", "ItemCallback")
sandbox.setItemCallback("Tab1", "ItemCallback")
sandbox.setItemCallback("callback 1", "ItemCallback")
sandbox.setItemCallback("callback 2", "ItemCallback")
sandbox.setItemCallback("callback 3", "ItemCallback")

def ItemCallback(sender):
    print("Called by ", sender);
