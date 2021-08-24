#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloatMulti
//     * mvSliderIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {
  
    //-----------------------------------------------------------------------------
    // mvSliderFloat4
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
    class mvSliderFloatMulti : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderFloatMulti, add_slider_floatx)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

    public:

        mvSliderFloatMulti(mvUUID uuid);

        // overriding until we can remove these
        bool preDraw() override { return true; }
        void postDraw() override {}
        
        void draw(ImDrawList* drawlist, float x, float y) override;
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
    private:

        mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
        float                       _disabled_value[4]{};
        float                       _min = 0.0f;
        float                       _max = 100.0f;
        std::string                 _format = "%.3f";
        ImGuiInputTextFlags         _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags         _stor_flags = ImGuiSliderFlags_None;
        int                         _size = 4;
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderIntMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
    class mvSliderIntMulti : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderIntMulti, add_slider_intx)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

    public:

        mvSliderIntMulti(mvUUID uuid);

        // overriding until we can remove these
        bool preDraw() override { return true; }
        void postDraw() override {}
        
        void draw(ImDrawList* drawlist, float x, float y) override;
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        
    private:
        mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
        int                       _disabled_value[4]{};
        int                       _min = 0;
        int                       _max = 100;
        std::string               _format = "%d";
        ImGuiInputTextFlags       _flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags       _stor_flags = ImGuiSliderFlags_None;
        int                       _size = 4;
    };

}
