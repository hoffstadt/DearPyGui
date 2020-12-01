#pragma once
#include "core/AppItems/mvTypeBases.h"
#include "mvCore.h"
#include <imnodes.h>

namespace Marvel {
enum class mvNodeAttributeType {
  mvNodeAttribute_Input = 0, mvNodeAttribute_Output, mvNodeAttribute_Static,
};
class mvNodeAttribute : public mvStringPtrBase {
 public:
  MV_APPITEM_TYPE(mvAppItemType::NodeAttribute, "add_node_attribute")
  mvNodeAttribute(const std::string &name, int id, mvNodeAttributeType a_type, const std::string &default_value);
  void draw() override;
  void setExtraConfigDict(PyObject *dict) override;
  void getExtraConfigDict(PyObject *dict) override;

 private:
  int m_pin_shape = 1;
  mvColor m_pin_color = mvColor(53, 150, 250, 255);

  //region Styling
  //  float m_pin_radius = 4.f;
  //  float m_pin_quad_side_length = 7.f;
  //  float m_pin_triangle_side_length = 9.5;
  //  float m_pin_line_thickness = 1.f;
  //  float m_pin_hover_radius = 10.f;
  //  float m_pin_offset = 0.f;
  //  mvColor m_pin_hovered = mvColor(53, 150, 250, 180);
  //endregion
 protected:
  int m_attr_id;
  mvNodeAttributeType m_attr_type;
};

}