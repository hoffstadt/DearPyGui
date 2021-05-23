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

			//-----------------------------------------------------------------------------
			// colors
			//-----------------------------------------------------------------------------

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

			//-----------------------------------------------------------------------------
			// styles
			//-----------------------------------------------------------------------------

			// updates styles if cache is invalid and caches
			SearchAncestorsForStyles(item);
			StyleIDCount = item->getStyleGroup().getCachedStyles().size() + item->getStyleGroup().getCachedStyles2().size();

			for (const auto& style : item->getStyleGroup().getCachedStyles2())
				ImPlot::PushStyleVar((ImPlotStyleVar)style.constant, { style.value1, style.value2 });

			for (const auto& style : item->getStyleGroup().getCachedStyles())
			{
				if((ImPlotStyleVar)style.constant == ImPlotStyleVar_Marker)
					ImPlot::PushStyleVar((ImPlotStyleVar)style.constant, (int)style.value1);
				else
					ImPlot::PushStyleVar((ImPlotStyleVar)style.constant, style.value1);
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