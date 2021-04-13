#pragma once

//-----------------------------------------------------------------------------
// mvPythonParser
//
//     - This class has the following responsibilities:
//
//         * Assist in Python tuple parsing and error checks the operation
//         * Builds documentation based on PythonDataElement List
//         * Categorizes Python Commands
//         * Builds the Python parse string
//
//     - Should probably be reworked for v1.0 to be clearer
//     
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <map>
#include <assert.h>

// forward declare PyObject
// as suggested on the python mailing list
// http://mail.python.org/pipermail/python-dev/2003-August/037601.html
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Enums and Helper Functions
    //-----------------------------------------------------------------------------
    enum class mvPyDataType
    {
        None = 0, String, Integer, Float, Bool, StringList, FloatList,
        Object, IntList, Double, Callable, Dict, ListFloatList, 
        ListStrList, ListListInt, Any
    };

    enum class mvArgType
    {
        REQUIRED_ARG=0,
        POSITIONAL_ARG,
        KEYWORD_ARG
    };


    const char* PythonDataTypeActual(mvPyDataType type);

    //-----------------------------------------------------------------------------
    // mvPythonDataElement
    //-----------------------------------------------------------------------------
    struct mvPythonDataElement
    {
        mvPyDataType type = mvPyDataType::None;
        const char* name = "";
        const char* description = "";
        const char* default_value = "...";
        mvArgType    arg_type = mvArgType::REQUIRED_ARG;
        bool         active = true;

        constexpr mvPythonDataElement(mvPyDataType type, const char* name, const char* description,
            const char* default_value, mvArgType arg_type, bool active)
            : type(type), name(name), description(description), default_value(default_value),
            arg_type(arg_type), active(active)
        {

        }

    };

    //-----------------------------------------------------------------------------
    // mvPythonParser
    //-----------------------------------------------------------------------------
    class mvPythonParser
    {

    public:

        //mvPythonParser() = default;

        explicit mvPythonParser(mvPyDataType returnType = mvPyDataType::None, const char* about = "Undocumented function", const char* category = "App");

        template<mvPyDataType type>
        void addArg(const char* name, mvArgType argType = mvArgType::REQUIRED_ARG, const char* defaultValue = "...", const char* description="")
        {
            for (const auto& arg : m_staged_elements)
            {
                if (strcmp(arg.name, name) == 0)
                {
                    assert(false);
                    return;
                }
            }
            m_staged_elements.emplace_back(type, name, description, defaultValue, argType, true);
        }

        void removeArg(const char* name);

        bool verifyRequiredArguments(PyObject* args);
        bool verifyPositionalArguments(PyObject* args);

        bool parse(PyObject* args, PyObject* kwargs, const char* message, ...);

        [[nodiscard]] const char*        getDocumentation         () const { return m_documentation.c_str(); }
        [[nodiscard]] const std::string& getCategory              () const { return m_category; }

        void finalize();

    private:

        std::vector<mvPythonDataElement> m_staged_elements;
        std::vector<mvPythonDataElement> m_required_elements;
        std::vector<mvPythonDataElement> m_optional_elements;
        std::vector<mvPythonDataElement> m_keyword_elements;
        std::vector<char>                m_formatstring;
        std::vector<const char*>         m_keywords;

        std::string                      m_about;
        mvPyDataType                     m_return;
        std::string                      m_documentation;
        std::string                      m_category;

    };

}