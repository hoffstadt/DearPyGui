#pragma once

#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <functional>
#include <cstring>
#include <fstream>
#include <assert.h>
#include "mvCore.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

// forward declarations
class mvAppItem;

struct mvGlobalIntepreterLock
{
    int _gstate;

    mvGlobalIntepreterLock();
    ~mvGlobalIntepreterLock();

};

class mvPyObject
{

public:

	mvPyObject(PyObject* rawObject, bool borrowed=false);
	mvPyObject(mvPyObject&& other);
	mvPyObject& operator=(mvPyObject&& other);

	mvPyObject(const mvPyObject& other) = delete;
	mvPyObject& operator=(mvPyObject& other) = delete;

	~mvPyObject();

	void addRef();
	void delRef();
	bool isOk() const { return m_ok; }

	operator PyObject* ();

private:

	PyObject* m_rawObject;
	bool      m_borrowed;
	bool      m_ok;
	bool      m_del = false;

};

// python reference counting using strict RAII, no implicit copy
// shared references to an object can use a shared_ptr<mvPyObjectStrict> or explicit copy()
class mvPyObjectStrict
{
    PyObject* m_rawObject = nullptr;

public:
    mvPyObjectStrict() = default;
    mvPyObjectStrict(PyObject* ptr, bool borrowed=true);
    mvPyObjectStrict(mvPyObjectStrict&& other) noexcept;

    mvPyObjectStrict& operator=(mvPyObjectStrict&& other) noexcept;

    ~mvPyObjectStrict();

    PyObject* operator*();
    explicit operator bool() const;

    // Takes the m_rawObject out of this container.
    // Shouldn't be done with an mvPyObjectStrict in a shared_ptr, you should
    // copy it out first.
    PyObject* steal();
    mvPyObjectStrict copy();
};

using mvPyObjectStrictPtr = std::shared_ptr<mvPyObjectStrict>;

mvPyObjectStrictPtr mvPyObjectStrictNonePtr(void);

enum class mvErrorCode
{
    mvNone                = 1000,
    mvTextureNotFound     = 1001,
    mvIncompatibleType    = 1002,
    mvIncompatibleParent  = 1003,
    mvIncompatibleChild   = 1004,
    mvItemNotFound        = 1005,
    mvSourceNotFound      = 1006,
    mvSourceNotCompatible = 1007,
    mvWrongType           = 1008,
    mvContainerStackEmpty = 1009,
    mvStagingModeOff      = 1010,
    mvParentNotDeduced    = 1011,
};

void mvThrowPythonError(mvErrorCode code, const std::string& message);
void mvThrowPythonError(mvErrorCode code, const std::string& command, const std::string& message, mvAppItem* item);

bool isPyObject_String        (PyObject* obj);
bool isPyObject_Int           (PyObject* obj);
bool isPyObject_Float         (PyObject* obj);
bool isPyObject_Bool          (PyObject* obj);
bool isPyObject_StringList    (PyObject* obj);
bool isPyObject_ListStringList(PyObject* obj);
bool isPyObject_FloatList     (PyObject* obj);
bool isPyObject_ListFloatList (PyObject* obj);
bool isPyObject_IntList       (PyObject* obj);
bool isPyObject_ListIntList   (PyObject* obj);
bool isPyObject_Double        (PyObject* obj);
bool isPyObject_Callable      (PyObject* obj);
bool isPyObject_Dict          (PyObject* obj);
bool isPyObject_Any           (PyObject* obj);

