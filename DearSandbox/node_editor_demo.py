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


class SubT(IntEnum):
    NONE = 0
    ADDER = auto()
    FLT = auto()
    NPY = auto()
    INT = auto()
    LST_INT = auto()
    LST_FLT = auto()
    STR = auto()
    MAX = 255


class BaseT(IntFlag):
    ATTR = auto()
    NODE = auto()
    LINK = auto()
    NONE = 0


# print(x)
# print(int(x))
@dataclass
class NedID:
    base_t: BaseT
    sub_t: SubT
    instance: int = 0
    label: str = ""
    _byte: ClassVar[Struct] = Struct('BBh')
    _prop_mask: ClassVar[int] = 65535
    deci: ClassVar[int]

    def __post_init__(self):
        self.deci = int.from_bytes((self._byte.pack(int(self.base_t), int(self.sub_t), self.instance)), sys.byteorder)

    @classmethod
    def from_int(cls, i):
        r = cls._byte.unpack(int.to_bytes(i, 3, sys.byteorder))
        return cls(BaseT(r[0]), SubT(r[1]), r[2])

    def __int__(self):
        return int.from_bytes((self._byte.pack(int(self.base_t), int(self.sub_t), self.instance)), sys.byteorder)

    def __str__(self):
        return str(int(self))

    def __repr__(self):
        return int(self)

    def get_byte_arr(self):
        return self._byte.pack(int(self.base_t), int(self.sub_t), self.instance)

    def get_key(self):
        return str(self.base_t) + str(self.sub_t)

    def same_props_as(self, ned_id):
        return bool((int(self) & self._prop_mask) == (int(ned_id) & self._prop_mask))


class NodeEditor:
    def __init__(self, name):
        self.name = name
        self.nodes = {}
        self.attributes = {}
        self.tree = {}
        self.a_map = {}

    def node(self, node):
        if issubclass(node, Node):
            n = node(self)
            self.nodes[int(n.node_id)] = n
            n.show()

    def attr(self, nat, io):
        if issubclass(nat, NodeAttr):
            a = nat(self)
            self.attributes[a.aid] = a
            a.show()

    def show(self):
        with node_editor(self.name):
            add_text("cheese")

    def issue_id(self, base_t, sub_t, lbl):
        print(base_t, sub_t, lbl)
        if base_t not in self.tree:
            self.tree[base_t] = {}
        if sub_t not in self.tree[base_t]:
            self.tree[base_t][sub_t] = []
        ps = len(self.tree[base_t][sub_t]) + 1
        nid = NedID(base_t=base_t, sub_t=sub_t, instance=ps, friendly=str(lbl))
        self.tree[base_t][sub_t].append(nid)
        return nid


class Node(ABC):
    @abstractmethod
    def __init__(self, ed: NodeEditor, label):
        self.ed = ed
        self.node_id = ed.issue_id(BaseT.NODE, SubT.LST_FLT, lbl=label)
        self.attrs = self.compose_attrs()

    @abstractmethod
    def compose_attrs(self):
        pass

    @abstractmethod
    def tick(self, t, dt):
        pass

    def show(self):
        with node(obj_id=int(self.node_id), label=self.node_id.label, parent=str(self.ed.name)):
            for i in self.inputs:
                i.show()
            for o in self.outputs:
                o.show()


class NodeAttr(ABC):
    def __init__(self, node, param, widget, io):
        self.base_t = BaseT.ATTR
        self.sub_t = SubT.NONE
        self.node = node
        self.aid = self.node.ed.issue_id(self.base_t, self.sub_t, self.label())
        self.value = None
        self.widget = widget
        self.io = io
        self.param = param
        # self.neighbors = {}

    def on_link(self, neighbor):
        return True

    def cb(self, sender, data):
        self.value = get_value(sender)

    def on_break(self):
        return True

    def show(self):
        with node_attribute(int(self.aid), self.io, target=self.tgt(), parent=str(self.node.node_id)):
            self.widget(name=self.tgt(), default_value=self.param.val, min_value=self.param.low,
                        max_value=self.param.high, label=self.label())

    def set(self):
        ...

    def get(self):
        ...

    def tick(self):
        pass

    def tgt(self):
        return str(self.aid.label) + "##" + str(self.aid)

    @staticmethod
    def base_t():
        return BaseT.ATTR

    @abstractmethod
    def sub_t(self):
        pass

    @abstractmethod
    def label(self):
        pass


@dataclass
class IntParam:
    id: str = "int"
    val: float = 0
    low: float = -10
    high: float = 10
    fmt: str = "%i"
    lbl: str = id


class NodeIntAttr(NodeAttr):
    def __init__(self, node):
        p = IntParam()
        super().__init__(node, p, add_drag_int, 1)

    def label(self):
        return "ok"

    def sub_t(self):
        return SubT.INT


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


# bean = NedID(base=BaseT.LINK | BaseT.OUTPUT, sub=SubT.FLT, nid=2, aid=3)
# bean2 = NedID(base=BaseT.LINK | BaseT.OUTPUT, sub=SubT.FLT, nid=5, aid=4)
# print(bean)
# qq = int(bean)
# q2 = int(bean2)
# print(qq)
# print(qq >> 16 & qq)
# print(bean.same_props_as(bean2))
# mask = NedID(base=Nef(255), sub=Nat(255), nid=255, aid=0)
# zz = (int(mask))
# print(zz)
# print(NedID.from_int(qq & ~zz))


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


@dataclass
class Param:
    id: str
    default: any
    low: any
    high: any
    fmt: str
    lbl: str


#
#

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
with window("Node Editor Demo", width=1300, height=900, no_scrollbar=True, no_resize=False):
    add_text("DPG Node Editor Demo")
with window("sidebar", width=300, height=800, x_pos=30, y_pos=55, no_close=True):
    add_text("Hello")
with window("editor_panel", width=1000, height=800, x_pos=555, y_pos=55, no_close=True):
    n = NodeEditor("editorbean")
    n.show()
    n.node(BabyNode)

set_primary_window("Node Editor Demo", True)
set_main_window_size(1600, 900)
set_main_window_pos(50, 0)
start_dearpygui()
