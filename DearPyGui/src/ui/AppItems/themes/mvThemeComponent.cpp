#include "mvThemeComponent.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvAppItemCommons.h"

namespace Marvel {

    mvThemeComponent::mvThemeComponent(mvUUID uuid)
        :
        mvAppItem(uuid)
    {
    }

    void mvThemeComponent::push_theme_items()
    {

        for (auto& child : childslots[1])
        {
            if (child->type == mvAppItemType::mvThemeColor)
                ((mvThemeColor*)child.get())->push_theme_color();
            else if (child->type == mvAppItemType::mvThemeStyle)
                ((mvThemeStyle*)child.get())->push_theme_style();
        }

    }

    void mvThemeComponent::pop_theme_items()
    {

        for (auto& child : childslots[1])
        {
            if (child->type == mvAppItemType::mvThemeColor)
                ((mvThemeColor*)child.get())->pop_theme_color();
            else if (child->type == mvAppItemType::mvThemeStyle)
                ((mvThemeStyle*)child.get())->pop_theme_style();
        }
        
    }

    void mvThemeComponent::handleSpecificPositionalArgs(PyObject* dict)
    {
        mv_local_persist mvRef<mvThemeComponent> all_item_theme_component = nullptr;

        if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
            return;

        for (int i = 0; i < PyTuple_Size(dict); i++)
        {
            PyObject* item = PyTuple_GetItem(dict, i);
            switch (i)
            {
            case 0:
            {
                _specificType = ToInt(item);
                _specificComponentPtr = &GetClassThemeComponent((mvAppItemType)_specificType);
                _specificDisabledComponentPtr = &GetDisabledClassThemeComponent((mvAppItemType)_specificType);

                if (_specificType == (int)mvAppItemType::All)
                {
                    _specificComponentPtr = &all_item_theme_component;
                    _specificDisabledComponentPtr = &all_item_theme_component;
                }

                break;
            }
            default:
                break;
            }
        }
    }

    void mvThemeComponent::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "enabled_state")) _specificEnabled = ToBool(item);
    }

    void mvThemeComponent::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_target = ToPyInt(_specificType);
		mvPyObject py_cat = ToPyBool((int)_specificEnabled);

		PyDict_SetItemString(dict, "item_type", py_target);
		PyDict_SetItemString(dict, "enabled_state", py_cat);
	}
}
