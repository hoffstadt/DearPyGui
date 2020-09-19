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
//     * mvDatePicker
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvDatePicker : public mvTimeItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DatePicker)

			mvDatePicker(const std::string& name, tm default_value, int level)
			: mvTimeItemBase(name, default_value), m_level(level)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (ImPlot::ShowDatePicker(m_name.c_str(), &m_level, &m_imvalue, &m_imvalue))
			{
				ImPlot::GetGmtTime(m_imvalue, &m_value);
				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);
			}


			popColorStyles();
		}

	private:

		int m_level = 0;


	};

}