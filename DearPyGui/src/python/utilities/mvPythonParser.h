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
#include <cstring>
#include <vector>
#include <fstream>
#include <map>
#include <assert.h>

// forward declare PyObject
// as suggested on the python mailing list
// http://mail.python.org/pipermail/python-dev/2003-August/037601.html
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

enum class mvPyDataType
{
    None = 0,
    Integer, Float, Double, String, Bool, Object, Callable, Dict,
    IntList, FloatList, DoubleList, StringList, ListAny,
    ListListInt, ListFloatList, ListDoubleList, ListStrList, UUID,
    UUIDList, Long,
    Any
};

enum class mvArgType
{
    REQUIRED_ARG = 0,
    POSITIONAL_ARG,
    KEYWORD_ARG,
    DEPRECATED_RENAME_KEYWORD_ARG,
    DEPRECATED_REMOVE_KEYWORD_ARG
};

enum CommonParserArgs
{
    MV_PARSER_ARG_ID            = 1 << 1,
    MV_PARSER_ARG_WIDTH         = 1 << 2,
    MV_PARSER_ARG_HEIGHT        = 1 << 3,
    MV_PARSER_ARG_INDENT        = 1 << 4,
    MV_PARSER_ARG_PARENT        = 1 << 5,
    MV_PARSER_ARG_BEFORE        = 1 << 6,
    MV_PARSER_ARG_SOURCE        = 1 << 7,
    MV_PARSER_ARG_CALLBACK      = 1 << 8,
    MV_PARSER_ARG_SHOW          = 1 << 9,
    MV_PARSER_ARG_ENABLED       = 1 << 10,
    MV_PARSER_ARG_POS           = 1 << 11,
    MV_PARSER_ARG_DROP_CALLBACK = 1 << 12,
    MV_PARSER_ARG_DRAG_CALLBACK = 1 << 13,
    MV_PARSER_ARG_PAYLOAD_TYPE  = 1 << 14,
    MV_PARSER_ARG_TRACKED       = 1 << 15,
    MV_PARSER_ARG_FILTER        = 1 << 16,
    MV_PARSER_ARG_SEARCH_DELAY  = 1 << 17
};

struct mvPythonDataElement
{
    mvPyDataType type = mvPyDataType::None;
    const char* name = "";
    mvArgType   arg_type = mvArgType::REQUIRED_ARG;
    const char* default_value = "...";
    const char* description = "";
    const char* new_name = "";
};

struct mvPythonParser
{
    std::vector<mvPythonDataElement> required_elements;
    std::vector<mvPythonDataElement> optional_elements;
    std::vector<mvPythonDataElement> keyword_elements;
    std::vector<mvPythonDataElement> deprecated_elements;
    std::vector<char>                formatstring;
    std::vector<const char*>         keywords;
    std::string                      documentation;
    bool                             unspecifiedKwargs = false;
    bool                             createContextManager = false;
    bool                             internal = false;
    std::string                      about;
    mvPyDataType                     returnType = mvPyDataType::None;
    std::vector<std::string>         category;
};

struct mvPythonParserSetup
{
    std::string              about = "Undocumented";
    mvPyDataType             returnType = mvPyDataType::None;
    std::vector<std::string> category = {"General"};
    bool                     createContextManager = false;
    bool                     unspecifiedKwargs = false;
    bool                     internal = false;
};

mvPythonParser FinalizeParser(const mvPythonParserSetup& setup, const std::vector<mvPythonDataElement>& args);
bool           Parse(const mvPythonParser& parser, PyObject* args, PyObject* kwargs, const char* message, ...);
const char*    PythonDataTypeActual(mvPyDataType type);
void           AddCommonArgs(std::vector<mvPythonDataElement>& args, CommonParserArgs argsFlags);

// arguments checks
bool VerifyRequiredArguments  (const mvPythonParser& parser, PyObject* args);
bool VerifyPositionalArguments(const mvPythonParser& parser, PyObject* args);
bool VerifyKeywordArguments   (const mvPythonParser& parser, PyObject* args);
bool VerifyArgumentCount      (const mvPythonParser& parser, PyObject* args);

// file generation
void GenerateStubFile        (const std::string& directory);
void GenerateDearPyGuiFile   (const std::string& directory);
void GenerateDearPyGuiFileRTD(const std::string& directory);