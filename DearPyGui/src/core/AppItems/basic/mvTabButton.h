#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvTabButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabButton, "add_tab_button")

		mvTabButton(const std::string& name);

		void draw()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;
	};
}