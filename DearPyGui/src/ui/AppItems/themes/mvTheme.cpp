#include "mvTheme.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvThemeComponent.h"

namespace Marvel {

	void 
	apply_local_theming(mvAppItem* item)
	{
		if (item->config.enabled)
		{
			if (auto classTheme = GetClassThemeComponent(item->type))
				classTheme->push_theme_items();
		}
		else
		{
			if (auto classTheme = GetDisabledClassThemeComponent(item->type))
				classTheme->push_theme_items();
		}

		if (item->theme)
		{
			item->theme->setSpecificEnabled(item->config.enabled);
			item->theme->setSpecificType((int)item->type);
			item->theme->push_theme_components();
		}
	}

	void 
	cleanup_local_theming(mvAppItem* item)
	{
		if (item->config.enabled)
		{
			if (auto classTheme = GetClassThemeComponent(item->type))
				classTheme->pop_theme_items();
		}
		else
		{
			if (auto classTheme = GetDisabledClassThemeComponent(item->type))
				classTheme->pop_theme_items();
		}

		if (item->theme)
		{
			item->theme->setSpecificEnabled(item->config.enabled);
			item->theme->setSpecificType((int)item->type);
			item->theme->pop_theme_components();
		}
	}

	mvTheme::mvTheme(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		config.show = false;
	}

	void mvTheme::push_theme_components()
	{
		for (auto& child : childslots[1])
		{
			auto comp = static_cast<mvThemeComponent*>(child.get());
			if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					comp->push_theme_items();
				}
				
			}
			if(comp->_specificType != _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					comp->_oldComponent = *comp->_specificComponentPtr;
					*comp->_specificComponentPtr = *(mvRef<mvThemeComponent>*)&child;
				}
				else
				{
					comp->_oldComponent = *comp->_specificDisabledComponentPtr;
					*comp->_specificDisabledComponentPtr = *(mvRef<mvThemeComponent>*) & child;
				}
			}
		}
	}

	void mvTheme::pop_theme_components()
	{

		for (auto& child : childslots[1])
		{
			auto comp = static_cast<mvThemeComponent*>(child.get());
			if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					comp->pop_theme_items();
				}
			}
			if (comp->_specificType != _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					*comp->_specificComponentPtr = comp->_oldComponent;
				}
				else
				{
					*comp->_specificDisabledComponentPtr = comp->_oldComponent;
				}
			}
		}
	}
}
