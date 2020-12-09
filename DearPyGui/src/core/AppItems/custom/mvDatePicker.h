#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvDatePicker : public mvTimePtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DatePicker, "add_date_picker")

		mvDatePicker(const std::string& name, tm default_value);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		int m_level = 0;

	};

}