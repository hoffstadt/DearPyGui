#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeColorGroup.h"

namespace Marvel {

	class mvThemeManager : public mvEventHandler
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_CREATE_EXTRA_COMMAND(set_theme_color);
		MV_CREATE_EXTRA_COMMAND(set_theme_color_disabled);
		MV_CREATE_EXTRA_COMMAND(set_theme_style);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(set_theme_color);
			MV_ADD_EXTRA_COMMAND(set_theme_color_disabled);
			MV_ADD_EXTRA_COMMAND(set_theme_style);
		MV_END_EXTRA_COMMANDS

		static void InValidateColorTheme();
		static void InValidateDisabledColorTheme();
		static void InValidateStyleTheme();

		static std::vector<std::tuple<std::string, long, mvColor*>>& GetColorsPtr() { return s_acolors; }
		static std::vector<std::tuple<std::string, long, mvColor*>>& GetDisabledColorsPtr() { return s_adcolors; }
		static std::vector<std::tuple<std::string, long, float*, float>>& GetStylesPtr() { return s_astyles; }
		static std::unordered_map<mvAppItemType, mvThemeColors>& GetColors() { return s_colors; }
		static std::unordered_map<mvAppItemType, mvThemeColors>& GetDisabledColors() { return s_disabled_colors; }
		static std::unordered_map<mvAppItemType, mvThemeStyles>& GetStyles() { return s_styles; }

	public:

		mvThemeManager();
		~mvThemeManager();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);
		bool add_disabled_color(mvEvent& event);
		bool add_style(mvEvent& event);

		static std::vector<std::tuple<std::string, long, mvColor*>> s_acolors;
		static std::vector<std::tuple<std::string, long, mvColor*>> s_adcolors;
		static std::vector<std::tuple<std::string, long, float*, float>>      s_astyles;
		static std::unordered_map<mvAppItemType, mvThemeColors>               s_colors;
		static std::unordered_map<mvAppItemType, mvThemeColors>               s_disabled_colors;
		static std::unordered_map<mvAppItemType, mvThemeStyles>               s_styles;

	};

	template<typename T>
	void SearchAncestorTreeForStyles(T* item, std::unordered_map<ImGuiStyleVar, float>& styles, std::unordered_map<ImGuiStyleVar, float>& styles1, std::unordered_map<ImGuiStyleVar, float>& styles2)
	{

		const std::vector<std::tuple<std::string, long, float, float>>& style_constants = T::GetStyleConstants();

		// style is were we place all zero first position floats
		// style2 is where we place a const that may have a second position for pushing ImVec2 constants
		static int styleID;
		std::unordered_map<long, bool> styles_found;
		for (const auto& style_pair : style_constants)
			styles_found[std::get<1>(style_pair)] = false;

		if (item->getStyles().find(item->getType()) != item->getStyles().end())
		{
			for (const auto& style : item->getStyles()[item->getType()])
			{
				styles_found[style.first] = true;
				DecodelibID(style.first, &styleID);
				if (DecodeIndex(style.first) > 0)
					styles2[styleID] = style.second;
				else
					styles1[styleID] = style.second;
			}
		}

		// search through ancestor tree for unfound styles
		mvAppItem* widget = item;
		while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
		{
			widget = widget->getParent();

			if (widget->getStyles().find(item->getType()) != widget->getStyles().end())
			{
				for (auto& style : widget->getStyles()[item->getType()])
				{
					// only apply if it wasn't found yet
					if (!styles_found[style.first])
					{
						styles_found[style.first] = true;
						DecodelibID(style.first, &styleID);
						if (DecodeIndex(style.first) > 0)
							styles2[styleID] = style.second;
						else
							styles1[styleID] = style.second;
					}
				}
			}
		}

		for (auto& style : mvThemeManager::GetStyles()[item->getType()])
		{
			// only apply if it wasn't found yet
			if (!styles_found[style.first])
			{
				styles_found[style.first] = true;
				DecodelibID(style.first, &styleID);
				if (DecodeIndex(style.first) > 0)
					styles2[styleID] = style.second;
				else
					styles1[styleID] = style.second;
			}
		}

		styles = styles1;

		for (const auto& style : styles2)
			styles.erase(style.first);
	}

}