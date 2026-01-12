# Takes themes from from imgui_draw.cpp, converts them to DPG themes, and stores in themes.py

from pathlib import Path
import re
from string import Template
from typing import Any, Dict, Set, Tuple, Union

imgui_draw = Path(__file__).parent.parent.parent / "imgui" / "imgui_draw.cpp"
implot = Path(__file__).parent.parent.parent / "implot" / "implot.cpp"
imnodes = Path(__file__).parent.parent.parent / "imnodes" / "imnodes.cpp"
themes_py = Path(__file__).parent.parent / "dearpygui_ext" / "themes.py"

main_template = Template("""
from typing import List, Any, Callable, Union, Tuple
import dearpygui.dearpygui as dpg

$themes
""".lstrip())

theme_template = Template("""
def create_theme_imgui_$name() -> Union[str, int]:

    with dpg.theme() as theme_id:
        with dpg.theme_component(0):
$colors

    return theme_id
""".lstrip())


def parse_color(expr: str, colors: Dict[str, Any]) -> Tuple[Union[float, int], ...]:
    expr = re.sub(r"(\d+\.\d*)f", r"\1", expr)
    expr = expr.replace("dest->Colors", "colors")
    # Turn constants into quoted strings
    expr = re.sub(r"((?:ImGuiCol|ImPlotCol|ImNodesCol)_\w+)", r'"\1"', expr)
    # We could have defined a multiplication operator in Python but this would require
    # a class for color object where we can override __mul__... it's easier to store
    # colors as tuples.
    expr = re.sub(r"(.*?)\*(.*)", r"multiply(\1, \2)", expr)

    # Helper definitions.  Yep I know it would be better to use a safer method than exec()
    # but I needed to craft a script quickly, and I do trust Omar's C code :).
    def ImVec4(x, y, z, w):
        return (x, y, z, w)

    def IM_COL32(x, y, z, w):
        # Unlike ImVec4, this returns a tuple of int's... and it must never be used in ImLerp or multiplication
        return (x, y, z, w)

    def ImLerp(col_a, col_b, k):
        return tuple([(a + k*(b - a)) for a, b in zip(col_a, col_b)])

    def multiply(col_a, col_b):
        return tuple([(a * b) for a, b in zip(col_a, col_b)])

    IMPLOT_AUTO_COL = None

    return eval(expr, None, locals())


def convert_colors(source: str) -> Dict[str, Tuple[Union[float, int], ...]]:
    colors = { name: value for name, value in re.findall(r"(?:colors|dest->Colors)\[((?:ImGuiCol|ImPlotCol|ImNodesCol)_.*?)\]\s*=\s*(.*?);", source, re.DOTALL) }
    # Let's try to "compute" the values
    for name, value in colors.items():
        colors[name] = parse_color(value, colors)
    return colors


def format_color(color: Tuple[Union[float, int], ...]) -> str:
    return ", ".join([
        (f"{c:.2f} * 255" if isinstance(c, float) else str(c))
            for c in color
    ])


OLD_NODE_COLORS: Set[str] = {
    "mvNodesCol_NodeBackground",
    "mvNodesCol_NodeBackgroundHovered",
    "mvNodesCol_NodeBackgroundSelected",
    "mvNodesCol_NodeOutline",
    "mvNodesCol_TitleBar",
    "mvNodesCol_TitleBarHovered",
    "mvNodesCol_TitleBarSelected",
    "mvNodesCol_Link",
    "mvNodesCol_LinkHovered",
    "mvNodesCol_LinkSelected",
    "mvNodesCol_Pin",
    "mvNodesCol_PinHovered",
    "mvNodesCol_BoxSelector",
    "mvNodesCol_BoxSelectorOutline",
    "mvNodesCol_GridBackground",
    "mvNodesCol_GridLine",
}

def format_color_line(name: str, color: Tuple[Union[float, int], ...]) -> str:
    # See what category it is
    cat = (
        "Plots" if name.startswith("ImPlotCol") else
        "Nodes" if name.startswith("ImNodesCol") else
        ""
    )
    cat = f", category=dpg.mvThemeCat_{cat}" if cat else ""
    # Replace prefix
    name = name.replace("ImGuiCol_", "mvThemeCol_").replace("ImPlotCol_", "mvPlotCol_").replace("ImNodesCol_", "mvNodesCol_")
    if name in OLD_NODE_COLORS:
        name = name.replace("mvNodesCol", "mvNodeCol")

    return f"            dpg.add_theme_color(dpg.{name:34}, ({format_color(color)}){cat})"


def create_theme(theme_name: str, colors: Dict) -> str:
    color_lines = [format_color_line(name, value) for name, value in colors.items() if value is not None]
    return theme_template.substitute(name=theme_name.lower(), colors="\n".join(color_lines))


# Okay now we're ready to read and convert the sources!

source = imgui_draw.read_text("utf-8")
theme_funcs = re.findall(r"void\s+ImGui::StyleColors(\w+)(.*?)^\}", source, re.DOTALL|re.MULTILINE)
all_colors = {name.lower(): convert_colors(source) for name, source in theme_funcs}

source = implot.read_text("utf-8")
theme_funcs = re.findall(r"void\s+StyleColors(\w+)(.*?)^\}", source, re.DOTALL|re.MULTILINE)

for name, source in theme_funcs:
    name = name.lower()
    if name in all_colors:
        all_colors[name].update(convert_colors(source))

source = imnodes.read_text("utf-8")
theme_funcs = re.findall(r"void\s+StyleColors(\w+)(.*?)^\}", source, re.DOTALL|re.MULTILINE)

for name, source in theme_funcs:
    name = name.lower()
    if name in all_colors:
        all_colors[name].update(convert_colors(source))

dpg_themes = [create_theme(name, colors) for name, colors in all_colors.items()]
new_themes_contents = main_template.substitute(themes="\n".join(dpg_themes))

themes_py.write_text(new_themes_contents, "utf-8")

print("Done.")
