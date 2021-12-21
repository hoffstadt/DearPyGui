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

    void mvThemeComponent::draw(ImDrawList* drawlist, float x, float y)
    {

        for (auto& childset : _children)
        {
            for (auto& child : childset)
            {
                child->draw(nullptr, 0.0f, 0.0f);
            }
        }

    }

    void mvThemeComponent::customAction(void* data)
    {
        for (auto& childset : _children)
        {
            for (auto& child : childset)
            {
                child->customAction(data);
            }
        }
        
    }

    void mvThemeComponent::handleSpecificPositionalArgs(PyObject* dict)
    {
        mv_local_persist mvRef<mvAppItem> all_item_theme_component = nullptr;

        if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(_type)], dict))
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
