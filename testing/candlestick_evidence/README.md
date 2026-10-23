# macOS candlestick validation

These unmodified window captures were taken on ARM64 macOS 26.6.2 with
Python 3.14.5, using separately compiled extensions and the pinned
ImGui/ImPlot/GLFW dependencies through the GLFW/Metal backend.

- `baseline.png`: commit `691d21713ca569a380cd0c434a3dd21eed85bf71`.
- `patched.png`: commit `ec16ad4de1cd88987f8316c8f8fdb3b0b94c55b2`.

Both captures used the same input script, 1160x940 viewport, default theme,
OHLC data, default candle weight 0.25, and automatic time-axis fitting.
The original capture script also recorded import paths and binary hashes;
`../candlestick_gui.py` retains its plot data and rendering settings while
removing machine-specific evidence collection. It was subsequently run with
both installed builds. Neither script manually sets epoch axis limits.

Minute A has oversized bodies on baseline; minute B has collapsed bodies.
Both have visible, correctly sized bodies after the patch. Five-minute, hourly,
and daily checks show no obvious regressions. Automatic fitting clips the
outer candle bodies at the plot boundaries in both builds; compare the
interior candles to inspect complete bodies.

All nine existing `simple_tests.py` tests passed with each installed build.
These are general API tests; the GUI example is a manual visual check.
This does not establish Windows, Intel macOS, sanitizer, or other Python
version coverage, nor make the standalone Linux geometry harness portable.
