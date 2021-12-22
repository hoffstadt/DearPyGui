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
		if (item->_enabled)
		{
			if (auto classTheme = GetClassThemeComponent(item->_type))
				static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}
		else
		{
			if (auto classTheme = GetDisabledClassThemeComponent(item->_type))
				static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		if (item->_theme)
		{
			static_cast<mvTheme*>(item->_theme.get())->setSpecificEnabled(item->_enabled);
			static_cast<mvTheme*>(item->_theme.get())->setSpecificType((int)item->_type);
			static_cast<mvTheme*>(item->_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}
	}

	void 
	cleanup_local_theming(mvAppItem* item)
	{
		// handle popping themes
		if (item->_enabled)
		{
			if (auto classTheme = GetClassThemeComponent(item->_type))
				static_cast<mvThemeComponent*>(classTheme.get())->customAction();
		}
		else
		{
			if (auto classTheme = GetDisabledClassThemeComponent(item->_type))
				static_cast<mvThemeComponent*>(classTheme.get())->customAction();
		}

		if (item->_theme)
		{
			static_cast<mvTheme*>(item->_theme.get())->setSpecificEnabled(item->_enabled);
			static_cast<mvTheme*>(item->_theme.get())->setSpecificType((int)item->_type);
			static_cast<mvTheme*>(item->_theme.get())->customAction();
		}
	}

	mvTheme::mvTheme(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = false;
	}

	void mvTheme::draw(ImDrawList* drawlist, float x, float y)
	{
		for (auto& child : _children[1])
		{
			auto comp = static_cast<mvThemeComponent*>(child.get());
			if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					child->draw(drawlist, x, y);
				}
				
			}
			if(comp->_specificType != _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					comp->_oldComponent = *comp->_specificComponentPtr;
					*comp->_specificComponentPtr = child;
				}
				else
				{
					comp->_oldComponent = *comp->_specificDisabledComponentPtr;
					*comp->_specificDisabledComponentPtr = child;
				}
			}
		}
	}

	void mvTheme::customAction(void* data)
	{

		for (auto& child : _children[1])
		{
			auto comp = static_cast<mvThemeComponent*>(child.get());
			if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					child->customAction(data);
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
