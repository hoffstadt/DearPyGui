#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeColorGroup.h"
#include "mvThemeStyleGroup.h"

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

		static std::vector<mvThemeColorGroup::mvThemeColor*>& GetColorsPtr() { return s_acolors; }
		static std::vector<mvThemeColorGroup::mvThemeColor*>& GetDisabledColorsPtr() { return s_adcolors; }
		static std::vector<mvThemeStyleGroup::mvThemeStyle*>& GetStylesPtr() { return s_astyles; }
		static std::vector<mvThemeColorGroup::mvThemeColor>& GetColors() { return s_colors; }
		static std::vector<mvThemeColorGroup::mvThemeColor>& GetDisabledColors() { return s_disabled_colors; }
		static std::vector<mvThemeStyleGroup::mvThemeStyle>& GetStyles() { return s_styles; }
		static const std::string& GetNameFromConstant(long constant);

		static std::vector<mvThemeColorGroup::mvThemeColor> GetColorsByType(mvAppItemType type, bool disabled);
		static std::vector<mvThemeStyleGroup::mvThemeStyle> GetStylesByType(mvAppItemType type);

	public:

		mvThemeManager();
		~mvThemeManager();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);
		bool add_disabled_color(mvEvent& event);
		bool add_style(mvEvent& event);

		// new
		static std::vector<mvThemeColorGroup::mvThemeColor> s_colors;
		static std::vector<mvThemeColorGroup::mvThemeColor*> s_acolors;

		static std::vector<mvThemeColorGroup::mvThemeColor> s_disabled_colors;
		static std::vector<mvThemeColorGroup::mvThemeColor*> s_adcolors;

		static std::vector<mvThemeStyleGroup::mvThemeStyle> s_styles;
		static std::vector<mvThemeStyleGroup::mvThemeStyle*> s_astyles;

	};

	template<typename T>
	void SearchAncestorsForColors(T* item, bool disabled = false)
	{
		static const std::vector<std::tuple<std::string, long, mvColor, mvColor>>& color_constants = T::GetColorConstants();

		// get current item's color group and check cache
		mvThemeColorGroup& colorGroup = disabled ? item->getDisabledColorGroup() : item->getColorGroup();
		if (colorGroup.isCacheValid())
		{
			if (!disabled)
				SearchAncestorsForColors<T>(item, true);
			return;
		}

		// keep up with whether colors are found or not
		std::unordered_map<long, bool> colors_found;
		for (const auto& color_pair : color_constants)
			colors_found[std::get<1>(color_pair)] = false;

		// get local colors
		const auto& localColorsForType = colorGroup.getColorsByType(item->getType());

		// register local colors as found
		for (const auto& color : localColorsForType)
		{
			if (colors_found.count(color.constant) > 0)
			{
				colors_found[color.constant] = true;
				colorGroup.addColor(color);
			}
		}

		// check ancestors for remaining colors
		mvAppItem* widget = item;
		while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
		{
			widget = widget->getParent();

			const auto& ancestorColorGroup = disabled ? widget->getDisabledColorGroup() : widget->getColorGroup();

			const auto& ancestorColorsForType = ancestorColorGroup.getColorsByType(item->getType());

			for (const auto& color : ancestorColorsForType)
			{
				// only update color if it wasn't found yet
				if (!colors_found[color.constant])
				{
					colors_found[color.constant] = true;
					colorGroup.addColor(color);
				}
			}
		}

		// if any colors were not found, check theme manager
		for (auto& color : mvThemeManager::GetColorsByType(item->getType(), disabled))
		{
			// only apply if it wasn't found yet
			if (!colors_found[color.constant])
			{
				colors_found[color.constant] = true;
				colorGroup.addColor(color);
			}
		}

		colorGroup.setCacheValid();

		// ensure all colors were handled
		for (const auto& color : colors_found)
		{
			if (!color.second)
			{
				assert(false && "not all colors were found");
				break;
			}
		}
	}

	template<typename T>
	void SearchAncestorsForStyles(T* item)
	{

		static const std::vector<std::tuple<std::string, long, float, float>>& style_constants = T::GetStyleConstants();

		// get current item's style group and check cache
		mvThemeStyleGroup& styleGroup = item->getStyleGroup();
		if (styleGroup.isCacheValid())
			return;

		// keep up with whether styles are found or not
		static int styleID = 0;
		std::unordered_map<long, bool> styles_found;
		for (const auto& style_pair : style_constants)
			styles_found[std::get<1>(style_pair)] = false;

		// get local styles
		const auto& localStylesForType = styleGroup.getStylesByType(item->getType());

		// register local styles as found
		for (const auto& style : localStylesForType)
		{
			if (styles_found.count(style.constant) > 0)
			{
				styles_found[style.constant] = true;
				styleGroup.addStyle(style);
			}
		}

		// check ancestors for remaining styles
		mvAppItem* widget = item;
		while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
		{
			widget = widget->getParent();

			const auto& ancestorStyleGroup = widget->getStyleGroup();
			const auto& ancestorStylesForType = ancestorStyleGroup.getStylesByType(item->getType());

			for (const auto& style : ancestorStylesForType)
			{
				// only update style if it wasn't found yet
				if (!styles_found[style.constant])
				{
					styles_found[style.constant] = true;
					styleGroup.addStyle(style);
				}
			}
		}

		// if any styles were not found, check theme manager
		for (auto& style : mvThemeManager::GetStylesByType(item->getType()))
		{
			// only apply if it wasn't found yet
			if (!styles_found[style.constant])
			{
				styles_found[style.constant] = true;
				styleGroup.addStyle(style);
			}
		}

		styleGroup.setCacheValid();

		// ensure all styles were handled
		for (const auto& style : styles_found)
		{
			if (!style.second)
			{
				assert(false && "not all styles were found");
				break;
			}
		}

	}

}