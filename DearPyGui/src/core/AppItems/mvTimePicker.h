#pragma once

#include "core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTimePicker
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvTimePicker : public mvPtrBase<tm, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TimePicker, "add_time_picker")

		mvTimePicker(const std::string& name, tm default_value);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:
		ImPlotTime* m_imvalue = nullptr;
		bool m_hour24 = false;

	};

}