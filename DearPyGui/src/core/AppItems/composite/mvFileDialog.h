/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

#pragma warning(push, 0) 
#include <ImGuiFileDialog.h>
#pragma warning(pop)

namespace Marvel {

    MV_REGISTER_WIDGET(mvFileDialog, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
    class mvFileDialog final : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileDialog, add_file_dialog)
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_CREATE_COMMAND(get_file_dialog_info);

        // TODO: add appropriate states
        MV_SET_STATES(MV_STATE_NONE);

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
