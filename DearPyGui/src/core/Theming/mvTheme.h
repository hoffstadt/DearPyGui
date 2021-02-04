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
		bool add_style(mvEvent& event);

	};

	class mvImGuiThemeScope
	{
	public:
		mvImGuiThemeScope(mvAppItem* item);
		~mvImGuiThemeScope();

		void cleanup();

	private:
		int libIDCount = 0;
		int StyleIDCount = 0;
	};

}