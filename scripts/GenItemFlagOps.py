# Takes a specific item's flags (a ImGuiSomethingFlags_ enum) from imgui.h and outputs
# boilerplate to copy-paste into handleSpecificKeywordArgs / getSpecificConfiguration.
# Run with --all to get updates for all known flags.
# Redirect output to a file for better reading / copy-pasting.

import argparse
import itertools
from pathlib import Path
import re
from string import Template
from typing import List, Set, Tuple


imgui_h = Path(__file__).parent.parent / "thirdparty" / "imgui" / "imgui.h"
src_path = Path(__file__).parent.parent / "src"
# implot_h = Path(__file__).parent.parent / "thirdparty" / "implot" / "implot.h"
# imnodes_h = Path(__file__).parent.parent / "thirdparty" / "imnodes" / "imnodes.h"


# We extract both set_configuration and handleSpecificKeywordArgs
dpg_func_re = re.compile(
    r"^(?:DearPyGui::set_configuration\(.*? (mv\w+?)Config&|"
    r"(?:void\s+)?(mv\w+)::handleSpecificKeywordArgs).*?\n\{(.*?)^\}",
    re.MULTILINE | re.DOTALL
)


def get_existing_flags(src_file: Path) -> List[Tuple[str, Set[str], str]]:
    text = src_file.read_text("utf-8")
    func_defs = dpg_func_re.findall(text)

    # We don't care what the destination flags variable is named
    declarationRe = re.compile(r'\s*flagop\("\w+",\s*(ImGui\w+),')

    def extract_flags(text: str) -> Set[str]:
        parsed = [declarationRe.match(l) for l in text.splitlines()]
        return set([m[1] for m in parsed if m is not None])

    def get_indentation(text: str) -> str:
        # Note: we can't use \s in the pattern because it also matches newlines
        m = re.search(r"^([ \t]*)flagop", text, re.MULTILINE)
        return "\t" if m is None else m[1]

    # The widget name is either in the group 1 or group 2, which are 0th and 1st elements
    # of the find_all result, respectively.
    return [ (d[0] or d[1], extract_flags(d[2]), get_indentation(d[2])) for d in func_defs ]


def gen_py_name(imgui_name: str) -> str:
    return re.sub("([^A-Z])([A-Z])", r"\1_\2", imgui_name).lower()
    # return re.sub("([A-Z])", r"_\1", imgui_name).lstrip("_").lower()


# Returns a list of (imgui_name, py_name, comment) tuples
def read_enum(src_file: Path, enum_name: str, existing_flags: Set[str]) -> List[Tuple[str, str, str]]:
    prefix = enum_name + "_"

    text = src_file.read_text("utf-8")
    m = re.search(r"^enum\s+" + prefix + r"\b.*?\n\{(.*?)^\};", text, re.MULTILINE | re.DOTALL)
    if m is None:
        print(f"Can't find enum {enum_name}.")
        return []

    text = m[1]
    text = re.sub(r"^#ifndef\s+IMGUI_DISABLE_OBSOLETE_FUNCTIONS.*?^#endif.*?\n", "", text, flags=re.MULTILINE | re.DOTALL)
    lines = text.splitlines()
    # drop all commented lines
    # lines = list(filter(lambda l: not l.lstrip().startswith("//"), lines))
    declarationRe = re.compile(r"\s*" + prefix + r"(\w+)\s*=\s*1\s*<<\s*\d+,\s*(?://\s*(.*))")
    parsed = [declarationRe.match(l) for l in lines]

    return [
        (prefix + m[1], gen_py_name(m[1]), m[2])
        for m in parsed
        if m is not None and (prefix + m[1]) not in existing_flags
    ]


def conv_set_config(flags: List[Tuple[str, str]], indent: str) -> List[str]:
    template = Template('flagop("$py_name", $imgui_name, outConfig.flags);')
    return [
        indent + template.substitute(py_name=py_name, imgui_name=imgui_name)
        for imgui_name, py_name, comment in flags
    ]


def conv_get_config(flags: List[Tuple[str, str]], indent: str) -> List[str]:
    template = Template('checkbitset("$py_name", $imgui_name, inConfig.flags);')
    return [
        indent + template.substitute(py_name=py_name, imgui_name=imgui_name)
        for imgui_name, py_name, comment in flags
    ]


