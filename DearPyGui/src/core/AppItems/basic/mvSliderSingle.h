#pragma once

#include "mvItemRegistry.h"
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
    class mvSliderFloat : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderFloat, add_slider_float)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_ACTIVE |
            MV_STATE_FOCUSED |
            MV_STATE_CLICKED |
            MV_STATE_VISIBLE |
            MV_STATE_EDITED |
            MV_STATE_ACTIVATED |
            MV_STATE_DEACTIVATED |
            MV_STATE_DEACTIVATEDAE |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

    public:

        explicit mvSliderFloat(mvUUID uuid);
        
        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<float>        _value = CreateRef<float>(0.0f);
        float               _disabled_value = 0.0f;
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
    class mvSliderInt : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
 
        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderInt, add_slider_int)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_ACTIVE |
            MV_STATE_FOCUSED |
            MV_STATE_CLICKED |
            MV_STATE_VISIBLE |
            MV_STATE_EDITED |
            MV_STATE_ACTIVATED |
            MV_STATE_DEACTIVATED |
            MV_STATE_DEACTIVATEDAE |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

    public:

        explicit mvSliderInt(mvUUID uuid);
            
        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
            
    private:

        mvRef<int>          _value = CreateRef<int>(0);
        int                 _disabled_value = 0;
        int                 _min = 0;
        int                 _max = 100;
        std::string         _format = "%d";
        bool                _vertical = false;
        ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;
    };


}
