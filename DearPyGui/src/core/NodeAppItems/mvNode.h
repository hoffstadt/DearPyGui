#pragma once
#include "core/AppItems/mvTypeBases.h"
#include <Python.h>
#include <imnodes.h>

namespace Marvel {
class mvNode : public mvBoolPtrBase {
 public:
  MV_APPITEM_TYPE(mvAppItemType::Node, "add_node")

  explicit mvNode(const std::string &name);

  void draw() override;

  void setExtraConfigDict(PyObject *dict) override;

  void getExtraConfigDict(PyObject *dict) override;

 private:
  bool m_draggable = true;
  bool m_dirty_pos = true;
  float m_x_pos = 10;
  float m_y_pos = 10;
  bool m_auto_title_bar = true;
  float m_node_corner_rounding = 4.f;
  float m_node_padding_h = 8.f;
  float m_node_padding_v = 1.f;
  float m_node_border_weight = 1.f;
  mvColor m_node_background = mvColor(50, 50, 50, 255);
  mvColor m_node_outline = mvColor(100, 100, 100, 255);
  mvColor m_node_background_hovered = mvColor(75, 75, 75, 255);
  mvColor m_node_background_selected = mvColor(75, 75, 75, 255);

 protected:
  const std::string name;
  int m_node_id;
};

}