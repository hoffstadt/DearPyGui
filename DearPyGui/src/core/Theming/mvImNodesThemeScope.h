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

			mvThemeColors& colors = item->getCachedThemeColors();
			std::unordered_map<ImGuiStyleVar, float>& styles = item->getCachedThemeStyles();
			std::unordered_map<ImGuiStyleVar, float>& styles1 = item->getCachedThemeStyles1();
			std::unordered_map<ImGuiStyleVar, float>& styles2 = item->getCachedThemeStyles2();

			if (!item->isThemeCacheValid())
			{
				SearchAncestorTreeForColors<T>(item, colors);
				SearchAncestorTreeForStyles<T>(item, styles, styles1, styles2);
				item->setThemeCacheValid();
			}

			libIDCount = colors.size();
			static int imColorID;
			for (auto& color : colors)
			{
				mvThemeManager::decodelibID(color.first, &imColorID);
				imnodes::PushColorStyle((imnodes::ColorStyle)imColorID, color.second.first);
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
		int libIDCount = 0;
		int StyleIDCount = 0;

	};
}