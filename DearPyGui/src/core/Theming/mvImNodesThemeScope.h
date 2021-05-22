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

			StyleIDCount = styles.size();

			for (const auto& style : styles)
				imnodes::PushStyleVar((imnodes::StyleVar)style.first, style.second);
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