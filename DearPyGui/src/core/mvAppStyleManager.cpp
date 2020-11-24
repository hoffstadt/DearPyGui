#pragma once
#include "mvAppStyleManager.h" 
#include "mvMarvel.h"
#include "mvAppItems.h"

namespace Marvel
{

	std::unordered_map<int, mvColor*> mvAppStyleManager::s_colorDefines = {
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

}