#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    enum class mvComboHeightMode 
    {
        mvComboHeight_Small = 0L,
        mvComboHeight_Regular,
        mvComboHeight_Large,
        mvComboHeight_Largest
    };

    struct mvComboConfig
    {
        ImGuiComboFlags          flags = ImGuiComboFlags_None;
        std::vector<std::string> items;
        bool                     popup_align_left = false;
        bool                     no_preview = false;
    };

    class mvCombo : public mvAppItem
    {

    public:

        explicit mvCombo(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

        mvRef<std::string> value = CreateRef<std::string>("");
        std::string        disabled_value = "";
        mvComboConfig      configData{};
    };

}
