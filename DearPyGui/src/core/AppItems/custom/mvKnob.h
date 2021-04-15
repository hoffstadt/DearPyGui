#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

namespace Marvel {

    MV_REGISTER_WIDGET(mvKnobFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvKnobFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvKnobFloat, add_knob_float)

            MV_START_EXTRA_COMMANDS
            MV_END_EXTRA_COMMANDS

            MV_START_GENERAL_CONSTANTS
            MV_END_GENERAL_CONSTANTS

            MV_START_COLOR_CONSTANTS
            MV_END_COLOR_CONSTANTS

            MV_START_STYLE_CONSTANTS
            MV_END_STYLE_CONSTANTS

    public:

        mvKnobFloat(const std::string& name);

        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    private:

        float m_min = 0.0f;
        float m_max = 100.0f;
        float m_step = 50.0f;

    };

}