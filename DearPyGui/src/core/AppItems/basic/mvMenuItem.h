#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	class mvMenuItem : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuItem, "add_menu_item")

		explicit mvMenuItem(const std::string& name);

		void draw() override;

		void setExtraConfigDict(PyObject* dict) override;

		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::string m_shortcut;
		bool        m_check = false;

	};

}