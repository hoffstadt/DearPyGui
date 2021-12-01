#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

    class mvNodeLink : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        friend class mvNodeEditor;

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeLink, add_node_link)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvNodeEditor)
        MV_END_PARENTS

    public:

        explicit mvNodeLink(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void getSpecificConfiguration(PyObject* dict) override;
        
        int getId() const { return _id0; }
        int getId1() const { return _id1; }
        int getId2() const { return _id2; }

    private:

        int _id0 = 0;
        int _id1 = 0;
        int _id2 = 0;

        mvUUID _id1uuid = 0;
        mvUUID _id2uuid = 0;
    };

}
