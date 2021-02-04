#include "mvTheme.h"
#include "imgui.h"

namespace Marvel {

	int mvTheme::GetLibraryConstant(int encoded_constant, mvAppItemType* out_type)
	{
		if(out_type)
			*out_type = (mvAppItemType)(encoded_constant / 100);
		return encoded_constant % 100;
	}

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
		mvAppItemType type;
		int libID = GetLibraryConstant(GetEInt(event, "ID"), &type);

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
			item->getColors()[type][libID] = color;
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

	mvNewImGuiThemeScope::mvNewImGuiThemeScope(mvAppItem* item)
	{

		const std::vector<std::pair<std::string, long>>& color_constants = item->getColorConstants();

		std::unordered_map<long, mvColor> colors;
		std::unordered_map<long, bool> colors_found;
		for (const auto& color_pair : color_constants)
			colors_found[color_pair.second] = false;

		// check local colors first
		if (item->getColors().find(item->getType()) != item->getColors().end())
		{
			for (const auto& color : item->getColors()[item->getType()])
			{
				colors_found[color.first] = true;
				colors[color.first] = color.second;
			}
		}

		// search through ancestor tree for unfound colors
		mvAppItem* widget = item;
		while (!widget->getDescription().root)
		{
			widget = widget->getParent();

			if (widget->getColors().find(item->getType()) != widget->getColors().end())
			{
				for (const auto& color : widget->getColors()[item->getType()])
				{
					// only apply if it wasn't found yet
					if(!colors_found[color.first])
						colors[color.first] = color.second;

					colors_found[color.first] = true;
				}
			}
		}

		libIDCount = colors.size();
		for (const auto& color : colors)
			ImGui::PushStyleColor(color.first, color.second.toVec4());

	}

	mvNewImGuiThemeScope::~mvNewImGuiThemeScope()
	{
		cleanup();
	}

	void mvNewImGuiThemeScope::cleanup()
	{
		if (libIDCount > 0)
			ImGui::PopStyleColor(libIDCount);
		libIDCount = 0;
	}

}