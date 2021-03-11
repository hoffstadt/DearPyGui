#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "composite/mvStyleWindow.h"

namespace Marvel {

	class mvThemeManager : public mvEventHandler
	{

	public:

		static void decodeType(long encoded_constant, mvAppItemType* type);
		static void decodelibID(long encoded_constant, int* libID);
		static int decodeIndex(long encoded_constant);

		static std::vector<std::tuple<std::string, long, mvColor*>>& GetColorsPtr() { return s_acolors; }
		static std::vector<std::tuple<std::string, long, float*, float>>& GetStylesPtr() { return s_astyles; }
		static std::unordered_map<mvAppItemType, mvThemeColors>& GetColors() { return s_colors; }
		static std::unordered_map<mvAppItemType, mvThemeStyles>& GetStyles() { return s_styles; }

	public:

		mvThemeManager();
		~mvThemeManager();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);
		bool add_style(mvEvent& event);

		static std::vector<std::tuple<std::string, long, mvColor*>> s_acolors;
		static std::vector<std::tuple<std::string, long, float*, float>>   s_astyles;
		static std::unordered_map<mvAppItemType, mvThemeColors>     s_colors;
		static std::unordered_map<mvAppItemType, mvThemeStyles>     s_styles;

	};

	template<typename T>
	class mvImGuiThemeScope
	{
	public:

		
		mvImGuiThemeScope(T* item)
		{
			const std::vector<std::tuple<std::string, long, mvColor>>& color_constants = T::GetColorConstants();
			const std::vector<std::tuple<std::string, long, int, int>>& style_constants = T::GetStyleConstants();

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
				ImGui::PushStyleVar(style.first, style.second);
		}

		~mvImGuiThemeScope()
		{
			cleanup();
		}

		void cleanup()
		{
			if (libIDCount > 0)
				ImGui::PopStyleColor(libIDCount);
			libIDCount = 0;
			if (StyleIDCount > 0)
				ImGui::PopStyleVar(StyleIDCount);
			StyleIDCount = 0;
		}

	private:
		int libIDCount = 0;
		int StyleIDCount = 0;

	};

}