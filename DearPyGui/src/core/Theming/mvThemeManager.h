#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "composite/mvStyleWindow.h"

namespace Marvel {

	class mvThemeManager : public mvEventHandler
	{

	public:

		static void decodeType(long encoded_constant, mvAppItemType* type);
		static void decodelibID(long encoded_constant, int* libID);
		static int decodeIndex(long encoded_constant);

		static std::vector<std::tuple<std::string, long, mvColor*, mvColor*>>& GetColorsPtr() { return s_acolors; }
		static std::vector<std::tuple<std::string, long, float*, float>>& GetStylesPtr() { return s_astyles; }
		static std::unordered_map<mvAppItemType, mvThemeColors>& GetColors() { return s_colors; }
		static std::unordered_map<mvAppItemType, mvThemeStyles>& GetStyles() { return s_styles; }

	public:

		mvThemeManager();
		~mvThemeManager();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);
		bool add_style(mvEvent& event);

		static std::vector<std::tuple<std::string, long, mvColor*, mvColor*>> s_acolors;
		static std::vector<std::tuple<std::string, long, float*, float>>   s_astyles;
		static std::unordered_map<mvAppItemType, mvThemeColors>     s_colors;
		static std::unordered_map<mvAppItemType, mvThemeStyles>     s_styles;

	};

}