#pragma once

#include <vector>
#include <unordered_map>
#include "mvCore.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvAppItem; 
	enum class mvAppItemType;

	//-----------------------------------------------------------------------------
	// mvThemeColorGroup
	//-----------------------------------------------------------------------------
	class mvThemeColorGroup
	{

	public:

		struct mvThemeColor
		{
			long          constant = 0L;
			mvColor       color{};
			mvAppItem*    parent = nullptr; // item where this color was found
			bool          defaultColor = false;
		};

	public:

		static mvAppItemType DecodeType(long encoded_constant);

	public:

		mvThemeColorGroup(mvAppItem* m_parent);

		bool                             addColor(mvThemeColor color);           // add a color from another group
		bool                             addColor(long constant, mvColor color); // add a new color
		std::vector<mvThemeColor>        getColorsByType(mvAppItemType type) const;

		// caching
		bool                             isCacheValid() const;
		void                             invalidateCache();
		void                             setCacheValid();
		const std::vector<mvThemeColor>& getCachedColors() const;

	private:

		bool doesColorExists(long constant) const;

	private:

		mvAppItem*                m_parent = nullptr;       // owner of this color group
		std::vector<mvThemeColor> m_colors;
		std::vector<mvThemeColor> m_cachedColors; // colors for the parent widget of this group
		bool                      m_dirty = true;

	};

	size_t GetCacheColorCount(mvAppItem* item);

	template<typename T>
	void SearchAncestorsForColors(T* item, bool disabled = false)
	{
		static const std::vector<std::tuple<std::string, long, mvColor, mvColor>>& color_constants = T::GetColorConstants();

		// get current item's color group and check cache
		mvThemeColorGroup& colorGroup = disabled ? item->getDisabledColorGroup() : item->getColorGroup();
		if (colorGroup.isCacheValid())
		{
			if (!disabled)
				SearchAncestorsForColors<T>(item, true);
			return;
		}

		// keep up with whether or not items are found
		std::unordered_map<long, bool> colors_found;
		for (const auto& color_pair : color_constants)
			colors_found[std::get<1>(color_pair)] = false;

		// get local colors
		const auto& localColorsForType = colorGroup.getColorsByType(item->getType());

		// register local colors as found
		for (const auto& color : localColorsForType)
		{
			if (colors_found.count(color.constant) > 0)
				colors_found[color.constant] = true;
		}

		// check ancestors for remaining colors
		mvAppItem* widget = item;
		while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
		{
			widget = widget->getParent();

			const auto& ancestorColorGroup = disabled ? widget->getDisabledColorGroup() : widget->getColorGroup();

			const auto& ancestorColorsForType = ancestorColorGroup.getColorsByType(item->getType());

			for (const auto& color : ancestorColorsForType)
			{
				// only update color if it wasn't found yet
				if (!colors_found[color.constant])
				{
					colorGroup.addColor(color);
					colors_found[color.constant] = true;
				}
			}
		}

		// if any colors were not found, check theme manager
		for (auto& color : disabled ? mvThemeManager::GetDisabledColors()[item->getType()] : mvThemeManager::GetColors()[item->getType()])
		{
			// only apply if it wasn't found yet
			if (!colors_found[color.first])
			{
				colorGroup.addColor({ color.first, color.second, nullptr, true });
				colors_found[color.first] = true;
			}
		}

		// ensure all colors were handles
		for (const auto& color : colors_found)
		{
			if (!color.second)
			{
				assert(false && "not all colors were found");
				break;
			}
		}
	}

}