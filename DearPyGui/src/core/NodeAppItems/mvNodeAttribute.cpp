#include "mvNodeAttribute.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {
mvNodeAttribute::mvNodeAttribute(const std::string &name,
								 int id,
								 mvNodeAttributeType a_type,
								 const std::string &default_value)
	: mvStringPtrBase(name, default_value, "") {
  m_attr_id = id;
  m_attr_type = a_type;
  m_description.container = true;
}

void mvNodeAttribute::draw() {

  if (m_pin_shape > 5 || m_pin_shape < 0) {
	m_pin_shape = 1;
  }
  auto pinShape = imnodes::PinShape(m_pin_shape);

  imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);
  switch (m_attr_type) {
	case mvNodeAttributeType::mvNodeAttribute_Input: imnodes::BeginInputAttribute(m_attr_id, pinShape);
	  break;
	case mvNodeAttributeType::mvNodeAttribute_Output: imnodes::BeginOutputAttribute(m_attr_id, pinShape);
	  break;
	case mvNodeAttributeType::mvNodeAttribute_Static: imnodes::BeginStaticAttribute(m_attr_id);
	  break;
  }

  if (m_width != 0)
	ImGui::PushItemWidth((float)m_width);

  for (mvAppItem *item : m_children) {
	if (item->getWidth() > 0) {
	  ImGui::SetNextItemWidth((float)item->getWidth());
	} else if (m_width > 0) {
	  item->setWidth(m_width);
	}
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

  switch (m_attr_type) {
	case mvNodeAttributeType::mvNodeAttribute_Input: imnodes::EndInputAttribute();
	  break;
	case mvNodeAttributeType::mvNodeAttribute_Output: imnodes::EndOutputAttribute();
	  break;
	case mvNodeAttributeType::mvNodeAttribute_Static: imnodes::EndStaticAttribute();
	  break;
  }
  imnodes::PopAttributeFlag();

}

void mvNodeAttribute::setExtraConfigDict(PyObject *dict) {
  if (dict == nullptr) return;

  mvGlobalIntepreterLock gil;
  if (PyObject *item = PyDict_GetItemString(dict, "pin_shape")) m_pin_shape = ToInt(item);
  if (PyObject *item = PyDict_GetItemString(dict, "pin_color")) m_pin_color = ToColor(item);

}

void mvNodeAttribute::getExtraConfigDict(PyObject *dict) {
  if (dict == nullptr) return;

  mvGlobalIntepreterLock gil;
  PyDict_SetItemString(dict, "pin_shape", ToPyInt(m_pin_shape));
  PyDict_SetItemString(dict, "pin_color", ToPyColor(m_pin_color));

}
}