#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvThemeComponent : public mvAppItem
    {

        friend class mvTheme;

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeComponent, add_theme_component)
        MV_NO_COMMANDS


        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTheme)
        MV_END_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvThemeColor),
            MV_ADD_CHILD(mvAppItemType::mvThemeStyle)
        MV_END_CHILDREN

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
