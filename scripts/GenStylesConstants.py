# Takes ImGuiStyleVar declarations from imgui.h and outputs a set of corresponding DPG styles

from itertools import dropwhile, takewhile
from pathlib import Path
import re
from string import Template


imgui_h = Path(__file__).parent.parent / "thirdparty" / "imgui" / "imgui.h"
lines = imgui_h.read_text("utf-8").splitlines()
lines = list(
    filter(lambda line: line.strip().startswith("ImGuiStyleVar_"),
        takewhile(lambda line: not line.strip().startswith("ImGuiStyleVar_COUNT"),
            dropwhile(lambda line: not line.strip().startswith("ImGuiStyleVar_"), lines)
        )
    )
)

styleRe = re.compile(r"\s*ImGuiStyleVar_(\w+),\s*(.*)")

parsed = [styleRe.match(l) for l in lines]
styles = [(m[1], m[2]) for m in parsed if m is not None]
template = Template('\t\tModuleConstants.push_back({ "mvStyleVar_$name", ImGuiStyleVar_$name });')

# name_width = 2 * max(len(name) for name, comment in styles) + len(template.substitute(name=""))
# Overriding it so that there are fewer changes in dearpygui.cpp
name_width = 38 + len(template.substitute(name=""))

lines = [template.substitute(name=name).ljust(name_width) + " " + comment for name, comment in styles]

print("\n".join(lines))
