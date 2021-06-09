#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>

namespace Marvel {

    MV_REGISTER_WIDGET(mvSlider3D, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
    class mvSlider3D : public mvFloat4PtrBase
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSlider3D, add_3d_slider)

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS

    public:

        mvSlider3D(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    private:

        float m_minX = 0.0f;
        float m_minY = 0.0f;
        float m_minZ = 0.0f;
        float m_maxX = 100.0f;
        float m_maxY = 100.0f;
        float m_maxZ = 100.0f;
        float m_scale = 1.0f;

    };

}