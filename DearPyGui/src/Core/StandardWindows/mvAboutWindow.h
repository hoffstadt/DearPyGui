#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvAboutWindow : public mvStandardWindow
	{

	public:

		virtual void render(bool& show) override;

	};

}