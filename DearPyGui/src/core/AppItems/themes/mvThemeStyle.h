#pragma once

#include <array>
#include "mvItemRegistry.h"
#include <imnodes.h>

namespace Marvel {

    class mvThemeStyle : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeStyle, add_theme_style)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvThemeComponent),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

    public:

        explicit mvThemeStyle(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

        void setLibType(mvLibType libType) { _libType = libType; }

    private:

        mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, -1.0f, 0.0f, 0.0f});
        ImGuiCol _targetStyle = 0;
        mvLibType _libType = mvLibType::MV_IMGUI;
        

    };

}
