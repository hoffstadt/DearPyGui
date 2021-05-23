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

}