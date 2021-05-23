#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeColorGroup.h"

namespace Marvel {

	class mvThemeManager : public mvEventHandler
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_CREATE_EXTRA_COMMAND(set_theme_color);
		MV_CREATE_EXTRA_COMMAND(set_theme_color_disabled);
		MV_CREATE_EXTRA_COMMAND(set_theme_style);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(set_theme_color);
			MV_ADD_EXTRA_COMMAND(set_theme_color_disabled);
			MV_ADD_EXTRA_COMMAND(set_theme_style);
		MV_END_EXTRA_COMMANDS

		static void InValidateColorTheme();
		static void InValidateDisabledColorTheme();
		static void InValidateStyleTheme();

		static std::vector<mvThemeColorGroup::mvThemeColor*>& GetColorsPtr() { return s_acolors; }
		static std::vector<mvThemeColorGroup::mvThemeColor*>& GetDisabledColorsPtr() { return s_adcolors; }
		static std::vector<mvThemeStyleGroup::mvThemeStyle*>& GetStylesPtr() { return s_astyles; }
		static std::vector<mvThemeColorGroup::mvThemeColor>& GetColors() { return s_colors; }
		static std::vector<mvThemeColorGroup::mvThemeColor>& GetDisabledColors() { return s_disabled_colors; }
		static std::vector<mvThemeStyleGroup::mvThemeStyle>& GetStyles() { return s_styles; }
		static const std::string& GetNameFromConstant(long constant);

		static std::vector<mvThemeColorGroup::mvThemeColor> GetColorsByType(mvAppItemType type, bool disabled);
		static std::vector<mvThemeStyleGroup::mvThemeStyle> GetStylesByType(mvAppItemType type);

	public:

		mvThemeManager();
		~mvThemeManager();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);
		bool add_disabled_color(mvEvent& event);
		bool add_style(mvEvent& event);

		// new
		static std::vector<mvThemeColorGroup::mvThemeColor> s_colors;
		static std::vector<mvThemeColorGroup::mvThemeColor*> s_acolors;

		static std::vector<mvThemeColorGroup::mvThemeColor> s_disabled_colors;
		static std::vector<mvThemeColorGroup::mvThemeColor*> s_adcolors;

		static std::vector<mvThemeStyleGroup::mvThemeStyle> s_styles;
		static std::vector<mvThemeStyleGroup::mvThemeStyle*> s_astyles;

		//static std::unordered_map<mvAppItemType, mvThemeStyles>          s_styles;
		//static std::vector<std::tuple<std::string, long, float*, float>> s_astyles;
	};

}