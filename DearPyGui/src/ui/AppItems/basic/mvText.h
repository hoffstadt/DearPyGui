#include "mvItemRegistry.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//
//-----------------------------------------------------------------------------

class mvText : public mvAppItem
{

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