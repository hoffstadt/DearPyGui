#pragma once

#include "mvApp.h"
#include "Core/AppItems/mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvPythonTranslator.h"


//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTimePicker
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvTimePicker : public mvTimeItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TimePicker, "add_time_picker")

		mvTimePicker(const std::string& name, tm default_value, bool hour24)
			: mvTimeItemBase(name, default_value), m_hour24(hour24)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImPlot::ShowTimePicker(m_name.c_str(), &m_imvalue, m_hour24))
			{
				ImPlot::GetGmtTime(m_imvalue, &m_value);
				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);
			}


			popColorStyles();
		}

	private:

		bool m_hour24;

	};

}