#include "mvAppItemTheme.h"

namespace Marvel {

	mvAppItemTheme::mvAppItemTheme(int itemCode)
		:m_itemCode(itemCode)
	{
		mvEventBus::Subscribe(this, SID(std::to_string(itemCode).c_str()));
	};

	std::unordered_map<int, mvColor>& mvAppItemTheme::getColors()
	{ 
		return m_colors; 
	}


	bool mvAppItemTheme::onEvent(mvEvent& event) 
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvAppItemTheme::add_color), SID(std::to_string(m_itemCode).c_str()));
		return event.handled;
	};


	bool mvAppItemTheme::add_color(mvEvent& event)
	{
		m_colors[GetEInt(event, "ID") % 100] = GetEColor(event, "COLOR");
		return false;
	}

}