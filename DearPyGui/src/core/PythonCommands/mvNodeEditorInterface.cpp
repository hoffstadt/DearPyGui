#include "mvNodeEditorInterface.h"
#include "mvInterfaceCore.h"
#include <core/NodeAppItems/mvNodeEditor.h>
#include <core/NodeAppItems/mvNode.h>
#include <core/NodeAppItems/mvNodeAttribute.h>
#include <core/NodeAppItems/mvNodeTitleBar.h>

namespace Marvel {

static mvNodeEditor *verifyNodeEditor(const char *id) {

  mvAppItem *attr = mvApp::GetApp()->getItemRegistry().getItem(id);

  if (attr == nullptr) {
	ThrowPythonException("Item with ID " + std::string(id) +
		" does not exist.");
	return nullptr;
  }
  if (attr->getType() != mvAppItemType::NodeEditor) {
	ThrowPythonException("Item with ID " + std::string(id) +
		" is not a node editor.");
	return nullptr;
  } else {
	return dynamic_cast<mvNodeEditor *>(attr);
  }
}

static mvNode *verifyNode(const char *id) {

  mvAppItem *attr = mvApp::GetApp()->getItemRegistry().getItem(id);

  if (attr == nullptr) {
	ThrowPythonException("Item with ID " + std::string(id) +
		" does not exist.");
	return nullptr;
  }
  if (attr->getType() != mvAppItemType::NodeEditor) {
	ThrowPythonException("Item with ID " + std::string(id) +
		" is not a Node.");
	return nullptr;
  } else {
	return dynamic_cast<mvNode *>(attr);
  }
}

static mvNodeAttribute *verifyAttribute(const int id) {
  std::string ids = std::to_string(id);
  mvAppItem *attr = mvApp::GetApp()->getItemRegistry().getItem(ids);

  if (attr == nullptr) {
	ThrowPythonException("Item with ID " + ids + " does not exist.");
	return nullptr;
  }
  if (attr->getType() != mvAppItemType::NodeAttribute) {
	ThrowPythonException("Item with ID " + ids +
		" is not a node attribute.");
	return nullptr;
  } else {
	return dynamic_cast<mvNodeAttribute *>(attr);
  }
}

void node_editor_interface(std::map<std::string, mvPythonParser> *parsers) {
  parsers->insert(
	  {"add_node_editor",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String, "name"},
				  {mvPythonDataType::KeywordOnly},
				  {mvPythonDataType::Float,
					  "grid_spacing",
					  "spacing of grid lines on node editor canvas",
					  "32.0"},
				  {mvPythonDataType::Callable,
					  "callback_link_created",
					  "function to call on link creation event",
					  "None"},
				  {mvPythonDataType::Callable,
					  "callback_link_destroyed",
					  "function to call on link destruction event",
					  "None"},
				  // inherited
				  {mvPythonDataType::String,
					  "parent",
					  "Parent to add this item to. (runtime adding)",
					  "''"},
				  {mvPythonDataType::String,
					  "before",
					  "This item will be displayed before the specified item in the parent. (runtime adding)",
					  "''"},
				  {mvPythonDataType::Integer, "width", "", "-1"},
				  {mvPythonDataType::Integer, "height", "", "-1"},
				  {mvPythonDataType::Bool, "show", "Attempt to render", "True"},

			  },
			  "Adds a node editor canvas.",
			  "None",
			  "NodeEditor")});
}

void node_interface(std::map<std::string, mvPythonParser> *parsers) {
  // region add_node
  parsers->insert(
	  {"add_node",
		  mvPythonParser(
			  {
				  {mvPythonDataType::Integer,
					  "obj_id",
					  "the unique ID (int) to represent this instance of a node"},
				  {mvPythonDataType::KeywordOnly},
				  {mvPythonDataType::Float,
					  "x_pos",
					  "position of node on node editor canvas",
					  "10"},
				  {mvPythonDataType::Float,
					  "y_pos",
					  "position of node on node editor canvas",
					  "10"},
				  {mvPythonDataType::String, "label", "the name of the node"},
				  {mvPythonDataType::Bool,
					  "draggable",
					  "set whether the node can be dragged",
					  "True"},
				  {mvPythonDataType::Bool,
					  "auto_title_bar",
					  "generate a default title bar. if false, a title bar MUST be "
					  "manually added.",
					  "True"},
				  {mvPythonDataType::Float,
					  "node_corner_rounding",
					  "Styling.",
					  "4.0"},
				  {mvPythonDataType::Float,
					  "node_padding_h",
					  "Styling.",
					  "8.0"},
				  {mvPythonDataType::Float,
					  "node_padding_v",
					  "Styling.",
					  "1.0"},
				  {mvPythonDataType::Float,
					  "node_border_weight",
					  "Styling.",
					  "1.0"},
				  // inherited
				  {mvPythonDataType::String,
					  "parent",
					  "Parent to add this item to. (runtime adding)",
					  "''"},
				  {mvPythonDataType::String,
					  "before",
					  "This item will be displayed before the specified item in the parent. (runtime adding)",
					  "''"},
				  {mvPythonDataType::Integer, "width", "", "100"},
				  {mvPythonDataType::Integer, "height", "", "100"},
				  {mvPythonDataType::Bool, "show", "Attempt to render", "True"},

			  },
			  "Adds a node within a node_editor.",
			  "None",
			  "NodeEditor")});
  // endregion
}

