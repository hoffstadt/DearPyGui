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

Naddr = namedtuple('Naddr', 'base,sub,id,id1')


class Nat(IntEnum):
    NONE = 0
    ADDER = auto()
    FLT = auto()
    NPY = auto()
    INT = auto()
    LST_INT = auto()
    LST_FLT = auto()
    STR = auto()
    MAX = 255


class Nef(IntFlag):
    ATTR = auto()
    NODE = auto()
    MULTI = auto()
    LINK = auto()
    INPUT = auto()
    OUTPUT = auto()
    AGG = auto()
    VIZ = auto()
    NONE = 0


# print(x)
# print(int(x))

class NodeEditor:
    def __init__(self):
        self.tick_list = []

    def add_node(self, node):
        if issubclass(node, Node):
            node(self)
    # def issue_id(self,nid):


class Node(ABC):
    @abstractmethod
    def __init__(self, ed):
        self.ed = ed
        # self._id_base =
        ...

    @abstractmethod
    def register(self):
        ...

    @abstractmethod
    def describe(self):
        ...


class MyNode(Node):

    def __init__(self, ed):
        super().__init__(ed)
        print(self.id)

    def register(self):
        print("bye!")
        pass


x = struct.pack('BBBB', 234, 25, 66, 46)
print(x)


def oink(a, b, c, d):
    print(a, b, c, d)


y = struct.unpack('BBBB', x)
# oink(*y)
# y = struct.unpack('i', x)
print(y)
# q = int.to_bytes(y[0],4,sys.byteorder)
#
z = struct.unpack('i', x)[0]


# print(z)

# H = Struct('BBBB')
# print(H.pack(234, 25, 66, 46))
#

@dataclass
class NedID:
    base: Nef
    sub: Nat
    nid: int = 0
    aid: int = 0
    _byte: ClassVar[Struct] = Struct('BBBB')
    _int: ClassVar[Struct] = Struct('i')
    _propmask: ClassVar[int] = 65535
    _atmask: ClassVar[int] = 16777215

    @classmethod
    def from_int(cls, i):
        r = cls._byte.unpack(cls._int.pack(i))
        return cls(Nef(r[0]), Nat(r[1]), r[2], r[3])

    def __int__(self):
        return self._int.unpack(self._byte.pack(int(self.base), int(self.sub), self.nid, self.aid))[0]

    def get_byte_arr(self):
        return self._byte.pack(int(self.base),int(self.sub),self.nid,self.aid)

    def get_key(self):
        return str(self.base)+str(self.sub)

    def same_props_as(self,ned_id):
        return bool((int(self) & self._propmask) == (int(ned_id)& self._propmask))

# bean = NedID(base=Nef.LINK|Nef.OUTPUT, sub=Nat.FLT, nid=2, aid=3)
# bean2 = NedID(base=Nef.LINK|Nef.OUTPUT, sub=Nat.FLT, nid=5, aid=4)
# print(bean)
# qq = int(bean)
# q2 = int(bean2)
# print(qq)
# print(qq>>16 & qq)
# print(bean.same_props_as(bean2))
# mask = NedID(base=Nef(255),sub=Nat(255),nid=255,aid=0)
# zz = (int(mask))
# print(zz)
# print(NedID.from_int(qq&~zz))

# x &= ~0xf
# print(x)
#
#
# x = T.FLT | T.IN
# print(x)
# print(int(x))
# y = T.INT | T.OUT
# print(x & y)
#
# class Nat:
#
# def nat_wrap(func,):
#     def inner1(*args, **kwargs):
#
#         print("before Execution")
#
#         # getting the returned value
#         returned_value = func(*args, **kwargs)
#         print("after Execution")
#
#         # returning the value to the original frame
#         return returned_value
#
#     return inner1
#


# How can I get the individual flags back?
# flags = [flag for flag in combined]
#
#
# @dataclass
# class Param:
#     id: str
#     val: any
#     low: any
#     high: any
#     fmt: str
#     lbl: str
#
# @dataclass
# class FParam(Param):
#     id: str


