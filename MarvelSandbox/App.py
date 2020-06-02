import sandbox

sandbox.addInputText("", "Testing", hint="a hint")

sandbox.addTabBar("", "TabBar1")

sandbox.addTab("TabBar1", "Tab1")

sandbox.addInputText("Tab1", "Testing1")
sandbox.addInputText("Tab1", "Testing2")

sandbox.endTab("Tab1")

sandbox.addTab("TabBar1", "Tab2")

sandbox.addInputText("Tab2", "Testing3")
sandbox.addInputText("Tab2", "Testing4")

sandbox.endTab("Tab2")
sandbox.endTabBar("TabBar1")

sandbox.setItemCallback("Testing", "ItemCallback")
sandbox.setItemCallback("Tab1", "ItemCallback")

def ItemCallback(sender):
    print("Fuck yeah ", sender);
    print(sandbox.getValue(sender))
    print(sandbox.getValue("Tab1"))
    print(sandbox.getValue("Testing3"))