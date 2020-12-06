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
    INPUT = 0
    OUTPUT = 1
    STATIC = 2
    LINK_T = auto()
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


@dataclass
class NedID:
    base_t: BaseT
    sub_t: SubT
    instance: int = 0
    label: str = ""
    _byte: ClassVar[Struct] = Struct("BBh")
    _prop_mask: ClassVar[int] = 65535
    deci: ClassVar[int]

    def __post_init__(self):
        self.deci = int.from_bytes(
            (self._byte.pack(int(self.base_t), int(self.sub_t), self.instance)),
            sys.byteorder,
        )

    @classmethod
    def from_int(cls, i):
        r = cls._byte.unpack(int.to_bytes(i, 3, sys.byteorder))
        return cls(BaseT(r[0]), SubT(r[1]), r[2])

    def __int__(self):
        return int.from_bytes(
            (self._byte.pack(int(self.base_t), int(self.sub_t), self.instance)),
            sys.byteorder,
        )

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
        self.links = {}

    def node(self, node):
        if issubclass(node, Node):
            n = node(self)
            self.nodes[int(n.node_id)] = n
            n.show()

    def attr(self, n_attr):
        self.attributes[int(n_attr.a_id)] = n_attr

    def cb_link_destroyed(self, sender, data):
        start_at, end_at = self.links[data]
        start_at.unlink(end_at)
        end_at.unlink(start_at)
        self.links.pop(data)
        delete_link(self.name, data)

    def cb_link_created(self, sender, data):
        id = self.issue_id(BaseT.LINK, SubT.LINK_T, str(data))
        start_at = self.attributes[data[0]]
        end_at = self.attributes[data[1]]
        if start_at.link(end_at) and end_at.link(start_at):
            add_link(sender, obj_id=int(id), start_attribute=data[0], end_attribute=data[1])
            self.links[int(id)] = (start_at, end_at)

    def tick(self, sender, data):
        for n in self.nodes:
            self.nodes[n].tick()

    def show(self):
        with node_editor(self.name, callback_link_created=self.cb_link_created,
                         callback_link_destroyed=self.cb_link_destroyed):
            add_text("cheese")

    def issue_id(self, base_t, sub_t, lbl):

        if base_t not in self.tree:
            self.tree[base_t] = {}
        if sub_t not in self.tree[base_t]:
            self.tree[base_t][sub_t] = []
        ps = len(self.tree[base_t][sub_t]) + 1
        nid = NedID(base_t=base_t, sub_t=sub_t, instance=ps, label=str(lbl))
        self.tree[base_t][sub_t].append(nid)
        print("# issued ID # \t", base_t, "\t", lbl, "\t", int(nid), "\t", sub_t)
        return nid


class Node(ABC):
    @abstractmethod
    def __init__(self, ed: NodeEditor, label):
        self.ed = ed
        self.node_id = ed.issue_id(BaseT.NODE, SubT.LST_FLT, lbl=label)

    @abstractmethod
    def compose_node(self):
        pass

    @abstractmethod
    def tick(self):
        pass

    def show(self):
        add_node(obj_id=int(self.node_id), label=self.node_id.label, parent=str(self.ed.name))
        self.compose_node()
        end()


@dataclass
class NAT:
    node: Node
    name: str
    widget: callable
    sub_t: SubT
    v: any
    allow_multi: bool = False
    source_match: bool = True

    def __post_init__(self):
        self.neighbors = {}
        self.a_id = self.node.ed.issue_id(BaseT.ATTR, self.sub_t, self.name)
        self.tgt: str = self.name + "##" + str(self.a_id)
        self.node.ed.attr(self)

    def link(self, other):
        if len(self.neighbors) > 1 and not self.allow_multi:
            return False
        if self.sub_t != other.sub_t:
            return False
        self.neighbors[int(other.a_id)] = other
        return True

    def unlink(self, other):
        if int(other.a_id) not in self.neighbors:
            print("Failed to unlink", self.a_id, "with", other.a_id)
            return False
        else:
            self.neighbors.pop(int(other.a_id))
            return True

    @property
    def value(self):
        return self.v

    @value.setter
    def value(self, value):
        self.v = value
        set_value(self.tgt, self.v)

    def show(self, **kwargs):
        with node_attribute(
                int(self.a_id), 2, target=self.tgt, parent=str(self.node.node_id)
        ):
            self.widget(
                name=self.tgt,
                **kwargs
            )

    def __str__(self):
        return 'NodeAttribute(%s):ID(%s) -> neighbors(%s)' % (self.name, self.a_id, self.neighbors)


@dataclass
class InputAttribute(NAT):

    def _cb(self, sender, data):
        self.value = get_value(sender)

    def show(self, **kwargs):
        add_node_attribute(int(self.a_id), 0, target=self.tgt, parent=str(self.node.node_id))
        self.widget(
            name=self.tgt,
            default_value=self.value,
            label=self.name,
            callback=self._cb,
            callback_data=self.name,
            **kwargs
        )
        end()


@dataclass
class OutputAttribute(NAT):
    def propagate(self):
        z = self.neighbors
        for k in z:
            z[k].value = self.value

    def show(self, **kwargs):
        with node_attribute(
                int(self.a_id), 1, target=self.tgt, parent=str(self.node.node_id)
        ):
            self.widget(
                name=self.tgt,
                **kwargs
            )
