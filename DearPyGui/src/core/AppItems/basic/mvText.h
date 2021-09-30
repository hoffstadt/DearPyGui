#include "mvItemRegistry.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvText
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvText, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
    class mvText : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvText, add_text)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_CLICKED |
            MV_STATE_VISIBLE |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

    public:

        explicit mvText(mvUUID uuid);

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

        mvRef<std::string> _value = CreateRef<std::string>("");
        std::string        _disabled_value = "";
        mvColor            _color = {-1.0f, 0.0f, 0.0f, 1.0f};
        int                _wrap = -1;
        bool               _bullet = false;
        bool               _show_label = false;

    };
}
