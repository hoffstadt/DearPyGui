#pragma once
#include "mvTypeBases.h"
#include <string>
#include "mvModule_DearPyGui.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloat
//     * mvSliderInt
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvSliderFloat
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvSliderFloat : public mvFloatPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderFloat, add_slider_float)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

    public:

        mvSliderFloat(mvUUID uuid);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    private:

        float               _min = 0.0f;
        float               _max = 100.0f;
        std::string         _format = "%.3f";
        bool                _vertical = false;
        ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;

    };
     
    //-----------------------------------------------------------------------------
    // mvSliderInt
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderInt, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
    class mvSliderInt : public mvIntPtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
 
        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderInt, add_slider_int)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

    public:

        mvSliderInt(mvUUID uuid);
            
        void setEnabled(bool value) override;   
        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
            
    private:

        int                 _min = 0;
        int                 _max = 100;
        std::string         _format = "%d";
        bool                _vertical = false;
        ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;
    };


}