# @dataclass
# class FloatParam:
#     id: str = "float"
#     val: float = 0.0
#     low: float = -1.0
#     high: float = 1.0
#     fmt: str = "%.3f"
#     lbl: str = id
#
#
# @dataclass
# class IntParam:
#     id: str = "int"
#     val: float = 0
#     low: float = -10
#     high: float = 10
#     fmt: str = "%i"
#     lbl: str = id
#
#
# class RGBNode:
#     def __init__(self, nid, ed):
#         self.ed = ed
#         self.nid = nid
#         self.vmap = {}
#
#     def n(self):
#         self.nid += 1
#         return self.nid
#
#     def na(self, l):
#         n = self.n()
#         return l + "##" + str(n), n
#
#     def cb_manual(self, sender, data):
#         self.vmap[sender] = get_value(sender)
#         print(self.vmap)
#
#     def show(self):
#         with node(self.nid, x_pos=50, y_pos=50, label="ok", width=100, parent=self.ed):
#             x = self.na("R")
#             with node_attribute(x[1], 0, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#             x = self.na("G")
#             with node_attribute(x[1], 0, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#             x = self.na("B")
#             with node_attribute(x[1], 0, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#             x = self.na("A")
#             with node_attribute(x[1], 0, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#
#
# class RGBNode2:
#     def __init__(self, nid, ed):
#         self.ed = ed
#         self.nid = nid
#         self.vmap = {}
#
#     def n(self):
#         self.nid += 1
#         return self.nid
#
#     def na(self, l):
#         n = self.n()
#         return l + "##" + str(n), n
#
#     def cb_manual(self, sender, data):
#         self.vmap[sender] = get_value(sender)
#         print(self.vmap)
#
#     def show(self):
#         with node(self.nid, x_pos=50, y_pos=50, label="ok", width=100, parent=self.ed):
#             x = self.na("R")
#             with node_attribute(x[1], 1, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#             x = self.na("G")
#             with node_attribute(x[1], 1, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#             x = self.na("B")
#             with node_attribute(x[1], 1, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#             x = self.na("A")
#             with node_attribute(x[1], 1, pin_shape=mvNodePinShape_CircleFilled, target=x[0]):
#                 add_drag_float(x[0], default_value=0, max_value=255, min_value=0, callback=self.cb_manual)
#
#
# class Ned:
#     def __init__(self):
#         self.node_idx = 1000
#         self.link_idx = 0
#         self.name = "editor"
#         self.at_map = {}
#         self.lmap = {}
#
#     def cb_link_destroyed(self, sender, data):
#         print(data)
#
#     def cb_link_created(self, sender, data):
#         self.link_idx += 1
#         print(data)
#         if add_link(sender, obj_id=self.link_idx, start_attribute=data[0], end_attribute=data[1]):
#             if not data[0] in self.at_map:
#                 self.at_map[data[0]] = []
#
#             self.at_map[data[0]].append(data[1])
#             self.lmap[self.link_idx] = data
#             print(self.lmap, self.at_map)
#
#     def add_RGB_node(self, sender, data):
#         close_popup("ctx")
#         self.node_idx += 100
#         n = RGBNode(self.node_idx, self.name)
#         n.show()
#
#     def add_RGB_node2(self, sender, data):
#         close_popup("ctx")
#         self.node_idx += 100
#         n = RGBNode2(self.node_idx, self.name)
#         n.show()
#
#     def show(self):
#         with node_editor(self.name, callback_link_created=self.cb_link_created):
#             add_popup(self.name, "ctx")
#             add_button("RGB Node", callback=self.add_RGB_node)
#             add_button("RGB Node2", callback=self.add_RGB_node2)
#             end()
#
#
# with window("Node Editor Demo", width=1300, height=900, no_scrollbar=True, no_resize=False):
#     add_text("DPG Node Editor Demo")
# with window("sidebar", width=300, height=800, x_pos=30, y_pos=55, no_close=True):
#     add_text("Hello")
# with window("editor_panel", width=1000, height=800, x_pos=555, y_pos=55, no_close=True):
#     n = Ned()
#     n.show()
#
# set_primary_window("Node Editor Demo", True)
# set_main_window_size(1600, 900)
# set_main_window_pos(50, 0)
# start_dearpygui()
