#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvThemeComponent : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeComponent, add_theme_component)

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