void node_title_bar_interface(std::map<std::string, mvPythonParser> *parsers) {
  // region add_node_title_bar
  parsers->insert(
	  {"add_node_title_bar",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String,
					  "name",
					  "unique string ID for this node title bar"},
				  {mvPythonDataType::KeywordOnly},
				  {mvPythonDataType::FloatList,
					  "title_bar_color",
					  "node background color",
					  "(41, 74, 122, 255)"},
				  {mvPythonDataType::FloatList,
					  "title_bar_hovered",
					  "node outline color",
					  "(41, 74, 122, 255)"},
				  {mvPythonDataType::FloatList,
					  "title_bar_selected",
					  "node hovered color",
					  "(66, 150, 250, 255)"},
				  // inherited
				  {mvPythonDataType::String,
					  "parent",
					  "Parent to add this item to. (runtime adding)",
					  "''"},
				  {mvPythonDataType::String,
					  "before",
					  "This item will be displayed before the specified item in the parent. (runtime adding)",
					  "''"},
				  {mvPythonDataType::Integer, "width", "", "-1"},
				  {mvPythonDataType::Integer, "height", "", "-1"},
				  {mvPythonDataType::Bool, "show", "Attempt to render", "True"},

			  },
			  "Adds a title bar to a node.",
			  "None",
			  "NodeEditor")});
  // endregion
}

void node_attribute_interface(std::map<std::string, mvPythonParser> *parsers) {
  // region add_node_attribute
  parsers->insert(
	  {"add_node_attribute",
		  mvPythonParser(
			  {
				  {mvPythonDataType::Integer,
					  "obj_id",
					  "the unique attribute ID (int) to represent this instance"},
				  {mvPythonDataType::Integer,
					  "type",
					  "input, output, or static, 0, 1, 2"},
				  {mvPythonDataType::KeywordOnly},
				  {mvPythonDataType::Integer,
					  "pin_shape",
					  "shape of attribute pin (circle,tri,quad)",
					  "1"},
				  {mvPythonDataType::String,
					  "target",
					  "store the name of a particular child widget in this attribute",
					  "''"},
				  // inherited
				  {mvPythonDataType::String,
					  "parent",
					  "Parent to add this item to. (runtime adding)",
					  "''"},
				  {mvPythonDataType::String,
					  "before",
					  "This item will be displayed before the specified item in the parent. (runtime adding)",
					  "''"},
				  {mvPythonDataType::Integer, "width", "", "-1"},
				  {mvPythonDataType::Integer, "height", "", "-1"},
				  {mvPythonDataType::Bool, "show", "Attempt to render", "True"},

			  },
			  "Adds a node attribute.",
			  "None",
			  "NodeEditor")});
  // endregion
}

