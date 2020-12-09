#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvMenu : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu, "add_menu")

		mvMenu(const std::string& name);

		void draw() override;

		void setExtraConfigDict(PyObject* dict) override;

		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool m_enabled = true;

	};

}