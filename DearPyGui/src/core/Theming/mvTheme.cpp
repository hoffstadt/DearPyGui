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

		//this needs to fill out the root or primary window
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

	mvImGuiThemeScope::mvImGuiThemeScope(mvAppItem* item)
	{
		int pushedIDs[ImGuiCol_COUNT];
		//this goes through the specific colors for the current item type and applies them
		for (auto& themeColor : item->getColors()[item->getType()])
		{
			ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
			pushedIDs[libIDCount] = themeColor.first;
			libIDCount++;
		}
		//this goes through the specific colors for the current item type and applies them
		mvAppItem* widget = item;
		while (!widget->getDescription().root)
		{
			widget = widget->getParent();
			for (auto& themeColor : widget->getColors()[item->getType()])
			{
				//checking if libID has been used
				int i = 0;
				while (i < libIDCount)
					if (pushedIDs[i] == themeColor.first)
						break;
					else
						i++;
				//adds libID if it has not been found
				if (i == libIDCount)
				{
					ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
					pushedIDs[libIDCount] = themeColor.first;
					libIDCount++;
				}
			}
		}

		for (auto& themeColor : mvApp::GetApp()->getColors()[item->getType()])
		{
			//checking if libID has been used
			int i = 0;
			while (i < libIDCount)
				if (pushedIDs[i] == themeColor.first)
					break;
				else
					i++;
			//adds libID if it has not been found
			if (i == libIDCount)
			{
				ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
				pushedIDs[libIDCount] = themeColor.first;
				libIDCount++;
			}
		}
	}

	mvImGuiThemeScope::~mvImGuiThemeScope()
	{
		if (libIDCount > 0)
			ImGui::PopStyleColor(libIDCount);
	}

}