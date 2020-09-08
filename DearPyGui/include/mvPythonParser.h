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
#include "mvCore.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Enums and Helper Functions
    //-----------------------------------------------------------------------------
    enum class mvPythonDataType
    {
        String, Integer, Float, Bool, StringList, FloatList, Optional,
        Object, IntList, KeywordOnly
    };

    //-----------------------------------------------------------------------------
    // mvPythonDataElement
    //-----------------------------------------------------------------------------
    struct mvPythonDataElement
    {
        const char*      name;
        mvPythonDataType type;
        std::string      description;

        mvPythonDataElement(mvPythonDataType type, const char* name = "", std::string  description = "");
        [[nodiscard]] char getSymbol() const;
    };

    const char* PythonDataTypeActual(mvPythonDataType type);

    //-----------------------------------------------------------------------------
    // mvPythonParser
    //-----------------------------------------------------------------------------
    class mvPythonParser
    {

    public:

        mvPythonParser() = default;

        mvPythonParser(const std::initializer_list<mvPythonDataElement>& elements, 
            std::string  about = "", std::string  returnType = "None",
            std::string  category = "App");

        bool                             parse(PyObject* args, PyObject* kwargs, const char* message, ...);
        [[nodiscard]] const char*        getDocumentation                () const { return m_documentation.c_str(); }
        [[nodiscard]] const std::string& getCategory                     () const { return m_category; }
        [[nodiscard]] const std::string& getReturnType                   () const { return m_return; }
        [[nodiscard]] const std::string& getAbout                        () const { return m_about; }
        [[nodiscard]] const std::vector<mvPythonDataElement>& getElements() const { return m_elements; }

        void buildDocumentation();

    private:

        std::vector<mvPythonDataElement> m_elements;
        std::vector<char>                m_formatstring;
        std::vector<const char*>         m_keywords;
        bool                             m_optional = false; // check if optional has been found already
        bool                             m_keyword  = false; // check if keyword has been found already
        std::string                      m_about;
        std::string                      m_return;
        std::string                      m_documentation;
        std::string                      m_category;

    };

    void GenerateStubFile(const std::string& file);
}