void node_editor_utils(std::map<std::string, mvPythonParser> *parsers) {
  // region add_link
  parsers->insert(
	  {"add_link",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String,
					  "editor",
					  "the string id for the editor where this link will be drawn"},
				  {mvPythonDataType::Integer, "obj_id", "the unique id for this link"},
				  {mvPythonDataType::Integer,
					  "start_attribute",
					  "the attribute ID (int) at which to start the link"},
				  {mvPythonDataType::Integer,
					  "end_attribute",
					  "the attribute ID (int) at which to end the link"},
				  {mvPythonDataType::KeywordOnly},
			  },
			  "Adds a link between two node attributes.",
			  "None",
			  "NodeEditor")});
  // endregion

  // region delete_link
  parsers->insert(
	  {"delete_link",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String,
					  "editor",
					  "the string id for the editor where this link should be deleted from"},
				  {mvPythonDataType::Integer, "obj_id", "the unique id for this link"},
			  },
			  "Deletes a link between two node attributes.",
			  "None",
			  "NodeEditor")});
  // endregion

  // region get_links
  parsers->insert(
	  {"get_links",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String,
					  "editor",
					  "the string id for the editor where this link will be drawn"},
			  },
			  "Get a list of links active in the editor.",
			  "None",
			  "NodeEditor")});
  // endregion

  // region get_selected_nodes
  parsers->insert(
	  {"get_selected_nodes",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String,
					  "editor",
					  "the string id for the editor where this link should be deleted from"},
			  },
			  "Get a list of selected nodes in the editor.",
			  "None",
			  "NodeEditor")});
  // endregion

  // region get_selected_links
  parsers->insert(
	  {"get_selected_links",
		  mvPythonParser(
			  {
				  {mvPythonDataType::String,
					  "editor",
					  "the string id for the editor where this link should be deleted from"},
			  },
			  "Get a list of selected links in the editor.",
			  "None",
			  "NodeEditor")});
  // endregion
}

void AddNodeEditorCommands(std::map<std::string, mvPythonParser> *parsers) {
  node_editor_interface(parsers);
  node_interface(parsers);
  node_title_bar_interface(parsers);
  node_attribute_interface(parsers);
  node_editor_utils(parsers);

}

PyObject *add_node_editor(PyObject *self, PyObject *args, PyObject *kwargs) {
  const char *name;
  float grid_spacing = 32.f;
  PyObject *callback_link_created = nullptr;
  PyObject *callback_link_destroyed = nullptr;
  const char *parent = "";
  const char *before = "";
  int width = -1;
  int height = -1;
  bool show = true;

  if (!(*mvApp::GetApp()->getParsers())["add_node_editor"].parse(
	  args,
	  kwargs,
	  __FUNCTION__,
	  &name,
	  &grid_spacing,
	  &callback_link_created,
	  &callback_link_destroyed,
	  &parent,
	  &before,
	  &width,
	  &height,
	  &show)) {
	return ToPyBool(false);
  }

  if (callback_link_created) Py_XINCREF(callback_link_created);
  if (callback_link_destroyed) Py_XINCREF(callback_link_destroyed);

  mvAppItem *item = new mvNodeEditor(name,
									 callback_link_created,
									 callback_link_destroyed);
  if (!item) return ToPyBool(false);

  item->checkConfigDict(kwargs);
  item->setConfigDict(kwargs);
  item->setExtraConfigDict(kwargs);

  if (AddItemWithRuntimeChecks(item, parent, before)) {

	mvApp::GetApp()->getItemRegistry().pushParent(item);

	return ToPyBool(true);
  }
  return ToPyBool(false);
}

PyObject *add_node(PyObject *self, PyObject *args, PyObject *kwargs) {
  int id;
  float x_pos;
  float y_pos;
  const char *label = "";
  bool draggable = true;
  bool auto_title_bar = true;
  float node_corner_rounding = 4.f;
  float node_padding_h = 8.f;
  float node_padding_v = 1.f;
  float node_border_weight = 1.f;
  const char *parent = "";
  const char *before = "";
  int width = -1;
  int height = -1;
  bool show = true;

  if (!(*mvApp::GetApp()->getParsers())["add_node"].parse(
	  args,
	  kwargs,
	  __FUNCTION__,
	  &id,
	  &x_pos,
	  &y_pos,
	  &label,
	  &draggable,
	  &auto_title_bar,
	  &node_corner_rounding,
	  &node_padding_h,
	  &node_padding_v,
	  &node_border_weight,
	  &parent,
	  &before,
	  &width,
	  &height,
	  &show)) {
	return ToPyBool(false);
  }

  std::string rename;
  rename.append(std::to_string(id));

  if (!std::string(parent).empty()) {
	auto ed = verifyNodeEditor(parent);
	if (ed == nullptr) {
	  ThrowPythonException(
		  "add_node: Specified an invalid parent.");
	  return ToPyBool(false);
	}
  } else {
	auto tp = mvApp::GetApp()->getItemRegistry().topParent();
	if (tp == nullptr || tp->getType() != mvAppItemType::NodeEditor) {
	  ThrowPythonException(
		  "add_node must follow a call to add_node_editor.");
	  return ToPyBool(false);
	}
  }
  mvAppItem *item = new mvNode(rename);
  if (!std::string(label).empty()) {
	item->setLabel(label);
  }
  item->checkConfigDict(kwargs);
  item->setConfigDict(kwargs);
  item->setExtraConfigDict(kwargs);

  if (AddItemWithRuntimeChecks(item, parent, before)) {
	mvApp::GetApp()->getItemRegistry().pushParent(item);
	return ToPyBool(true);
  } else {
	return ToPyBool(false);
  }
}

