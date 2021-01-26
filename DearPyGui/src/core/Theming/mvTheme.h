#pragma once

#include <unordered_map>
#include "mvAppItem.h"

namespace Marvel {

	class mvTheme : public mvEventHandler
	{

	public:

		mvTheme();
		~mvTheme();

	private:
		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);

	};

	class mvImGuiThemeScope
	{
	public:
		mvImGuiThemeScope(mvAppItem* item);
		~mvImGuiThemeScope();

	private:

		int libIDCount = 0;
	};

}