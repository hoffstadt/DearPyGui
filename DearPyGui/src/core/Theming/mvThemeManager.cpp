#include "mvThemeManager.h"
#include "imgui.h"
#include "mvAppItems.h"
#include "mvCore.h"
#include "mvItemRegistry.h"

namespace Marvel {

	std::vector<std::tuple<std::string, long, mvColor*>> mvThemeManager::s_acolors;
	std::vector<std::tuple<std::string, long, float*>>   mvThemeManager::s_astyles;
	std::unordered_map<mvAppItemType, mvThemeColors>     mvThemeManager::s_colors;
	std::unordered_map<mvAppItemType, mvThemeStyles>     mvThemeManager::s_styles;

	void mvThemeManager::decodeType(long encoded_constant, mvAppItemType* type)
	{
		*type = (mvAppItemType)(encoded_constant / 1000);
	}
	void mvThemeManager::decodelibID(long encoded_constant, int* libID)
	{
		*libID = (int)((encoded_constant / 10) % 100);
	}
	int mvThemeManager::decodeIndex(long encoded_constant)
	{
		return (int)(encoded_constant % 10);
	}

	mvThemeManager::mvThemeManager()
	{
		mvEventBus::Subscribe(this, SID("color_change"), mvEVT_CATEGORY_THEMES);
		mvEventBus::Subscribe(this, SID("style_change"), mvEVT_CATEGORY_THEMES);
	}

	mvThemeManager::~mvThemeManager()
	{
		mvEventBus::UnSubscribe(this);
	}
 
	bool mvThemeManager::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvThemeManager::add_style), SID("style_change"));
		dispatcher.dispatch(BIND_EVENT_METH(mvThemeManager::add_color), SID("color_change"));
		return event.handled;
	};

	bool mvThemeManager::add_color(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		decodeType(mvThemeConstant, &type);
		mvColor color = GetEColor(event, "COLOR");
		const std::string& widget = GetEString(event, "WIDGET");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			GetColors()[type][mvThemeConstant] = color;
			return true;
		}

		//check widget can take color and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
			item->getColors()[type][mvThemeConstant] = color;
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
			{
				ThrowPythonException("Item type does not except this color constant.");
			});
		}
		return true;
	}

	bool mvThemeManager::add_style(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		decodeType(mvThemeConstant, &type);
		float style = GetEFloat(event, "STYLE");
		const std::string& widget = GetEString(event, "WIDGET");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			GetStyles()[type][mvThemeConstant] = style;
			return true;
		}

		//check widget can take style and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
			item->getStyles()[type][mvThemeConstant] = style;
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
			{
				ThrowPythonException("Item type does not except this style constant.");
			});
		}
		return true;
	}

}