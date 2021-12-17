#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvTab : public mvAppItem
    {

        enum class TabOrdering {
            mvTabOrder_Reorderable = 0L,
            mvTabOrder_Fixed,
            mvTabOrder_Leading,
            mvTabOrder_Trailing
        };

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTab, add_tab)

    public:

        explicit mvTab(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        
        void addFlag   (ImGuiTabItemFlags flag);
        void removeFlag(ImGuiTabItemFlags flag);

    private:

        mvRef<bool>       _value = CreateRef<bool>(false);
        bool              _disabled_value = false;
        bool              _closable = false;
        ImGuiTabItemFlags _flags = ImGuiTabItemFlags_None;

    };

}