PyObject *add_node_title_bar(PyObject *self, PyObject *args, PyObject *kwargs) {
  const char *name;

  mvColor title_bar_color = mvColor(41, 74, 122, 255);
  mvColor title_bar_hovered = mvColor(66, 150, 250, 255);
  mvColor title_bar_selected = mvColor(66, 150, 250, 255);
  const char *parent = "";
  const char *before = "";
  int width = -1;
  int height = -1;
  bool show = true;

  if (!(*mvApp::GetApp()->getParsers())["add_node_title_bar"].parse(
	  args,
	  kwargs,
	  __FUNCTION__,
	  &name,
	  &title_bar_color,
	  &title_bar_hovered,
	  &title_bar_selected,
	  &parent,
	  &before,
	  &width,
	  &height,
	  &show)) {
	return ToPyBool(false);
  }

  if (!std::string(parent).empty()) {
	auto ed = verifyNode(parent);
	if (ed == nullptr) {
	  ThrowPythonException(
		  "add_node_title_bar: Specified an invalid parent.");
	  return ToPyBool(false);
	}
  } else {
	auto tp = mvApp::GetApp()->getItemRegistry().topParent();
	if (tp == nullptr || tp->getType() != mvAppItemType::Node) {
	  ThrowPythonException(
		  "add_node_title_bar must follow a call to add_node.");
	  return ToPyBool(false);
	}
  }

  mvAppItem *item = new mvNodeTitleBar(std::string(name));

  item->checkConfigDict(kwargs);
  item->setConfigDict(kwargs);
  item->setExtraConfigDict(kwargs);

  if (AddItemWithRuntimeChecks(item, parent, before)) {
	mvApp::GetApp()->getItemRegistry().pushParent(item);
	return ToPyBool(true);
  } else {
	return ToPyBool(false);
  }
}

PyObject *add_node_attribute(PyObject *self, PyObject *args, PyObject *kwargs) {

  int id;
  int attribute_type;
  int pin_shape = 1;
  const char *target = "";
  const char *parent = "";
  const char *before = "";
  int width = -1;
  int height = -1;
  bool show = true;

  if (!(*mvApp::GetApp()->getParsers())["add_node_attribute"].parse(
	  args,
	  kwargs,
	  __FUNCTION__,
	  &id,
	  &attribute_type,
	  &pin_shape,
	  &target,
	  &parent,
	  &before,
	  &width,
	  &height,
	  &show)) {
	return ToPyBool(false);
  }

  mvNodeAttributeType a_type;
  switch (attribute_type) {
	case (0):a_type = mvNodeAttributeType::mvNodeAttribute_Input;
	  break;
	case (1):a_type = mvNodeAttributeType::mvNodeAttribute_Output;
	  break;
	case (2):a_type = mvNodeAttributeType::mvNodeAttribute_Static;
	  break;
	default:
	  ThrowPythonException(
		  "node_input_attribute attribute_type must be of the specified types.");
	  return ToPyBool(false);
  }

  //sanitize enum constant for pin shape
  if (pin_shape > 6 || pin_shape < 0) {
	pin_shape = 1;
  }

  if (!std::string(parent).empty()) {
	auto ed = verifyNode(parent);
	if (ed == nullptr) {
	  ThrowPythonException(
		  "add_node_attribute: Specified an invalid parent.");
	  return ToPyBool(false);
	}
  } else {
	auto tp = mvApp::GetApp()->getItemRegistry().topParent();
	if (tp == nullptr || tp->getType() != mvAppItemType::Node) {
	  ThrowPythonException(
		  "add_node_attribute must follow a call to add_node.");
	  return ToPyBool(false);
	}
  }

  mvAppItem *item =
	  new mvNodeAttribute(std::to_string(id), id, a_type, target);
  item->checkConfigDict(kwargs);
  item->setConfigDict(kwargs);
  item->setExtraConfigDict(kwargs);
  if (AddItemWithRuntimeChecks(item, parent, before)) {
	mvApp::GetApp()->getItemRegistry().pushParent(item);
	return ToPyBool(true);
  } else {
	return ToPyBool(false);
  }

}

