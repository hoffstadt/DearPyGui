#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputInt
//     * mvInputFloat
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvInputInt
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputInt, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
    class mvInputInt : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputInt, add_input_int)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

    public:

        mvInputInt(mvUUID uuid);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        
    private:

        int                 _step = 1;
        int                 _step_fast = 100;
        int                 _min = 0;
        int                 _max = 100;
        bool                _min_clamped = false;
        bool                _max_clamped = false;
        ImGuiInputTextFlags _flags = 0;
        ImGuiInputTextFlags _stor_flags = 0;
        int                 _last_value = 0;
        
    };
        
    //-----------------------------------------------------------------------------
    // mvInputFloat
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvInputFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvInputFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputFloat, add_input_float)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

    public:

        mvInputFloat(mvUUID uuid);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    private:

        float               _min = 0.0f;
        float               _max = 100.0f;
        bool                _min_clamped = false;
        bool                _max_clamped = false;
        std::string         _format = "%.3f";
        float               _step = 0.1f;
        float               _step_fast = 1.0f;
        ImGuiInputTextFlags _flags = 0;
        ImGuiInputTextFlags _stor_flags = 0;
        float               _last_value = 0.0f;
        
    };
 
}
