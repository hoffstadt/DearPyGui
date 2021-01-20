#include "mvTheme.h"

namespace Marvel {

	mvTheme::mvTheme()
	{
		mvEventBus::Subscribe(this, SID("color_change"), mvEVT_CATEGORY_THEMES);
	}

	mvTheme::~mvTheme()
	{
		mvEventBus::UnSubscribe(this);
	}
 
	bool mvTheme::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvTheme::add_color), SID("color_change"), mvEVT_CATEGORY_THEMES);
		return event.handled;
	};

	bool mvTheme::add_color(mvEvent& event)
	{
		mvAppItemType type = (mvAppItemType)(GetEInt(event, "ID") / 100);
		int libID = GetEInt(event, "ID") % 100;
		mvColor color = GetEColor(event, "COLOR");
		const std::string& widget = GetEString(event, "WIDGET");

		//this needs to fill out the root or primary window
		if (widget.empty())
		{
			widget == mvApp::GetApp()->getItemRegistry().getWindows()[0];
		}

		//check widget can take color and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
		{
			item->getColors()[type][libID] = color;
		}
		else ThrowPythonException("Item does not except this theme constant.");
		return true;
	}

}