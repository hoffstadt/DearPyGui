#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Button, "add_button")

		mvButton(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool     m_small = false;
		bool     m_arrow = false;
		ImGuiDir m_direction = ImGuiDir_Up;

	};

}
