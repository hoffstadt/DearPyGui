#include "mvNodeTitleBar.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

mvNodeTitleBar::mvNodeTitleBar(const std::string &name) : mvAppItem(name) {
  m_description.container = true;
}

void mvNodeTitleBar::draw() {

  imnodes::BeginNodeTitleBar();
  imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, m_title_bar_color);
  imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, m_title_bar_hovered);
  imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, m_title_bar_selected);

  if (m_width != 0)
	ImGui::PushItemWidth((float)m_width);

  for (mvAppItem *item : m_children) {
	if (item->getWidth() > 0) {
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

  if (m_width != 0)
	ImGui::PopItemWidth();

  imnodes::EndNodeTitleBar();
  imnodes::PopColorStyle();
  imnodes::PopColorStyle();
  imnodes::PopColorStyle();
}

void mvNodeTitleBar::setExtraConfigDict(PyObject *dict) {
  if (dict == nullptr) return;

  mvGlobalIntepreterLock gil;

  if (PyObject *item = PyDict_GetItemString(dict, "title_bar_color"))
	m_title_bar_color = ToColor(item);
  if (PyObject *item = PyDict_GetItemString(dict, "title_bar_hovered"))
	m_title_bar_hovered = ToColor(item);
  if (PyObject *item = PyDict_GetItemString(dict, "title_bar_selected"))
	m_title_bar_selected = ToColor(item);
}

void mvNodeTitleBar::getExtraConfigDict(PyObject *dict) {
  if (dict == nullptr) return;

  mvGlobalIntepreterLock gil;

  PyDict_SetItemString(dict, "title_bar_color", ToPyColor(m_title_bar_color));
  PyDict_SetItemString(
	  dict, "title_bar_hovered", ToPyColor(m_title_bar_hovered));
  PyDict_SetItemString(
	  dict, "title_bar_selected", ToPyColor(m_title_bar_selected));
}

}
