#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFilterSet : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFilterSet, add_filter_set)
 
    public:

        explicit mvFilterSet(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

        // values
        void setPyValue(PyObject* value) override;
        PyObject* getPyValue() override;

    private:

        ImGuiTextFilter _imguiFilter;
    };

}
