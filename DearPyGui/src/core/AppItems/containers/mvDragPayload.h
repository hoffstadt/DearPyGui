#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDragPayload : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragPayload, add_drag_payload)

    public:

        explicit mvDragPayload(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        PyObject* getDragData() const { return _dragData; }
        PyObject* getDropData() const { return _dropData; }

    private:

        std::string _payloadType = "$$DPG_PAYLOAD";
        PyObject*   _dragData = nullptr;
        PyObject*   _dropData = nullptr;

    };

}
