#pragma once
#include "mvAppStyleManager.h" 
#include "mvMarvel.h"
#include "mvAppItems.h"

namespace Marvel
{

	std::unordered_map<int, mvColor*> mvAppStyleManager::s_colorDefines = {
	{ GetConstantId("mvGuiCol_Text"), &mvButton::s_textColor},
	{ GetConstantId("mvGuiCol_Button"), &mvButton::s_buttonColor},
	{ GetConstantId("mvGuiCol_ButtonHovered"), &mvButton::s_buttonHoveredColor},
	{ GetConstantId("mvGuiCol_ButtonActive"), &mvButton::s_buttonActiveColor} };

	void mvAppStyleManager::SetAppColor(int id, mvColor color)
	{	
		s_colorDefines[id]->r = color.r;
		s_colorDefines[id]->g = color.g;
		s_colorDefines[id]->b = color.b;
		s_colorDefines[id]->a = color.a;
	}

	void mvAppStyleManager::SetAppColors(std::string theme)
	{
		if (theme == "Dark")
		{
			for (auto color : s_darkAppColors)
				SetAppColor(GetConstantId(color.first), color.second);
		}
		else if (theme == "Classic")
		{
			for (auto color : s_classicAppColors)
				SetAppColor(GetConstantId(color.first), color.second);
		}
		else if (theme == "Light")
		{
			for (auto color : s_lightAppColors)
				SetAppColor(GetConstantId(color.first), color.second);
		}
	}

	std::vector <std::pair<std::string, mvColor>> mvAppStyleManager::s_darkAppColors
	{
		{"mvGuiCol_Text", mvColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"mvGuiCol_Button", mvColor(0.26f, 0.59f, 0.98f, 0.40f)},
		{"mvGuiCol_ButtonHovered", mvColor(0.26f, 0.59f, 0.98f, 1.00f)},
		{"mvGuiCol_ButtonActive", mvColor(0.06f, 0.53f, 0.98f, 1.00f)}
	};

	std::vector <std::pair<std::string, mvColor>> mvAppStyleManager::s_classicAppColors
	{
		{"mvGuiCol_Text", mvColor(0.90f, 0.90f, 0.90f, 1.00f)},
		{"mvGuiCol_Button", mvColor(0.35f, 0.40f, 0.61f, 0.62f)},
		{"mvGuiCol_ButtonHovered", mvColor(0.40f, 0.48f, 0.71f, 0.79f)},
		{"mvGuiCol_ButtonActive", mvColor(0.46f, 0.54f, 0.80f, 1.00f)}
	};

	std::vector <std::pair<std::string, mvColor>> mvAppStyleManager::s_lightAppColors
	{
		{"mvGuiCol_Text", mvColor(0.00f, 0.00f, 0.00f, 1.00f)},
		{"mvGuiCol_Button", mvColor(0.26f, 0.59f, 0.98f, 0.40f)},
		{"mvGuiCol_ButtonHovered", mvColor(0.26f, 0.59f, 0.98f, 1.00f)},
		{"mvGuiCol_ButtonActive", mvColor(0.06f, 0.53f, 0.98f, 1.00f)}
	};
}