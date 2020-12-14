#include "mvTheme.h"
#include "mvAppItemTheme.h"

namespace Marvel {

	mvTheme::mvTheme()
	{
		for (int i = 1; i < (int)mvAppItemType::ItemTypeCount; i++)
			m_themeItems[(mvAppItemType)i] = mvAppItemTheme(i);
	}

	mvAppItemTheme* mvTheme::getThemeItem(mvAppItemType type)
	{
		return &m_themeItems.at(type);
	}

}