// conversion to python
PyObject*   GetPyNone ();
PyObject*   ToPyUUID  (mvAppItem* item);
PyObject*   ToPyUUID  (mvUUID value);
PyObject*   ToPyLong  (long value);
PyObject*   ToPyInt   (int value);
PyObject*   ToPyFloat (float value);
PyObject*   ToPyDouble(double value);
PyObject*   ToPyBool  (bool value);
PyObject*   ToPyString(const std::string& value);
PyObject*   ToPyMPair (int x, float y);
PyObject*   ToPyMTrip (int i, float x, float y);
PyObject*   ToPyPair  (float x, float y);
PyObject*   ToPyPair  (double x, double y);
PyObject*   ToPyPairII(int x, int y);
PyObject*   ToPyPair  (const std::string& x, const std::string& y);
PyObject*   ToPyList  (const std::vector<mvVec2>& value);
PyObject*   ToPyList  (const std::vector<mvVec4>& value);
PyObject*   ToPyList  (const std::vector<int>& value);
PyObject*   ToPyList  (const std::vector<mvUUID>& value);
PyObject*   ToPyList  (const std::vector<float>& value);
PyObject*   ToPyList  (const std::vector<double>& value);
PyObject*   ToPyList  (const std::vector<std::vector<float>>& value);
PyObject*   ToPyList  (const std::vector<std::vector<double>>& value);
PyObject*   ToPyList  (const std::vector<std::string>& value);
PyObject*   ToPyList  (const std::vector<std::vector<std::string>>& value);
PyObject*   ToPyList  (const std::vector<std::pair<int, int>>& value);
PyObject*   ToPyList  (const std::vector<std::pair<std::string, std::string>>& value);
PyObject*   ToPyColor (const mvColor& color);
PyObject*   ToPyTime  (const tm& time);

PyObject*   ToPyIntList  (const int* value, int count);
PyObject*   ToPyFloatList(const float* value, int count);
PyObject*   ToPyFloatList(const double* value, int count);

// updates
void        UpdatePyIntList         (PyObject* pyvalue, const std::vector<int>& value);
void        UpdatePyFloatList       (PyObject* pyvalue, const std::vector<float>& value);
void        UpdatePyStringStringList(PyObject* pyvalue, const std::vector<std::vector<std::string>>& value);

// conversion to c++
int         ToInt   (PyObject* value, const std::string& message = "Type must be an integer.");
float       ToFloat (PyObject* value, const std::string& message = "Type must be a float.");
double      ToDouble(PyObject* value, const std::string& message = "Type must be a double.");
bool        ToBool  (PyObject* value, const std::string& message = "Type must be a bool.");
mvColor     ToColor (PyObject* value, const std::string& message = "Type must be a list or tuple of ints.");
ImPlotPoint ToPoint (PyObject* value, const std::string& message = "Type must be a list or tuple of doubles.");
mvVec2      ToVec2  (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
mvVec4      ToVec4  (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
std::string ToString(PyObject* value, const std::string& message = "Type must be a string.");
mvUUID      ToUUID  (PyObject* value, const std::string& message = "Type must be a UUID.");
tm          ToTime  (PyObject* value, const std::string& message = "Type must be a dict");

std::vector<mvVec2>                              ToVectVec2           (PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.");
std::vector<mvVec4>                              ToVectVec4           (PyObject* value, const std::string& message = "Type must be a list/tuple of list/tuple.");
std::pair<std::vector<float>, std::vector<float>>ToPairVec            (PyObject* value, const std::string& message = "Type must be a list/tuple of two list/tuple.");
std::vector<int>                                 ToIntVect            (PyObject* value, const std::string& message = "Type must be a list or tuple of integers.");
std::vector<mvUUID>                              ToUUIDVect           (PyObject* value, const std::string& message = "Type must be a list or tuple of integers.");
std::vector<unsigned char>                       ToUCharVect          (PyObject* value, const std::string& message = "Type must be a list or tuple of ints.");
std::vector<float>                               ToFloatVect          (PyObject* value, const std::string& message = "Type must be a list or tuple of floats.");
std::vector<double>                              ToDoubleVect         (PyObject* value, const std::string& message = "Type must be a list or tuple of doubles.");
std::vector<std::string>                         ToStringVect         (PyObject* value, const std::string& message = "Type must be a list or tuple of strings.");
std::vector<std::pair<int, int>>                 ToVectInt2           (PyObject* value, const std::string& message = "Type must be an list/tuple of integer.");
std::vector<std::pair<std::string, std::string>> ToVectPairString     (PyObject* value, const std::string& message = "Type must be an list/tuple of string pairs.");
std::vector<std::vector<std::string>>            ToVectVectString     (PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of strings.");
std::vector<std::vector<float>>                  ToVectVectFloat      (PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of floats.");
std::vector<std::vector<int>>                    ToVectVectInt        (PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of ints.");
std::vector<std::vector<double>>                 ToVectVectDouble     (PyObject* value, const std::string& message = "Type must be an list/tuple of list/tuple of doubles.");
std::vector<std::pair<std::string, float>>       ToVectPairStringFloat(PyObject* value, const std::string& message = "Type must be an list/tuple of str,float pairs.");

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
