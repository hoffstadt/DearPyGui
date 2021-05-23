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
	// mvThemeColorGroup
	//-----------------------------------------------------------------------------
	class mvThemeColorGroup
	{

	public:

		struct mvThemeColor
		{
			std::string   name;
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
		bool                             addColor(const std::string& name, long constant, mvColor color); // add a new color
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

}