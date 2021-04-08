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
#include "mvCore.h"
#include "mvPython.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Enums and Helper Functions
    //-----------------------------------------------------------------------------
    enum class mvPyDataType
    {
        None = 0, String, Integer, Float, Bool, StringList, FloatList, Optional,
        Object, IntList, KeywordOnly, Double, Callable, Dict, ListFloatList, 
        ListStrList, Kwargs
    };

    enum class mvArgType
    {
        REQUIRED, OPTIONAL, KEYWORD
    };

    //-----------------------------------------------------------------------------
    // mvPythonDataElement
    //-----------------------------------------------------------------------------
    struct mvPythonDataElement
    {
        mvPyDataType type = mvPyDataType::None;
        std::string  name = "";
        std::string  description = "";
        std::string  default_value = "...";
        mvArgType    arg_type = mvArgType::REQUIRED;
        bool         active = true;
    };

    const char* PythonDataTypeActual(mvPyDataType type);

    //-----------------------------------------------------------------------------
    // mvPythonParser
    //-----------------------------------------------------------------------------
    class mvPythonParser
    {

    public:

        mvPythonParser() = default;

        mvPythonParser(mvPyDataType returnType = mvPyDataType::None, std::string about = "Undocumented function", std::string category = "App");

        template<mvPyDataType type>
        void addArg(std::string name, mvArgType argType, std::string defaultValue = "...", std::string description = "")
        {
            mvPythonDataElement element = { type, name, description, defaultValue, argType };
            for (auto& arg : m_staged_elements)
            {
                if (arg.name == name)
                {
                    assert(false);
                    return;
                }
            }
            m_staged_elements.push_back(element);
        }

        void removeArg(std::string name);

        [[nodiscard]] const char*        getDocumentation                () const { return m_documentation.c_str(); }
        [[nodiscard]] const std::string& getCategory                     () const { return m_category; }

        void finalize();

    private:

        std::vector<mvPythonDataElement> m_staged_elements;
        std::vector<mvPythonDataElement> m_positional_elements;
        std::vector<mvPythonDataElement> m_optional_elements;
        std::vector<mvPythonDataElement> m_keyword_elements;

        std::string                      m_about;
        mvPyDataType                     m_return;
        std::string                      m_documentation;
        std::string                      m_category;

    };

}