import collections
import math
from struct import Struct
import struct
import sys
from collections import namedtuple

from dearpygui.core import *
from dearpygui.simple import *
from dataclasses import dataclass
from enum import IntFlag, auto, Flag, unique, Enum, IntEnum
from abc import ABC, abstractmethod
from typing import ClassVar

from DearSandbox.node_editor_helper import *


class Timer(Node):
    def __init__(self, ed: NodeEditor):
        super().__init__(ed, "Adder")

        self.max_t = InputAttribute(self, 'max_time', add_slider_int, sub_t=SubT.INT, v=1000)
        self.t = OutputAttribute(self, 'current_T', add_input_int, sub_t=SubT.INT, v=0, allow_multi=True)

    def compose_node(self):
        self.max_t.show(width=100, min_value=500, max_value=2000)
        self.t.show(width=100, readonly=True)

    def tick(self):
        if self.t.value >= self.max_t.value:
            self.t.value = 0
        self.t.value += 1
        self.t.propagate()


class AdderNode(Node):
    def __init__(self, ed: NodeEditor):
        super().__init__(ed, "SineWave")

        self.t = InputAttribute(self, 'time', add_slider_int, sub_t=SubT.INT, v=0)
        self.amp = InputAttribute(self, 'amplitude', add_slider_float, sub_t=SubT.FLT, v=10)
        self.freq = InputAttribute(self, 'frequency', add_slider_float, sub_t=SubT.FLT, v=2.5)
        self.signal = OutputAttribute(self, 'signal', add_input_float, sub_t=SubT.FLT, v=0)

    def compose_node(self):
        self.t.show(width=100)

        self.amp.show(width=100,
                      min_value=0,
                      max_value=35,
                      format="%.3f mA", )

        self.freq.show(width=100,
                       min_value=0.5,
                       max_value=8,
                       format="%.3f",
                       )
        self.signal.show(readonly=True)

    def tick(self):
        self.signal.value = self.amp.value / 2 * math.sin(2 * math.pi * self.freq.value * self.t.value / 1000)
        self.signal.propagate()

class StreamPlotter(Node):
    def __init__(self, ed: NodeEditor):
        super().__init__(ed, "StreamPlotter")

        self.x_vals = collections.deque(maxlen=500)
        self.y_vals = collections.deque(maxlen=500)
        self.x = InputAttribute(self, 'x', add_slider_int, sub_t=SubT.INT, v=0)
        self.y = InputAttribute(self, 'y', add_slider_float, sub_t=SubT.FLT, v=0)
        self.plt  = "Plot##"+str(self.node_id)

    def compose_node(self):
        self.x.show(width=100)
        self.y.show(width=100)

        add_plot(self.plt,width=400,height=200,no_child=True,yaxis_log_scale=True)
        set_plot_ylimits(self.plt,-100,100)


    def tick(self):
        self.x_vals.append(self.x.value)
        self.y_vals.append(self.y.value)
        add_line_series(self.plt,"Values",list(self.x_vals),list(self.y_vals))



with window(
        "Node Editor Demo", width=1300, height=900, no_scrollbar=True, no_resize=False
):
    add_text("DPG Node Editor Demo")
with window("sidebar", width=300, height=800, x_pos=30, y_pos=55, no_close=True):
    add_text("Hello")
with window("editor_panel", width=1000, height=800, x_pos=555, y_pos=55, no_close=True):
    editor = NodeEditor("ned")
    editor.show()
    editor.node(Timer)
    editor.node(AdderNode)
    editor.node(StreamPlotter)

    set_render_callback(editor.tick)
set_primary_window("Node Editor Demo", True)
set_main_window_size(1600, 900)
set_main_window_pos(50, 0)
start_dearpygui()