def conv_parser(flags: List[Tuple[str, str]]) -> List[str]:
    template = Template('        args.push_back({ mvPyDataType::Bool, "$py_name", mvArgType::KEYWORD_ARG, "False", "$desc" });')
    return [
        template.substitute(py_name=py_name, desc=comment)
        for imgui_name, py_name, comment in flags
    ]


def process_enum(enum_name: str, existing_flags: Set[str], widget_name: str, indent: str) -> List[str]:
    enum_name = enum_name.rstrip("_")
    flags = read_enum(imgui_h, enum_name, existing_flags)
    if not flags:
        return []

    if widget_name:
        widget_name += ": "

    return (
        [
            f"=== {widget_name}{enum_name} ===",
            "",
            "Copy this to parser definitions in mvAppItem.cpp:",
            "",
        ] +
        conv_parser(flags) +
        [
            "",
            "Copy this to handleSpecificKeywordArgs() / DearPyGui::set_configuration():",
            "",
        ] +
        conv_set_config(flags, indent) +
        [
            "",
            "Copy this to getSpecificConfiguration() / DearPyGui::fill_configuration_dict():",
            "",
        ] +
        conv_get_config(flags, indent) +
        ["", ""]
    )


parser = argparse.ArgumentParser(description="Convert item flags into DPG boilerplate.")
parser.add_argument("enums", metavar="enum-name", nargs="*",
                    help="enum name, with or without the trailing underscore")
parser.add_argument("--all", action="store_true", help="refresh all known enums found in DPG source")

args = parser.parse_args()

# A list of (widget_name, imgui_flags, indentation) tuples
widget_defs = (
    get_existing_flags(src_path / "mvBasicWidgets.cpp") +
    get_existing_flags(src_path / "mvColors.cpp") +
    get_existing_flags(src_path / "mvContainers.cpp") +
    # get_existing_flags(src_path / "mvNodes.cpp") +
    # get_existing_flags(src_path / "mvPlotting.cpp") +
    get_existing_flags(src_path / "mvTables.cpp")
)

# This dict maps widget name to a list of ImGui flags
existing = {name: flags for name, flags, indentation in widget_defs}

# Just squashing all prefixes in each sub-list into a set.
# The resulting dict maps widget name to a set of ImGui enum prefixes, ideally just
# one enum per widget (but who knows).
existing_prefixes = {
    widget: set([ e.partition("_")[0] for e in flags ]) for widget, flags in existing.items()
}

# This will keep a (widget, enum) list, where widget may be empty e.g. if we only
# have an enum name but it's not used in any known widget
enums: List[Tuple[str, str]]

if args.all or not args.enums:
    # Here we get all combinations of (widget, prefix) pairs
    enums = sorted([
        (widget, prefix)
        for widget, prefixes in existing_prefixes.items()
        for prefix in prefixes
    ], key=lambda item: (item[1], item[0]))

else:
    # Here we're only given enum prefixes, let's find the corresponding widgets
    prefixes = [e.rstrip("_") for e in args.enums]
    # We only need this mapping to make sure we don't lose prefixes that are not
    # found anywhere in existing_prefixes (i.e. something new).  For these, we add
    # an empty widget name in their list so they are not lost.
    widgets_per_pref = {
        prefix: (sorted([w for w, p in existing_prefixes.items() if prefix in p]) or [""])
        for prefix in prefixes
    }
    # Now reverse the mapping
    enums = [
        (w, p)
        for p, widgets in widgets_per_pref.items()
        for w in widgets
    ]

# Double check if we might be missing some widgets

no_eyecatcher = [name for name, flags in existing.items() if not flags]
if no_eyecatcher:
    print(
        "=== No flags detected ===\n\n"
        "The following items seem to not have any flags (or, rather, flagop() calls)\n"
        "and are not included into the report below:\n")
    print("\n".join(no_eyecatcher) + "\n")

# Go compare the flags
widget_indents = {name: indentation for name, flags, indentation in widget_defs}

lines = itertools.chain.from_iterable(
    [
        process_enum(enum_name, existing[widget], widget, widget_indents[widget])
        for widget, enum_name in enums
    ]
)

print("\n".join(lines))
