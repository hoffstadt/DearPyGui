#pragma once
#include "core/AppItems/mvTypeBases.h"
#include <Python.h>
#include <imnodes.h>
#include "mvEventHandler.h"

namespace Marvel {

struct mvLink {
  int id, start_attr, end_attr;
};

class mvNodeEditor : public mvAppItem {

 public:
  MV_APPITEM_TYPE(mvAppItemType::NodeEditor, "add_node_editor")
  explicit mvNodeEditor(const std::string &name,
						PyObject *linkCreateCallback,
						PyObject *linkDestroyCallback);

  void addLink(mvLink link);
  bool deleteLink(int link_id);
  void draw() override;
  void setExtraConfigDict(PyObject *dict) override;
  void getExtraConfigDict(PyObject *dict) override;
  std::vector<int> getSelectedNodes();
  std::vector<int> getSelectedLinks();
  std::vector<mvLink> *getLinks();
  bool are_attrs_linked(int first_attr, int second_attr);
  bool link_exists(int link_id, int start_attr, int end_attr);
 private:
  std::vector<mvLink> m_links = {};
  int m_number_nodes_selected = 0;
  int m_number_links_selected = 0;
  PyObject *m_callback_link_created;
  PyObject *m_callback_link_destroyed;

  // region Styling
  //  float m_grid_spacing = 32.f;
  //  mvColor m_grid_background = mvColor(40, 40, 50, 200);
  //  mvColor m_grid_line = mvColor(200, 200, 200, 40);
  //  mvColor m_box_selector = mvColor(61, 133, 224, 30);
  //  mvColor m_box_selector_outline = mvColor(61, 133, 224, 150);
  //  mvColor m_link_color = mvColor(50, 50, 50, 255);
  //  mvColor m_link_hovered = mvColor(100, 100, 100, 255);
  //  mvColor m_link_selected = mvColor(75, 75, 75, 255);
  //  float m_link_weight = 3.f;
  //  float m_link_segments = 0.1;
  //  float m_link_hover_distance = 10.f;
  //endregion

};

}