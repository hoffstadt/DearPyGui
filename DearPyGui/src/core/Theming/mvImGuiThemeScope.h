#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "composite/mvStyleWindow.h"
#include "mvThemeManager.h"

namespace Marvel {

	template<typename T>
	class mvImGuiThemeScope
	{

	public:

		mvImGuiThemeScope(T* item)
		{

			mvThemeColors& colors = item->getCachedThemeColors();
			std::unordered_map<ImGuiStyleVar, float>& styles = item->getCachedThemeStyles();
			std::unordered_map<ImGuiStyleVar, float>& styles1 = item->getCachedThemeStyles1();
			std::unordered_map<ImGuiStyleVar, float>& styles2 = item->getCachedThemeStyles2();

			if (!item->isThemeColorCacheValid())
			{
				SearchAncestorTreeForColors<T>(item, colors);
				item->setThemeColorCacheValid();
			}

			if (!item->isThemeStyleCacheValid())
			{
				SearchAncestorTreeForStyles<T>(item, styles, styles1, styles2);
				item->setThemeStyleCacheValid();
			}

			// decode and push colors to ImGui
			libIDCount = colors.size();
			static ImGuiCol imColorID;
			if (item->getCoreConfig().enabled)
			{
				for (const auto& color : colors)
				{
					mvThemeManager::decodelibID(color.first, &imColorID);
					ImGui::PushStyleColor(imColorID, color.second.first.toVec4());
				}
			}
			else
			{
				for (const auto& color : colors)
				{
					mvThemeManager::decodelibID(color.first, &imColorID);
					ImGui::PushStyleColor(imColorID, color.second.second.toVec4());
				}
			}

			// push styles2 and its matching style1 vect to ImGui then push the remaining float styles
			StyleIDCount = styles2.size() + styles.size();
			for (const auto& style : styles2)
				ImGui::PushStyleVar(style.first, { styles1[style.first], styles2[style.first] });

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