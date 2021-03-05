#include "mvThemeManager.h"
#include "imgui.h"
#include "mvAppItem.h"
#include "mvCore.h"
#include "mvItemRegistry.h"

namespace Marvel {

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
			mvApp::GetApp()->getColors()[type][mvThemeConstant] = color;
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
			mvApp::GetApp()->getStyles()[type][mvThemeConstant] = style;
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

	mvImGuiThemeScope::mvImGuiThemeScope(mvAppItem* item)
	{

		const std::vector<std::tuple<std::string, long, mvColor>>& color_constants = item->getColorConstants();
		const std::vector<std::tuple<std::string, long, int, int>>& style_constants = item->getStyleConstants();

		mvThemeColors colors;
		std::unordered_map<long, bool> colors_found;
		for (const auto& color_pair : color_constants)
			colors_found[std::get<1>(color_pair)] = false;

		//style is were we place all zero first position floats
		//style2 is where we place a const that may have a second position for pushing ImVec2 constants
		std::unordered_map<ImGuiStyleVar, float> styles;
		std::unordered_map<ImGuiStyleVar, float> styles2;
		static int styleID;
		std::unordered_map<long, bool> styles_found;
		for (const auto& style_pair : style_constants)
			styles_found[std::get<1>(style_pair)] = false;

		// check local colors first
		if (item->getColors().find(item->getType()) != item->getColors().end())
		{
			for (const auto& color : item->getColors()[item->getType()])
			{
				colors_found[color.first] = true;
				colors[color.first] = color.second;
			}
		}
		if (item->getStyles().find(item->getType()) != item->getStyles().end())
		{
			for (const auto& style : item->getStyles()[item->getType()])
			{
				styles_found[style.first] = true;
				mvThemeManager::decodelibID(style.first, &styleID);
				if (mvThemeManager::decodeIndex(style.first) > 0)
					styles2[styleID] = style.second;
				else
					styles[styleID] = style.second;
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
					if (!colors_found[color.first])
					{
						colors[color.first] = color.second;
						colors_found[color.first] = true;
					}
				}
			}
			if (widget->getStyles().find(item->getType()) != widget->getStyles().end())
			{
				for (auto& style : widget->getStyles()[item->getType()])
				{
					// only apply if it wasn't found yet
					if (!styles_found[style.first])
					{
						styles_found[style.first] = true;
						mvThemeManager::decodelibID(style.first, &styleID);
						if (mvThemeManager::decodeIndex(style.first) > 0)
							styles2[styleID] = style.second;
						else
							styles[styleID] = style.second;
					}
				}
			}
		}

		for (auto& color : mvApp::GetApp()->getColors()[item->getType()])
		{
			// only apply if it wasn't found yet
			if (!colors_found[color.first])
			{
				colors[color.first] = color.second;
				colors_found[color.first] = true;
			}
		}
		for (auto& style : mvApp::GetApp()->getStyles()[item->getType()])
		{
			// only apply if it wasn't found yet
			if (!styles_found[style.first])
			{
				styles_found[style.first] = true;
				mvThemeManager::decodelibID(style.first, &styleID);
				if (mvThemeManager::decodeIndex(style.first) > 0)
					styles2[styleID] = style.second;
				else
					styles[styleID] = style.second;
			}
		}

		libIDCount = colors.size();
		static ImGuiCol imColorID;
		for (const auto& color : colors) 
		{
			mvThemeManager::decodelibID(color.first, &imColorID);
			ImGui::PushStyleColor(imColorID, color.second.toVec4());
		}

		StyleIDCount = styles.size();
		for (const auto& style : styles2)
		{
			ImGui::PushStyleVar(style.first, { styles[style.first], styles2[style.first] });
			styles.erase(style.first);
		}
		for (const auto& style : styles)
		{
			ImGui::PushStyleVar(style.first, style.second);
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
		if (StyleIDCount > 0)
			ImGui::PopStyleVar(StyleIDCount);
		StyleIDCount = 0;
	}

}