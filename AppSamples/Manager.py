from marvel import *
import marvel_constants as mc

add_button("Run App", callback="Launcher")
add_button("Run Demo", callback="Launcher")
add_button("Run Documentation", callback="Launcher")
add_button("Run IDE", callback="Launcher")

def Launcher(sender):

    if sender == "Run App":
        run_file("MarvelSandbox", "App.py")
        
    elif sender == "Run Demo":
        run_file("MarvelSandbox", "Demo.py")
        
    if sender == "Run Documentation":
        run_file("MarvelSandbox", "", flags="--documentation")
        
    if sender == "Run IDE":
        run_file("MarvelSandbox", "", flags="--editor")