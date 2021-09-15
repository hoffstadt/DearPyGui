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

#include "mvAppItem.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvFileExtension, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
    class mvFileExtension : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileExtension, add_file_extension)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvFileDialog),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

    public:

        explicit mvFileExtension(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        const std::string& getFilter() { return _extension; }

    private:

        std::string _extension;
        std::string _customText;
        mvColor     _color = {-1.0f, 0.0f, 0.0f, 1.0f};
    };

}
