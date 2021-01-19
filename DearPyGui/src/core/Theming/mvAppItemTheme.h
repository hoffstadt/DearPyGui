#pragma once

#include "mvEvents.h"
#include <unordered_map> 

namespace Marvel {

	//required forward declaration because we might need to move app item type somewhere else
	enum class mvAppItemType;
	typedef std::unordered_map<int, mvColor> themeColors;

	class mvAppItemTheme
	{

	public:

		std::unordered_map<mvAppItemType, themeColors>& getColors() {return m_colors;}

	private:

		//every item has the potential to hold an entire theme 
		//but will only hold colors given to it by the 
		//theme manager, only containers will get more than their colors
		std::unordered_map<mvAppItemType, themeColors> m_colors;

	};
}