#include <core/mvApp.h>
#include "mvNodeEditor.h"
#include "imgui.h"
#include "mvInput.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

mvNodeEditor::mvNodeEditor(const std::string &name,
						   PyObject *linkCreateCallback,
						   PyObject *linkDestroyCallback)
	: mvAppItem(name),
	  m_callback_link_created(linkCreateCallback),
	  m_callback_link_destroyed(linkDestroyCallback) {
  m_description.container = true;
  m_callback_link_created = linkCreateCallback;
}

std::vector<int> mvNodeEditor::getSelectedNodes() {
  const int num_nodes_selected = imnodes::NumSelectedNodes();
  if (num_nodes_selected > 0) {
	static std::vector<int> selected_nodes;
	selected_nodes.resize(static_cast<size_t>(num_nodes_selected));
	imnodes::GetSelectedNodes(selected_nodes.data());
	return selected_nodes;
  }
}

std::vector<int> mvNodeEditor::getSelectedLinks() {
  const int num_links_selected = imnodes::NumSelectedLinks();
  if (num_links_selected > 0) {
	static std::vector<int> selected_links;
	selected_links.resize(static_cast<size_t>(num_links_selected));
	imnodes::GetSelectedLinks(selected_links.data());
	return selected_links;
  }
}

void mvNodeEditor::draw() {

  imnodes::GetIO().link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;
  imnodes::BeginNodeEditor();

  for (mvAppItem *item : m_children) {
	item->draw();
	item->getState().update();
  }

  for (auto link : m_links) {
	imnodes::Link(link.id, link.start_attr, link.end_attr);
  }

  imnodes::EndNodeEditor();



  // region Check for created links
  {
	int start_attr, end_attr;
	if (imnodes::IsLinkCreated(&start_attr, &end_attr) &&
		m_callback_link_created != nullptr) {
	  PyGILState_STATE g_state = PyGILState_Ensure();
	  PyObject *link = PyTuple_New(2);
	  PyTuple_SetItem(link, 0, ToPyInt(start_attr));
	  PyTuple_SetItem(link, 1, ToPyInt(end_attr));
	  PyGILState_Release(g_state);
	  mvApp::GetApp()->runCallback(m_callback_link_created, m_name, link);
	}
  }
  // endregion

  // region Check for destroyed links
  {
	int link_id;
	if (imnodes::IsLinkDestroyed(&link_id) &&
		m_callback_link_destroyed != nullptr) {
	  PyGILState_STATE g_state = PyGILState_Ensure();
	  PyObject *link = ToPyInt(link_id);
	  PyGILState_Release(g_state);
	  mvApp::GetApp()->runCallback(
		  m_callback_link_destroyed, m_name, link);
	}
  }
  // endregion
}

void mvNodeEditor::addLink(mvLink link) {
  m_links.push_back(link);
}

bool mvNodeEditor::are_attrs_linked(int start_attr, int end_attr) {
  for (auto link : m_links) {
	if (link.start_attr == start_attr && link.end_attr == end_attr) {
	  return true;
	}
  }
  return false;
}

bool mvNodeEditor::link_exists(int link_id = -1, int start_attr = -1, int end_attr = -1) {
  for (auto link : m_links) {
	if ((link.start_attr == start_attr && link.end_attr == end_attr) || link.id == link_id) {
	  return true;
	}
  }
  return false;
}

bool mvNodeEditor::deleteLink(int link_id) {
  if (link_exists(link_id)) {
	auto oldLinks = m_links;
	m_links.clear();
	for (auto item : oldLinks) {
	  if (item.id == link_id) {
		delete &item;
		continue;
	  }
	  m_links.push_back(item);
	}
	return true;
  } else {
	return false;
  }
}

std::vector<mvLink> *mvNodeEditor::getLinks() {
  return &m_links;
}

} // namespace Marvel