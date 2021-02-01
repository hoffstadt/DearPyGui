#include "mvCppTranslator.h"
#include <utility>

namespace Marvel {

	PyObject* PyDict_GetItemString(PyObject* dict, const char* name)
	{
		auto& result = std::get<std::unordered_map<std::string, PyObject>>(dict->data);

		// Key is not present 
		if (result.find(std::string(name)) == result.end())
			return nullptr;

		return &result.at(std::string(name));
	}

	void PyDict_SetItemString(PyObject* dict, const char* name, PyObject* value)
	{
		auto result = std::get<std::unordered_map<std::string, PyObject>>(dict->data);
		result.at(std::string(name)).data = value->data;
	}

	PyObject* GetPyNone()
	{
		auto var = new PyObject(nullptr);
		return var;
	}

	PyObject* ToPyString(const std::string& value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyFloat(float value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyInt(int value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyBool(bool value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyMPair(int x, float y)
	{
		auto var = new PyObject(std::make_pair<>(x, y));
		return var;
	}

	PyObject* ToPyMTrip(int i, float x, float y)
	{
		auto var = new PyObject(std::make_tuple<>(i, x, y));
		return var;
	}

	PyObject* ToPyPair(float x, float y)
	{
		auto var = new PyObject(std::make_pair<>(x, y));
		return var;
	}

	PyObject* ToPyPairII(int x, int y)
	{
		auto var = new PyObject(std::make_pair<>(x, y));
		return var;
	}

	PyObject* ToPyPair(const std::string& x, const std::string& y)
	{
		auto var = new PyObject(std::make_pair<>(x, y));
		return var;
	}

	PyObject* ToPyList(const std::vector<mvVec2>& value) 
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyList(const std::vector<int>& value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyList(const std::vector<float>& value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyList(const std::vector<std::string>& value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyList(const std::vector<std::vector<std::string>>& value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyList(const std::vector<std::pair<int, int>>& value)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyColor(const mvColor& color)
	{
		auto var = new PyObject(color);
		return var;
	}

	PyObject* ToPyTime(const tm& time)
	{
		auto var = new PyObject(time);
		return var;
	}

	PyObject* ToPyIntList(int* value, int count)
	{
		auto var = new PyObject(value);
		return var;
	}

	PyObject* ToPyFloatList(float* value, int count)
	{
		auto var = new PyObject(value);
		return var;
	}

	int ToInt(PyObject* value, const std::string& message)
	{
		return std::get<int>(value->data);
	}

	float ToFloat(PyObject* value, const std::string& message)
	{
		return std::get<float>(value->data);
	}

	bool ToBool(PyObject* value, const std::string& message)
	{
		return std::get<bool>(value->data);
	}

	mvColor ToColor(PyObject* value, const std::string& message)
	{
		return std::get<mvColor>(value->data);
	}

	mvVec2 ToVec2(PyObject* value, const std::string& message)
	{
		return std::get<mvVec2>(value->data);
	}

	mvVec4 ToVec4(PyObject* value, const std::string& message)
	{
		return std::get<mvVec4>(value->data);
	}

	std::string ToString(PyObject* value, const std::string& message)
	{
		return std::get<std::string>(value->data);
	}

	tm ToTime(PyObject* value, const std::string& message)
	{
		return std::get<tm>(value->data);
	}

	std::vector<int> ToIntVect(PyObject* value, const std::string& message)
	{
		return std::get<std::vector<int>>(value->data);
	}

	std::vector<float> ToFloatVect(PyObject* value, const std::string& message)
	{
		return std::get<std::vector<float>>(value->data);
	}

	std::vector<std::string> ToStringVect(PyObject* value, const std::string& message)
	{
		return std::get<std::vector<std::string>>(value->data);
	}

	std::vector<std::pair<std::string, std::string>> ToVectPairString(PyObject* value, const std::string& message)
	{
		return std::get<std::vector<std::pair<std::string, std::string>>>(value->data);
	}

	std::vector<mvVec2> ToVectVec2(PyObject* value, const std::string& message)
	{
		return std::get<std::vector<Marvel::mvVec2>>(value->data);
	}

	std::vector<std::vector<std::string>> ToVectVectString(PyObject* value, const std::string& message)
	{
		return std::get<std::vector<std::vector<std::string>>>(value->data);
	}

}