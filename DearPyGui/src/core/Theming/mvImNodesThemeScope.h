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

			mvThemeColors& colors = item->getCachedThemeColors();
			mvThemeColors& disabled_colors = item->getCachedThemeDisabledColors();
			std::unordered_map<ImGuiStyleVar, float>& styles = item->getCachedThemeStyles();
			std::unordered_map<ImGuiStyleVar, float>& styles1 = item->getCachedThemeStyles1();
			std::unordered_map<ImGuiStyleVar, float>& styles2 = item->getCachedThemeStyles2();

			if (!item->isThemeColorCacheValid())
			{
				SearchAncestorTreeForColors<T>(item, colors);
				item->setThemeColorCacheValid();
			}

			if (!item->isThemeDisabledColorCacheValid())
			{
				SearchAncestorTreeForDisabledColors<T>(item, disabled_colors);
				item->setThemeDisabledColorCacheValid();
			}

			if (!item->isThemeStyleCacheValid())
			{
				SearchAncestorTreeForStyles<T>(item, styles, styles1, styles2);
				item->setThemeStyleCacheValid();
			}

			if (item->isEnabled())
				libIDCount = colors.size();
			else
				libIDCount = disabled_colors.size();
			static int imColorID;
			if (item->isEnabled())
			{
				for (auto& color : colors)
				{
					DecodelibID(color.first, &imColorID);
					imnodes::PushColorStyle((imnodes::ColorStyle)imColorID, color.second);
				}
			}
			else
			{
				for (auto& color : disabled_colors)
				{
					DecodelibID(color.first, &imColorID);
					imnodes::PushColorStyle((imnodes::ColorStyle)imColorID, color.second);
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