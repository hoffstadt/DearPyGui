#pragma once
#include "mvAppStyleManager.h" 
#include "mvMarvel.h"
#include "mvAppItems.h"

namespace Marvel
{
	mvAppStyleManager::mvAppStyleManager(){
	};

	std::unordered_map<int, mvColor*> mvAppStyleManager::colorDefines = {
	{ 21, &mvButton::buttonColor},
	{ 22, &mvButton::buttonHoveredColor},
	{ 23, &mvButton::buttonActiveColor} };

	void mvAppStyleManager::setAppColor(int id, mvColor color)
	{	
		colorDefines[id]->r = color.r;
		colorDefines[id]->g = color.g;
		colorDefines[id]->b = color.b;
		colorDefines[id]->a = color.a;
	}

}