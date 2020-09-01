#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvStyleWindow : public mvStandardWindow
	{

	public:

		mvStyleWindow() : mvStandardWindow("Dear PyGui Style Editor") {}

		void render(bool& show) override;

	};

}