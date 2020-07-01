#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvMetricsWindow : public mvStandardWindow
	{

	public:

		virtual void render(bool& show) override;

	};

}