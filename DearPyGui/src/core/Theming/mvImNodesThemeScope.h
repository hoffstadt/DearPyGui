#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeManager.h"
#include <imnodes.h>

namespace Marvel {

	template<typename T>
	class mvImNodesThemeScope
	{

	public:

		mvImNodesThemeScope(T* item)
		{

			//-----------------------------------------------------------------------------
			// colors
			//-----------------------------------------------------------------------------

			// updates colors if cache is invalid (disabled and regular)
			SearchAncestorsForColors(item);
			libIDCount = GetCacheColorCount(item);

			static int imColorID;
			if (item->isEnabled())
			{
				for (const auto& color : item->getColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					imnodes::PushColorStyle((imnodes::ColorStyle)imColorID, mvColor::ConvertToUnsignedInt(color.color));
				}
			}
			else
			{
				for (const auto& color : item->getDisabledColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					imnodes::PushColorStyle((imnodes::ColorStyle)imColorID, mvColor::ConvertToUnsignedInt(color.color));
				}
			}

			//-----------------------------------------------------------------------------
			// styles
			//-----------------------------------------------------------------------------

			// updates styles if cache is invalid and caches
			SearchAncestorsForStyles(item);
			StyleIDCount = item->getStyleGroup().getCachedStyles().size() + item->getStyleGroup().getCachedStyles2().size();

			for (const auto& style : item->getStyleGroup().getCachedStyles())
				imnodes::PushStyleVar((imnodes::StyleVar)style.constant, style.value1);
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
		size_t libIDCount = 0;
		size_t StyleIDCount = 0;

	};
}