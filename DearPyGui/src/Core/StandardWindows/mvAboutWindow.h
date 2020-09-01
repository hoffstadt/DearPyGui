#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvAboutWindow : public mvStandardWindow
	{

	public:

		mvAboutWindow();

		void render(bool& show) override;

	};

}