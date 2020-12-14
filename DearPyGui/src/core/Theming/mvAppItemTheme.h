#pragma once

#include "mvEvents.h"
#include <unordered_map> 

namespace Marvel {

	class mvAppItemTheme : public mvEventHandler
	{

	public:

		mvAppItemTheme() = default;
		~mvAppItemTheme();

		mvAppItemTheme(int code);

		std::unordered_map<int, mvColor>& getColors();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);

	private:

		int m_itemCode = 0;
		std::unordered_map<int, mvColor> m_colors;

	};
}