"""Build and run the real candlestick renderer without a window/GPU (Linux).

Requires a GNU-compatible C++17 compiler, Python development headers, and the
pinned imgui/implot submodules. No installed DearPyGui extension is needed.
Run: python3 testing/run_candlestick_precision.py
Set CXX and CXXFLAGS to select compiler/options (e.g. -O2 or sanitizers).
"""
import os
from pathlib import Path
import shlex
import subprocess
import sysconfig
import tempfile

root = Path(__file__).resolve().parents[1]
compiler = shlex.split(os.environ.get("CXX", "c++"))
flags = shlex.split(os.environ.get("CXXFLAGS", "-O0 -g"))
includes = [root / "src", sysconfig.get_path("include")]
includes += [root / "thirdparty" / name for name in
             ("imgui", "implot", "imnodes", "ImGuiFileDialog", "stb")]
sources = [root / "testing/candlestick_precision.cpp"]
sources += [root / "thirdparty/imgui" / (name + ".cpp") for name in
            ("imgui", "imgui_draw", "imgui_widgets", "imgui_tables")]
sources += [root / "thirdparty/implot" / (name + ".cpp") for name in
            ("implot", "implot_items")]
with tempfile.TemporaryDirectory(prefix="dpg-candlestick-") as temp:
    executable = Path(temp) / "candlestick_precision"
    command = compiler + ["-std=c++17", "-ffunction-sections", "-fdata-sections",
                          '-DIMGUI_USER_CONFIG="mvImGuiConfig_linux.h"']
    command += flags + ["-I" + str(path) for path in includes]
    command += [str(path) for path in sources]
    command += ["-Wl,--gc-sections", "-o", str(executable)]
    print(shlex.join(command), flush=True)
    subprocess.run(command, check=True)
    subprocess.run([str(executable)], check=True)
