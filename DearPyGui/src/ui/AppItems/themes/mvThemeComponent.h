#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvThemeComponent : public mvAppItem
    {

    public:

        explicit mvThemeComponent(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

        void push_theme_items();
        void pop_theme_items();

    public:

        int                      _specificType                 = (int)mvAppItemType::All;
        bool                     _specificEnabled              = true;
        mvRef<mvThemeComponent>* _specificComponentPtr         = nullptr;
        mvRef<mvThemeComponent>* _specificDisabledComponentPtr = nullptr;
        mvRef<mvThemeComponent>  _oldComponent                 = nullptr;

    };

}
