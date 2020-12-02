#pragma once
#include "core/AppItems/mvTypeBases.h"
#include "mvCore.h"
#include <Python.h>
#include <imnodes.h>

namespace Marvel {
class mvNodeTitleBar : public mvAppItem {
 public:
  MV_APPITEM_TYPE(mvAppItemType::NodeTitleBar, "add_node_title_bar")

  explicit mvNodeTitleBar(const std::string &name);

  void draw() override;

  void setExtraConfigDict(PyObject *dict) override;

  void getExtraConfigDict(PyObject *dict) override;

 private:
  mvColor m_title_bar_color = mvColor(41, 74, 122, 255);
  mvColor m_title_bar_hovered = mvColor(66, 150, 250, 255);
  mvColor m_title_bar_selected = mvColor(66, 150, 250, 255);
};
}