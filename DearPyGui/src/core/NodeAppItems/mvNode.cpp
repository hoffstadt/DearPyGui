#include "mvNode.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {
mvNode::mvNode(const std::string &name) : mvBoolPtrBase(name, false, name) {
  m_description.container = true;
  m_node_id = std::stoi(name);
}

void mvNode::draw() {
  // region Push Node Styling
  imnodes::PushStyleVar(imnodes::StyleVar_NodeCornerRounding,
						m_node_corner_rounding);
  imnodes::PushStyleVar(imnodes::StyleVar_NodePaddingHorizontal,
						m_node_padding_h);
  imnodes::PushStyleVar(imnodes::StyleVar_NodePaddingVertical,
						m_node_padding_v);
  imnodes::PushStyleVar(imnodes::StyleVar_NodeBorderThickness,
						m_node_border_weight);
  // endregion

  imnodes::BeginNode(m_node_id);

  if (m_auto_title_bar) {
	imnodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(&m_label[0]);
	imnodes::EndNodeTitleBar();
  }

  // draw child elements.
  if (m_width > 0) // if the node has a defined width
  {
	ImGui::PushItemWidth((float)m_width);
  }

  for (mvAppItem *item : m_children) {
	if (item->getWidth() >
		0) // if this child has a defined width, allow it to override
	{
	  ImGui::SetNextItemWidth((float)item->getWidth());
	} else if (m_width > 0) {
	  item->setWidth(m_width);
	}

	// skip item if it's not shown
	if (!item->isShown())
	  continue;

	item->draw();

	// Regular Tooltip (simple)
	if (!item->getTip().empty() && ImGui::IsItemHovered())
	  ImGui::SetTooltip("%s", item->getTip().c_str());

	item->getState().update();
  }

  if (m_width > 0)
	ImGui::PopItemWidth();

  imnodes::EndNode();
  // region Pop Node Styling
  imnodes::SetNodeDraggable(m_node_id, m_draggable);
  imnodes::PopStyleVar();
  imnodes::PopStyleVar();
  imnodes::PopStyleVar();
  imnodes::PopStyleVar();
  // endregion
  if (m_dirty_pos) {
	imnodes::SetNodeEditorSpacePos(m_node_id, ImVec2(m_x_pos, m_y_pos));
	m_dirty_pos = false;
  }
}

void mvNode::setExtraConfigDict(PyObject *dict) {
  if (dict == nullptr)
	return;

  mvGlobalIntepreterLock gil;
  if (PyObject *item = PyDict_GetItemString(dict, "x_pos"))
	m_x_pos = ToFloat(item);
  if (PyObject *item = PyDict_GetItemString(dict, "y_pos"))
	m_y_pos = ToFloat(item);
  if (PyObject *item = PyDict_GetItemString(dict, "draggable"))
	m_draggable = ToBool(item);
  if (PyObject *item = PyDict_GetItemString(dict, "auto_title_bar"))
	m_auto_title_bar = ToBool(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_corner_rounding"))
	m_node_corner_rounding = ToFloat(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_padding_h"))
	m_node_padding_h = ToFloat(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_padding_v"))
	m_node_padding_v = ToFloat(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_border_weight"))
	m_node_border_weight = ToFloat(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_background"))
	m_node_background = ToColor(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_outline"))
	m_node_outline = ToColor(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_hovered"))
	m_node_background_hovered = ToColor(item);
  if (PyObject *item = PyDict_GetItemString(dict, "node_selected"))
	m_node_background_selected = ToColor(item);
}

void mvNode::getExtraConfigDict(PyObject *dict) {
  if (dict == nullptr)
	return;

  mvGlobalIntepreterLock gil;
  PyDict_SetItemString(dict, "x_pos", ToPyFloat(m_x_pos));
  PyDict_SetItemString(dict, "y_pos", ToPyFloat(m_y_pos));
  PyDict_SetItemString(dict, "draggable", ToPyBool(m_draggable));
  PyDict_SetItemString(dict, "auto_title_bar", ToPyBool(m_auto_title_bar));
  PyDict_SetItemString(dict, "node_corner_rounding",
					   ToPyFloat(m_node_corner_rounding));
  PyDict_SetItemString(dict, "node_padding_h", ToPyFloat(m_node_padding_h));
  PyDict_SetItemString(dict, "node_padding_v", ToPyFloat(m_node_padding_v));
  PyDict_SetItemString(dict, "node_border_weight",
					   ToPyFloat(m_node_border_weight));
  PyDict_SetItemString(dict, "node_background", ToPyColor(m_node_background));
  PyDict_SetItemString(dict, "node_outline", ToPyColor(m_node_outline));
  PyDict_SetItemString(dict, "node_hovered",
					   ToPyColor(m_node_background_hovered));
  PyDict_SetItemString(dict, "node_selected",
					   ToPyColor(m_node_background_selected));
}

} // namespace Marvel