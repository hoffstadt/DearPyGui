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

# various widgets
sandbox.addSpacing("", 10)
sandbox.addButton("", "Press me")
sandbox.addInputText("", "Testing", hint="a hint")
sandbox.addGroup("", "Group1")
sandbox.addRadioButton("Group1", "radiobutton1", ("First Option", "Second Option", "Third Option"))
sandbox.endGroup("Group1")
sandbox.addSameLine("")
sandbox.addChild("", "Child1", 100, 200)
sandbox.addRadioButton("Child1", "radiobutton2", ("First Option", "Second Option", "Third Option"))
sandbox.endChild("Child1")
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
sandbox.setItemCallback("Press me", "ItemCallback")
sandbox.setItemCallback("Testing", "ItemCallback")
sandbox.setItemCallback("Tab1", "ItemCallback")
sandbox.setItemCallback("callback 1", "ItemCallback")
sandbox.setItemCallback("callback 2", "ItemCallback")
sandbox.setItemCallback("callback 3", "ItemCallback")

def ItemCallback(sender):
    print("Called by ", sender);
