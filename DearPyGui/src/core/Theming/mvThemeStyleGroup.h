#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "mvCore.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvAppItem;
	enum class mvAppItemType;

	//-----------------------------------------------------------------------------
	// mvThemeStyleGroup
	//-----------------------------------------------------------------------------
	class mvThemeStyleGroup
	{

	public:

		struct mvThemeStyle
		{
			std::string   name;
			long          constant = 0L;
			float         value1 = 0.0f;
			float         value2 = 0.0f;
			mvAppItem*    parent = nullptr; // item where this color was found
			bool          defaultStyle = false;
			bool          has2Values = false;
			float         maxValue = 0.0f;
		};

	public:

		mvThemeStyleGroup(mvAppItem* m_parent);

		bool                      addStyle(mvThemeStyle style);                                  // add a color from another group
		bool                      addStyle(const std::string& name, long constant, float value1, float value2); // add a new color
		std::vector<mvThemeStyle> getStylesByType(mvAppItemType type) const;

		// caching
		bool                             isCacheValid() const;
		void                             invalidateCache();
		void                             setCacheValid();
		const std::vector<mvThemeStyle>& getCachedStyles() const;  // 1-component styles
		const std::vector<mvThemeStyle>& getCachedStyles2() const; // 2-component styles

	private:

		bool doesStyleExists(long constant) const;

	private:

		mvAppItem*                m_parent = nullptr; // owner of this style group
		std::vector<mvThemeStyle> m_styles;
		std::vector<mvThemeStyle> m_cachedStyles;  // styles for the parent widget of this group
		std::vector<mvThemeStyle> m_cachedStyles2; // styles for the parent widget of this group
		bool                      m_dirty = true;

	};

	template<typename T>
	void SearchAncestorsForStyles(T* item)
	{

		static const std::vector<std::tuple<std::string, long, float, float>>& style_constants = T::GetStyleConstants();

		// get current item's style group and check cache
		mvThemeStyleGroup& styleGroup = item->getStyleGroup();
		if (styleGroup.isCacheValid())
			return;

		// keep up with whether styles are found or not
		static int styleID = 0;
		std::unordered_map<long, bool> styles_found;
		for (const auto& style_pair : style_constants)
			styles_found[std::get<1>(style_pair)] = false;

		// get local styles
		const auto& localStylesForType = styleGroup.getStylesByType(item->getType());

		// register local styles as found
		for (const auto& style : localStylesForType)
		{
			if (styles_found.count(style.constant) > 0)
			{
				styles_found[style.constant] = true;
				styleGroup.addStyle(style);
			}
		}

		// check ancestors for remaining styles
		mvAppItem* widget = item;
		while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
		{
			widget = widget->getParent();

			const auto& ancestorStyleGroup = widget->getStyleGroup();
			const auto& ancestorStylesForType = ancestorStyleGroup.getStylesByType(item->getType());

			for (const auto& style : ancestorStylesForType)
			{
				// only update style if it wasn't found yet
				if (!styles_found[style.constant])
				{
					styles_found[style.constant] = true;
					styleGroup.addStyle(style);
				}
			}
		}

		// if any styles were not found, check theme manager
		for (auto& style : mvThemeManager::GetStylesByType(item->getType()))
		{
			// only apply if it wasn't found yet
			if (!styles_found[style.constant])
			{
				styles_found[style.constant] = true;
				styleGroup.addStyle(style);
			}
		}

		styleGroup.setCacheValid();

		// ensure all styles were handled
		for (const auto& style : styles_found)
		{
			if (!style.second)
			{
				assert(false && "not all styles were found");
				break;
			}
		}

		
	}
}