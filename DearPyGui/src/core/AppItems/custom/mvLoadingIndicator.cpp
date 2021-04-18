#include "mvLoadingIndicator.h"
#include "mvLoadingIndicatorCustom.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvLoadingIndicator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("enabled");
        parser.removeArg("callback");
        parser.removeArg("callback_data");
        parser.removeArg("source");

        parser.addArg<mvPyDataType::Integer>("style", mvArgType::KEYWORD_ARG, "0");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvLoadingIndicator::mvLoadingIndicator(const std::string& name)
        : mvAppItem(name)
    {
    }

    void mvLoadingIndicator::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (m_style == 0)
            LoadingIndicatorCircle(m_specificedlabel.c_str(), 3.0f);
        else
            LoadingIndicatorCircle2(m_specificedlabel.c_str(), 3.0f);
    }

    void mvLoadingIndicator::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "style")) m_style = ToInt(item);
    }

    void mvLoadingIndicator::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "style", ToPyInt(m_style));

    }

}