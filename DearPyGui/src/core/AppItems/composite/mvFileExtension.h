#pragma once

#include "mvAppItem.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvFileExtension : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileExtension, add_file_extension)

    public:

        explicit mvFileExtension(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        const std::string& getFilter() { return _extension; }

    public:

        std::string _extension;
        std::string _customText;
        mvColor     _color = {-1.0f, 0.0f, 0.0f, 1.0f};
    };

}
