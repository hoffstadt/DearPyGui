#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "composite/mvStyleWindow.h"
#include "mvThemeManager.h"
#include <imnodes.h>

namespace Marvel {

	template<typename T>
	class mvImNodesThemeScope
	{

	public:

		mvImNodesThemeScope(T* item)
		{

			// this is messy and disgusting. Needs to be cleaned up and optimized

			const std::vector<std::tuple<std::string, long, mvColor>>& color_constants = T::GetColorConstants();
			const std::vector<std::tuple<std::string, long, float, float>>& style_constants = T::GetStyleConstants();

			mvThemeColors colors;
			std::unordered_map<long, bool> colors_found;
			for (const auto& color_pair : color_constants)
				colors_found[std::get<1>(color_pair)] = false;

			//style is were we place all zero first position floats
			std::unordered_map<imnodes::StyleVar, float> styles;
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
					styles[(imnodes::StyleVar)styleID] = style.second;
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
							styles[(imnodes::StyleVar)styleID] = style.second;
						}
					}
				}
			}

			for (auto& color : mvThemeManager::GetColors()[item->getType()])
			{
				// only apply if it wasn't found yet
				if (!colors_found[color.first])
				{
					colors[color.first] = color.second;
					colors_found[color.first] = true;
				}
			}
			for (auto& style : mvThemeManager::GetStyles()[item->getType()])
			{
				// only apply if it wasn't found yet
				if (!styles_found[style.first])
				{
					styles_found[style.first] = true;
					mvThemeManager::decodelibID(style.first, &styleID);
					styles[(imnodes::StyleVar)styleID] = style.second;
				}
			}

			libIDCount = colors.size();
			static int imColorID;
			for (auto& color : colors)
			{
				mvThemeManager::decodelibID(color.first, &imColorID);
				imnodes::PushColorStyle((imnodes::ColorStyle)imColorID, color.second);
			}

			StyleIDCount = styles.size();

			for (const auto& style : styles)
				imnodes::PushStyleVar(style.first, style.second);
		}

		~mvImNodesThemeScope()
		{
			cleanup();
		}

		void cleanup()
		{
			while (libIDCount > 0)
			{
				imnodes::PopColorStyle();
				--libIDCount;
			}
			while (StyleIDCount > 0)
			{
				imnodes::PopStyleVar();
				--StyleIDCount;
			}
		}

	private:
		int libIDCount = 0;
		int StyleIDCount = 0;

	};
}