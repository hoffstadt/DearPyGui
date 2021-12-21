#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvCombo : public mvAppItem
    {

        enum class ComboHeightMode {
            mvComboHeight_Small = 0L,
            mvComboHeight_Regular,
            mvComboHeight_Large,
            mvComboHeight_Largest
        };

    public:

        explicit mvCombo(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::string>       _value = CreateRef<std::string>("");
        std::string              _disabled_value = "";
        ImGuiComboFlags          _flags = ImGuiComboFlags_None;
        std::vector<std::string> _items;
        bool                     _popup_align_left = false;
        bool                     _no_preview = false;
    };

}
