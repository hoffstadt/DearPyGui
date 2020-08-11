#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvAboutWindow : public mvStandardWindow
	{

	public:

		void render(bool& show) override;

	};

}