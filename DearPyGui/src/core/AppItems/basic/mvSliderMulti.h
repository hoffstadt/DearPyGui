#pragma once
#include "mvTypeBases.h"
#include <string>
#include "mvModule_Core.h"

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
    class mvSliderFloatMulti : public mvFloat4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderFloatMulti, add_slider_floatx)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

    public:

        mvSliderFloatMulti(mvUUID uuid);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        
    private:

        float                   m_min = 0.0f;
        float                   m_max = 100.0f;
        std::string             m_format = "%.3f";
        ImGuiInputTextFlags     m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags     m_stor_flags = ImGuiSliderFlags_None;
        int                     m_size = 4;
    };
        
    //-----------------------------------------------------------------------------
    // mvSliderIntMulti
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvSliderIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
    class mvSliderIntMulti : public mvInt4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSliderIntMulti, add_slider_intx)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

    public:

        mvSliderIntMulti(mvUUID uuid);
        
        void setEnabled(bool value) override;
        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        
    private:

        int                 m_min = 0;
        int                 m_max = 100;
        std::string         m_format = "%d";
        ImGuiInputTextFlags m_flags = ImGuiSliderFlags_None;
        ImGuiInputTextFlags m_stor_flags = ImGuiSliderFlags_None;
        int                 m_size = 4;
    };

}
