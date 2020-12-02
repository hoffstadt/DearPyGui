#pragma once

#include "core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDatePicker
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvDatePicker : public mvPtrBase<tm, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DatePicker, "add_date_picker")

		mvDatePicker(const std::string& name, tm default_value);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:
		ImPlotTime* m_imvalue = nullptr;
		int m_level = 0;

	};

}