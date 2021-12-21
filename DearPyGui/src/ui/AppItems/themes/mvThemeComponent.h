#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvThemeComponent : public mvAppItem
    {

    public:

        explicit mvThemeComponent(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void customAction(void* data = nullptr) override;

    public:

        int _specificType = (int)mvAppItemType::All;
        bool _specificEnabled = true;
        mvRef<mvAppItem>* _specificComponentPtr = nullptr;
        mvRef<mvAppItem>* _specificDisabledComponentPtr = nullptr;
        mvRef<mvAppItem> _oldComponent = nullptr;

    };

}
