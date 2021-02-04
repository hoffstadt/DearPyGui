#include "mvTheme.h"
#include "imgui.h"

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

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			mvApp::GetApp()->getColors()[type][libID] = color;
			return true;
		}

		//check widget can take color and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
		{
			item->getColors()[type][libID] = color;
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					ThrowPythonException("Item does not except this theme constant.");
				});
		}
		return true;
	}

	bool mvTheme::add_style(mvEvent& event)
	{
		mvAppItemType type = (mvAppItemType)(GetEInt(event, "ID") / 100);
		int libID = GetEInt(event, "ID") % 100;
		float style = GetEFloat(event, "STYLE");
		const std::string& widget = GetEString(event, "WIDGET");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			mvApp::GetApp()->getStyles()[type][libID] = style;
			return true;
		}

		//check widget can take style and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
		{
			item->getStyles()[type][libID] = style;
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					ThrowPythonException("Item does not except this theme constant.");
				});
		}
		return true;
	}

	mvImGuiThemeScope::mvImGuiThemeScope(mvAppItem* item)
	{
		int pushedIDs[ImGuiCol_COUNT];
		//this goes through the specific colors for the current item type and applies them
		for (auto& themeColor : item->getColors()[item->getType()])
		{
			if (themeColor.first < ImGuiCol_COUNT)
			{
				ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
				pushedIDs[libIDCount] = themeColor.first;
				libIDCount++;
			}
		}
		for (auto& themeStyle : item->getStyles()[item->getType()])
		{
			if (themeStyle.first < ImGuiStyleVar_COUNT)
			{
				ImGui::PushStyleVar((ImGuiStyleVar)themeStyle.first, themeStyle.second);
				pushedIDs[libIDCount] = themeStyle.first;
				libIDCount++;
			}
		}
		//this goes through the specific colors for the current item type and applies them
		mvAppItem* widget = item;
		while (!widget->getDescription().root)
		{
			widget = widget->getParent();
			for (auto& themeColor : widget->getColors()[item->getType()])
			{
				//checking through all the libID's to see if our id matches one
				int i = 0;
				while (i < libIDCount)
					if (pushedIDs[i] == themeColor.first)
						break;
					else
						i++;
				//if our position is the equal to the libIDCount then we did not find the current libID 
				//so push libID and colo to DIG
				if (i == libIDCount)
				{
					if (themeColor.first < ImGuiCol_COUNT)
					{
						ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
						pushedIDs[libIDCount] = themeColor.first;
						libIDCount++;
					}
				}
			}
		}

		for (auto& themeColor : mvApp::GetApp()->getColors()[item->getType()])
		{
			//checking through all the libID's to see if our id matches one
			int i = 0;
			while (i < libIDCount)
				if (pushedIDs[i] == themeColor.first)
					break;
				else
					i++;
			//if our position is the equal to the libIDCount then we did not find the current libID 
			//so push libID and colo to DIG
			if (i == libIDCount)
			{
				if (themeColor.first < ImGuiCol_COUNT)
				{
					ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
					pushedIDs[libIDCount] = themeColor.first;
					libIDCount++;
				}
			}
		}
	}

	mvImGuiThemeScope::~mvImGuiThemeScope()
	{
		cleanup();
	}

	void mvImGuiThemeScope::cleanup()
	{
		if (libIDCount > 0)
			ImGui::PopStyleColor(libIDCount);
		libIDCount = 0;
	}

}