#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeManager.h"

namespace Marvel {

	template<typename T>
	class mvImGuiThemeScope
	{

	public:

		mvImGuiThemeScope(T* item)
		{
			std::unordered_map<ImGuiStyleVar, float>& styles = item->getCachedThemeStyles();
			std::unordered_map<ImGuiStyleVar, float>& styles1 = item->getCachedThemeStyles1();
			std::unordered_map<ImGuiStyleVar, float>& styles2 = item->getCachedThemeStyles2();

			if (!item->isThemeStyleCacheValid())
			{
				SearchAncestorTreeForStyles<T>(item, styles, styles1, styles2);
				item->setThemeStyleCacheValid();
			}

			// updates colors if cache is invalid (disabled and regular)
			SearchAncestorsForColors(item);
			libIDCount = GetCacheColorCount(item);

			static ImGuiCol imColorID;
			if (item->isEnabled())
			{
				for (const auto& color : item->getColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					ImGui::PushStyleColor(imColorID, color.color.toVec4());
				}
			}
			else
			{
				for (const auto& color : item->getDisabledColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					ImGui::PushStyleColor(imColorID, color.color.toVec4());
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
				ImGui::PopStyleColor((int)libIDCount);
			libIDCount = 0;
			if (StyleIDCount > 0)
				ImGui::PopStyleVar((int)StyleIDCount);
			StyleIDCount = 0;
		}

	private:

		size_t libIDCount = 0;
		size_t StyleIDCount = 0;

	};
}