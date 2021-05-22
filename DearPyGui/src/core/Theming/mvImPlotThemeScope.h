#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeManager.h"

namespace Marvel {

	template<typename T>
	class mvImPlotThemeScope
	{

	public:

		mvImPlotThemeScope(T* item)
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

			static ImPlotCol imColorID;
			if (item->isEnabled())
			{
				for (const auto& color : item->getColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					ImPlot::PushStyleColor(imColorID, color.color.toVec4());
				}
			}
			else
			{
				for (const auto& color : item->getDisabledColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					ImPlot::PushStyleColor(imColorID, color.color.toVec4());
				}
			}

			StyleIDCount = styles2.size() + styles.size();
			for (const auto& style : styles2)
				ImPlot::PushStyleVar((ImPlotStyleVar)style.first, { styles1[style.first], styles2[style.first] });

			for (const auto& style : styles)
			{
				if((ImPlotStyleVar)style.first == ImPlotStyleVar_Marker)
					ImPlot::PushStyleVar((ImPlotStyleVar)style.first, (int)style.second);
				else
					ImPlot::PushStyleVar((ImPlotStyleVar)style.first, style.second);
			}
		}

		~mvImPlotThemeScope()
		{
			cleanup();
		}

		void cleanup()
		{
			if (libIDCount > 0)
				ImPlot::PopStyleColor((int)libIDCount);
			libIDCount = 0;
			if (StyleIDCount > 0)
				ImPlot::PopStyleVar((int)StyleIDCount);
			StyleIDCount = 0;
		}

	private:
		size_t libIDCount = 0;
		size_t StyleIDCount = 0;

	};
}