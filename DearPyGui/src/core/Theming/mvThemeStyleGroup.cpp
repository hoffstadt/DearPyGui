#include "mvThemeStyleGroup.h"
#include "mvAppItem.h"

namespace Marvel {

	mvThemeStyleGroup::mvThemeStyleGroup(mvAppItem* parent)
		:
		m_parent(parent)
	{
	}

	bool mvThemeStyleGroup::doesStyleExists(long constant) const
	{
		for (const auto& style : m_cachedStyles)
		{
			if (style.constant == constant)
				return true;
		}

		return false;
	}

	bool mvThemeStyleGroup::addStyle(mvThemeStyle style)
	{
		if (doesStyleExists(style.constant))
		{
			assert(false && "style already exists here");
			return false;
		}

		m_cachedStyles.push_back(style);
		return true;
	}

	bool mvThemeStyleGroup::addStyle(const std::string& name, long constant, float value1, float value2)
	{
		if (doesStyleExists(constant))
		{
			assert(false && "style already exists here");
			return false;
		}

		m_styles.push_back({ name, constant, value1, value2, m_parent, false, false, 0.0f});
		return true;
	}

	std::vector<mvThemeStyleGroup::mvThemeStyle> mvThemeStyleGroup::getStylesByType(mvAppItemType type) const
	{
		std::vector<mvThemeStyleGroup::mvThemeStyle> styles;

		for (const auto& style : m_styles)
		{
			if ((mvAppItemType)mvThemeColorGroup::DecodeType(style.constant) == type)
				styles.push_back(style);
		}

		return styles;
	}

	const std::vector<mvThemeStyleGroup::mvThemeStyle>& mvThemeStyleGroup::getCachedStyles() const
	{
		return m_cachedStyles;
	}

	const std::vector<mvThemeStyleGroup::mvThemeStyle>& mvThemeStyleGroup::getCachedStyles2() const
	{
		return m_cachedStyles2;
	}

	bool mvThemeStyleGroup::isCacheValid() const
	{
		return !m_dirty;
	}

	void mvThemeStyleGroup::invalidateCache()
	{
		m_dirty = true;
		m_cachedStyles.clear();
		m_cachedStyles2.clear();
	}

	void mvThemeStyleGroup::setCacheValid()
	{
		const std::vector<mvThemeStyleGroup::mvThemeStyle> oldStyles = m_cachedStyles;
		m_cachedStyles.clear();

		std::vector<long> singleConstants;

		// first pass checking for second values of 2-component items
		for (const auto& style : oldStyles)
		{
			if (DecodeIndex(style.constant) > 0)
			{
				m_cachedStyles2.push_back({ style.name, style.constant-1, style.value1, style.value2, m_parent, false, true, 0.0f });
				singleConstants.push_back(style.constant - 1);

			}
		}

		auto updateValue1IfPossible = [&](mvThemeStyleGroup::mvThemeStyle style) {
			for (auto& item : m_cachedStyles2)
			{
				if (style.constant == item.constant)
				{
					item.value1 = style.value1;
					return true;
				}
				else if (style.constant == item.constant + 1)
					return true;
			}
			return false;
		};

		// second pass checking for first values of 2-component items
		for (const auto& style : oldStyles)
		{

			if (!updateValue1IfPossible(style))
				m_cachedStyles.push_back(style);
		}

		m_dirty = false;
	}

}