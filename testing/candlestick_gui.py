"""Manual candlestick regression using an installed, locally built DearPyGui.

Run the same file with baseline and patched Python environments:
    /path/to/venv/bin/python -I testing/candlestick_gui.py --label BASELINE
    /path/to/other/venv/bin/python -I testing/candlestick_gui.py --label PATCHED

Baseline: minute A has oversized bodies; minute B has collapsed bodies.
Patched: both have half-width 15 seconds at the default weight of 0.25.
The remaining plots provide visual checks at longer intervals.
Close the window to exit. This is a manual example, not a pixel assertion test.
"""
import argparse

import dearpygui.dearpygui as dpg


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--label', default='Candlestick precision')
    args = parser.parse_args()
    dpg.create_context()
    try:
        axes = []
        series = [
            ('One minute A: first timestamp 1678089480', 1678089480, 60, dpg.mvTimeUnit_Min),
            ('One minute B: first timestamp 1678089540', 1678089540, 60, dpg.mvTimeUnit_Min),
            ('Five minutes', 1678089480, 300, dpg.mvTimeUnit_Min),
            ('Hourly', 1678089480, 3600, dpg.mvTimeUnit_Hr),
            ('Daily', 1678089480, 86400, dpg.mvTimeUnit_Day),
        ]
        with dpg.window(tag='main'):
            dpg.add_text(f'DearPyGui PR #2669 | {args.label}')
            dpg.add_text('Identical OHLC / automatic axis fitting / default weight 0.25')
            for label, start, interval, unit in series:
                with dpg.plot(label=label, height=166, width=-1, no_menus=True):
                    x = dpg.add_plot_axis(dpg.mvXAxis, scale=dpg.mvPlotScale_Time, auto_fit=True)
                    with dpg.plot_axis(dpg.mvYAxis, label='USD', auto_fit=True) as y:
                        dpg.add_candle_series(
                            [start + i * interval for i in range(4)],
                            [22000, 22070, 22020, 22090],
                            [22060, 22010, 22085, 22035],
                            [21970, 21990, 21995, 22010],
                            [22085, 22100, 22110, 22115],
                            time_unit=unit, tooltip=False,
                        )
                    axes.extend([x, y])
        dpg.create_viewport(title=f'DearPyGui PR2669 - {args.label}',
                            width=1160, height=940, x_pos=50, y_pos=40,
                            resizable=False, vsync=True)
        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.set_primary_window('main', True)
        frames = 0
        while dpg.is_dearpygui_running():
            dpg.render_dearpygui_frame()
            frames += 1
            if frames == 10:
                # Avoid manually setting epoch-scale limits: that API has a
                # separate precision issue. Fitting is identical for both builds.
                for axis in axes:
                    dpg.fit_axis_data(axis)
    finally:
        dpg.destroy_context()


if __name__ == '__main__':
    main()
