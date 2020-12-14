#pragma once

#include <unordered_map>
#include "mvAppItem.h"
#include "mvAppItemTheme.h"

namespace Marvel {

	class mvTheme
	{

	public:

		mvTheme();

		mvAppItemTheme* getThemeItem(mvAppItemType type);


	private:

		std::unordered_map<mvAppItemType, mvAppItemTheme> m_themeItems;

	};

}