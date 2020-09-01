#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvMetricsWindow : public mvStandardWindow
	{

	public:

		mvMetricsWindow() : mvStandardWindow("Dear PyGui Metrics") 
		{
			m_flags = ImGuiWindowFlags_AlwaysAutoResize;
		}

		void render(bool& show) override;

	};

}