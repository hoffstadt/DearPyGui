#pragma once

#include <map>
#include <string>
#include <array>
#include <vector>

namespace Marvel {

	class mvTheme
	{

	public:

		mvTheme();
		
		void changeThemeItem(const char* name, float r, float g, float b, float a);
		void addThemeItem   (const char* name, float r, float g, float b, float a);
		void addThemeItem   (const char* name, std::array<float, 4> color);

		inline const std::vector< std::array<float, 4>>& getColors() const { return m_colors; }
		inline unsigned getNumberOfItems() const { return m_items; }


	private:

		unsigned                            m_items = 0u;
		std::map<std::string, unsigned >    m_colorIds;
		std::vector< std::array<float, 4> > m_colors;

	};

}