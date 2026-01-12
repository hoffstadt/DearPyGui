# Takes ImGuiStyleVar declarations from imgui.h and outputs a set of corresponding DPG styles

from itertools import dropwhile, takewhile
from pathlib import Path
import re
from string import Template
from typing import Callable, List, Set, Tuple, Union


imgui_h = Path(__file__).parent.parent / "thirdparty" / "imgui" / "imgui.h"
implot_h = Path(__file__).parent.parent / "thirdparty" / "implot" / "implot.h"
imnodes_h = Path(__file__).parent.parent / "thirdparty" / "imnodes" / "imnodes.h"

# Returns a list of (name, comment) tuples
def read_constants(src_file: Path, enum_name: str) -> List[Tuple[str, str]]:
    prefix = enum_name + "_"
    end_marker = prefix + "COUNT"
    lines = src_file.read_text("utf-8").splitlines()
    lines = list(
        filter(lambda line: line.strip().startswith(prefix),
            takewhile(lambda line: not line.strip().startswith(end_marker),
                dropwhile(lambda line: not line.strip().startswith(prefix), lines)
            )
        )
    )

    declarationRe = re.compile(r"\s*" + prefix + r"(\w+)\s*(?:=\s*\w+)?,\s*(.*)")

    parsed = [declarationRe.match(l) for l in lines]
    return [(m[1], m[2]) for m in parsed if m is not None]


OLD_NODE_COLORS: Set[str] = {
    "NodeBackground",
    "NodeBackgroundHovered",
    "NodeBackgroundSelected",
    "NodeOutline",
    "TitleBar",
    "TitleBarHovered",
    "TitleBarSelected",
    "Link",
    "LinkHovered",
    "LinkSelected",
    "Pin",
    "PinHovered",
    "BoxSelector",
    "BoxSelectorOutline",
    "GridBackground",
    "GridLine",
}


def convert_constants(src_file: Path, dpg_prefix: Union[str, Callable], enum_name: str, comment_pos: int) -> List[str]:
    template = Template('\t\tModuleConstants.push_back({ "${dpg_prefix}_$name", ${imgui_prefix}_$name });')
    dpg_prefix_gen =  (lambda name: dpg_prefix) if isinstance(dpg_prefix, str) else dpg_prefix
    defs = read_constants(src_file, enum_name)
    return [
        # Regarding comment_pos: we need to subtract 1 to account for 1-based column number,
        # subtract 1 more to account for the space that we add before the comment,
        # and subtract 3 twice to account for two tab characters at the start of the template
        # (with tab width = 4, these take 4 characters on the screen but 1 character in actual text).
        (template.substitute(name=name, dpg_prefix = dpg_prefix_gen(name), imgui_prefix = enum_name).ljust(comment_pos - 8) + " " + comment).rstrip()
        for name, comment in defs
    ]


convert_constants(implot_h, "mvPlotCol", "ImPlotCol", 90)

lines = (
    convert_constants(imgui_h, "mvThemeCol", "ImGuiCol", 93) +
    [""] +
    convert_constants(implot_h, "mvPlotCol", "ImPlotCol", 90) +
    [""] +
    convert_constants(imnodes_h, (lambda name: "mvNodeCol" if name in OLD_NODE_COLORS else "mvNodesCol"), "ImNodesCol", 90) +
    [""] +
    convert_constants(imgui_h, "mvStyleVar", "ImGuiStyleVar", 109) +
    [""] +
    convert_constants(implot_h, "mvPlotStyleVar", "ImPlotStyleVar", 112) +
    [""] +
    convert_constants(imnodes_h, "mvNodesStyleVar", "ImNodesStyleVar", 109)
)

print("\n".join(lines))
