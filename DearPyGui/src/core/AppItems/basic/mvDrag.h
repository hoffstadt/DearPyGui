#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvModule_DearPyGui.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragFloatMulti
//     * mvDragInt
//     * mvDragIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloat
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragFloat, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
    class mvDragFloat : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragFloat, add_drag_float)
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

        explicit mvDragFloat(mvUUID uuid);

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
        float               _speed = 1.0f;
        float               _min = 0.0f;
        float               _max = 100.0f;
        std::string         _format = "%.3f";
        ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;

    };

    //-----------------------------------------------------------------------------
    // mvDragFloatMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
    class mvDragFloatMulti : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragFloatMulti, add_drag_floatx)
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

        explicit mvDragFloatMulti(mvUUID uuid);

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

        mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
        float                       _disabled_value[4]{};
        float                       _speed = 1.0f;
        float                       _min = 0.0f;
        float                       _max = 100.0f;
        std::string                 _format = "%.3f";
        ImGuiInputTextFlags         _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags         _stor_flags = ImGuiSliderFlags_None;
        int                         _size = 4;
    };

    //-----------------------------------------------------------------------------
    // mvDragInt
    //----------------------------------------------------------------------------- 
    MV_REGISTER_WIDGET(mvDragInt, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
    class mvDragInt : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragInt, add_drag_int)
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

        explicit mvDragInt(mvUUID uuid);

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
        float               _speed = 1.0f;
        int                 _min = 0;
        int                 _max = 100;
        std::string         _format = "%d";
        ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;

    };

    //-----------------------------------------------------------------------------
    // mvDragIntMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvDragIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
    class mvDragIntMulti : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragIntMulti, add_drag_intx)
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

        explicit mvDragIntMulti(mvUUID uuid);

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

        mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
        int                       _disabled_value[4]{};
        float                     _speed = 1.0f;
        int                       _min = 0;
        int                       _max = 100;
        std::string               _format = "%d";
        ImGuiInputTextFlags       _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags       _stor_flags = ImGuiSliderFlags_None;
        int                       _size = 4;
    };
}
