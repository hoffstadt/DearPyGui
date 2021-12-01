#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

#pragma warning(push, 0) 
#include <ImGuiFileDialog.h>
#pragma warning(pop)

namespace Marvel {

    class mvFileDialog final : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileDialog, add_file_dialog)
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

        MV_CREATE_COMMAND(get_file_dialog_info);       

        MV_START_COMMANDS
            MV_ADD_COMMAND(get_file_dialog_info);
        MV_END_COMMANDS

    public:

        explicit mvFileDialog(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

        void drawPanel();

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

        ImGuiFileDialog& getDialog();
        void             markDirty() { _dirtySettings = true; }
        PyObject*        getInfoDict();
        bool             getContinueValue() const { return *_value; }

    private:

        mvRef<bool>     _value = CreateRef<bool>(false);
        bool            _disabled_value = false;
        ImGuiFileDialog _instance;
        bool            _dirtySettings = true;
        std::string     _filters;
        std::string     _defaultPath;
        std::string     _defaultFilename = ".";
        int             _fileCount = 0;
        bool            _modal = false;
        bool            _directory = false;
    };

}
