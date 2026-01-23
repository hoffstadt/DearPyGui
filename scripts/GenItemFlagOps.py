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


def get_existing_flags(src_file: Path) -> List[str]:
    lines = src_file.read_text("utf-8").splitlines()
    # We don't care what the destination flags variable is named
    declarationRe = re.compile(r'\s*flagop\("\w+",\s*(ImGui\w+),')
    parsed = [declarationRe.match(l) for l in lines]
    return [m[1] for m in parsed if m is not None]


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


def conv_set_config(flags: List[Tuple[str, str]]) -> List[str]:
    template = Template('    flagop("$py_name", $imgui_name, outConfig.flags);')
    return [
        template.substitute(py_name=py_name, imgui_name=imgui_name)
        for imgui_name, py_name, comment in flags
    ]


def conv_get_config(flags: List[Tuple[str, str]]) -> List[str]:
    template = Template('    checkbitset("$py_name", $imgui_name, inConfig.flags);')
    return [
        template.substitute(py_name=py_name, imgui_name=imgui_name)
        for imgui_name, py_name, comment in flags
    ]


def conv_parser(flags: List[Tuple[str, str]]) -> List[str]:
    template = Template('        args.push_back({ mvPyDataType::Bool, "$py_name", mvArgType::KEYWORD_ARG, "False", "$desc" });')
    return [
        template.substitute(py_name=py_name, desc=comment)
        for imgui_name, py_name, comment in flags
    ]


def process_enum(enum_name: str, existing_flags: Set[str]) -> List[str]:
    enum_name = enum_name.rstrip("_")
    flags = read_enum(imgui_h, enum_name, existing_flags)
    if not flags:
        return []

    return (
        [
            f"=== {enum_name} ===",
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
        conv_set_config(flags) +
        [
            "",
            "Copy this to getSpecificConfiguration() / DearPyGui::fill_configuration_dict():",
            "",
        ] +
        conv_get_config(flags) +
        ["", ""]
    )


parser = argparse.ArgumentParser(description="Convert item flags into DPG boilerplate.")
parser.add_argument("enums", metavar="enum-name", nargs="*",
                    help="enum name, with or without the trailing underscore")
parser.add_argument("--all", action="store_true", help="refresh all known enums found in DPG source")

args = parser.parse_args()

enums = args.enums

existing = set(
    get_existing_flags(src_path / "mvBasicWidgets.cpp") +
    get_existing_flags(src_path / "mvColors.cpp") +
    get_existing_flags(src_path / "mvContainers.cpp") +
    # get_existing_flags(src_path / "mvNodes.cpp") +
    # get_existing_flags(src_path / "mvPlotting.cpp") +
    get_existing_flags(src_path / "mvTables.cpp")
)

if args.all:
    enums = sorted(list(set([e.partition("_")[0] for e in existing])))

lines = itertools.chain.from_iterable([process_enum(e, existing) for e in enums])

print("\n".join(lines))
