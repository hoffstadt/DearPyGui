#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvCombo : public mvStringPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Combo, "add_combo")

		mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;
		std::vector<std::string> m_items;
	};

}