PyObject *add_link(PyObject *self, PyObject *args, PyObject *kwargs) {

  const char *editor;
  int id;
  int start_attr;
  int end_attr;
  mvColor link_color = mvColor(50, 50, 50, 255);
  mvColor link_hovered = mvColor(100, 100, 100, 255);
  mvColor link_selected = mvColor(75, 75, 75, 255);
  float link_weight = 3.f;
  float link_segments = 0.1;
  float link_hover_distance = 10.f;

  if (!(*mvApp::GetApp()->getParsers())["add_link"].parse(
	  args,
	  kwargs,
	  __FUNCTION__,
	  &editor,
	  &id,
	  &start_attr,
	  &end_attr,
	  &link_color,
	  &link_hovered,
	  &link_selected,
	  &link_weight,
	  &link_segments,
	  &link_hover_distance)) {
	return ToPyBool(false);
  }

  mvNodeEditor *ed = verifyNodeEditor(editor);
  mvNodeAttribute *start = verifyAttribute(start_attr);
  mvNodeAttribute *end = verifyAttribute(end_attr);

  const std::string rename = std::to_string(id);

  if (ed == nullptr || start == nullptr || end == nullptr) {
	ThrowPythonException("Failed to create link " + std::to_string(id) +
		" : invalid attributes or editor.");
	return ToPyBool(false);
  } else {
	if (ed->are_attrs_linked(start_attr, end_attr)) {
	  ThrowPythonException("Failed to create link " + std::to_string(id) +
		  " : attributes in question are already linked.");
	  return ToPyBool(false);
	} else {
	  auto link = mvLink{id, start_attr, end_attr};
	  ed->addLink(link);
	  return GetPyNone();
	}
  }
}

PyObject *delete_link(PyObject *self, PyObject *args, PyObject *kwargs) {

  const char *editor;
  int id;

  if (!(*mvApp::GetApp()->getParsers())["delete_link"].parse(
	  args, kwargs, __FUNCTION__, &editor, &id)) {
	return ToPyBool(false);
  }
  mvNodeEditor *ed = verifyNodeEditor(editor);

  const std::string rename = std::to_string(id);

  if (ed == nullptr) {
	ThrowPythonException("Failed to delete link " + std::to_string(id) +
		" : invalid editor.");
	return ToPyBool(false);
  } else {
	if (ed->deleteLink(id)) {
	  return ToPyBool(true);
	}
  }
  return ToPyBool(false);
}

PyObject *get_links(PyObject *self, PyObject *args, PyObject *kwargs) {
  const char *editor;

  if (!(*mvApp::GetApp()->getParsers())["get_links"].parse(
	  args, kwargs, __FUNCTION__, &editor)) {
	return ToPyBool(false);
  }

  mvNodeEditor *ed = verifyNodeEditor(editor);
  if (ed == nullptr) {
	ThrowPythonException("Failed to get links of editor " + std::to_string(*editor) +
		" : invalid editor.");
	return ToPyBool(false);
  } else {
	return ToPyListLinks(*ed->getLinks());
  }
}

PyObject *get_selected_links(PyObject *self, PyObject *args, PyObject *kwargs) {
  const char *editor;
  mvNodeEditor *ed = verifyNodeEditor(editor);

  if (!(*mvApp::GetApp()->getParsers())["get_selected_links"].parse(
	  args, kwargs, __FUNCTION__, &editor)) {
	return ToPyBool(false);
  }

  if (ed == nullptr) {
	ThrowPythonException("Failed to get selected links of editor " + std::to_string(*editor) +
		" : invalid editor.");
	return ToPyBool(false);
  } else {
	std::vector<int> l_vec = ed->getSelectedLinks();
	return ToPyList(l_vec);
  }
}

PyObject *get_selected_nodes(PyObject *self, PyObject *args, PyObject *kwargs) {
  const char *editor;
  mvNodeEditor *ed = verifyNodeEditor(editor);
  if (!(*mvApp::GetApp()->getParsers())["get_selected_nodes"].parse(
	  args, kwargs, __FUNCTION__, &editor)) {
	return ToPyBool(false);
  }
  if (ed == nullptr) {
	ThrowPythonException("Failed to get selected links of editor " + std::to_string(*editor) +
		" : invalid editor.");
	return ToPyBool(false);
  } else {
	std::vector<int> l_vec = ed->getSelectedNodes();
	return ToPyList(l_vec);
  }
}
} // namespace Marvel