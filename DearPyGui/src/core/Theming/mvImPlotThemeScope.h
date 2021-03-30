#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "composite/mvStyleWindow.h"
#include "mvThemeManager.h"

namespace Marvel {

	template<typename T>
	class mvImPlotThemeScope
	{

	public:

		mvImPlotThemeScope(T* item)
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

			libIDCount = colors.size();
			static ImPlotCol imColorID;
			for (const auto& color : colors)
			{
				DecodelibID(color.first, &imColorID);
				ImPlot::PushStyleColor(imColorID, color.second.first.toVec4());
			}

			StyleIDCount = styles.size();
			for (const auto& style : styles2)
			{
				ImPlot::PushStyleVar((ImPlotStyleVar)style.first, { styles[style.first], styles2[style.first] });
				styles.erase(style.first);
			}
			for (const auto& style : styles)
				ImPlot::PushStyleVar((ImPlotStyleVar)style.first, style.second);
		}

		~mvImPlotThemeScope()
		{
			cleanup();
		}

		void cleanup()
		{
			if (libIDCount > 0)
				ImPlot::PopStyleColor(libIDCount);
			libIDCount = 0;
			if (StyleIDCount > 0)
				ImPlot::PopStyleVar(StyleIDCount);
			StyleIDCount = 0;
		}

	private:
		int libIDCount = 0;
		int StyleIDCount = 0;

	};
}