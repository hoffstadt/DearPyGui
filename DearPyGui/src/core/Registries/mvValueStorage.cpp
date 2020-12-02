#include "mvValueStorage.h"
#include "PythonUtilities/mvPythonTranslator.h"
#include "mvApp.h"
#include "mvAppLog.h"

namespace Marvel
{

	PyObject* mvValueStorage::GetPyValue(const std::string& name)
	{

		if (!HasValue(name))
			return GetPyNone();

		switch (GetType(name))
		{

		case mvValueStorage::ValueTypes::Color:
		{
			float* fcolor = GetValue<std::array<float, 4>>(name)->data();
			mvColor color = { (int)(fcolor[0] * 255), (int)(fcolor[1] * 255),
							  (int)(fcolor[2] * 255), (int)(fcolor[3] * 255), true };
			return ToPyColor(color);
		}

		case mvValueStorage::ValueTypes::Int:
			return ToPyInt(*GetValue<int>(name));

		case mvValueStorage::ValueTypes::Int2:
			return ToPyIntList(GetValue<std::array<int, 2>>(name)->data(), 2);

		case mvValueStorage::ValueTypes::Int3:
			return ToPyIntList(GetValue<std::array<int, 3>>(name)->data(), 3);

		case mvValueStorage::ValueTypes::Int4:
			return ToPyIntList(GetValue<std::array<int, 4>>(name)->data(), 4);

		case mvValueStorage::ValueTypes::Float:
			return ToPyFloat(*GetValue<float>(name));

		case mvValueStorage::ValueTypes::Float2:
			return ToPyFloatList(GetValue<std::array<float, 2>>(name)->data(), 4);

		case mvValueStorage::ValueTypes::Float3:
			return ToPyFloatList(GetValue<std::array<float, 3>>(name)->data(), 3);

		case mvValueStorage::ValueTypes::Float4:
			return ToPyFloatList(GetValue<std::array<float, 4>>(name)->data(), 4);

		case mvValueStorage::ValueTypes::String:
			return ToPyString(*GetValue<std::string>(name));

		case mvValueStorage::ValueTypes::Bool:
			return ToPyBool(*GetValue<bool>(name));

		case mvValueStorage::ValueTypes::FloatVect:
			return ToPyList(*GetValue<std::vector<float>>(name));

		case mvValueStorage::ValueTypes::Time:
			return ToPyTime(*GetValue<tm>(name));

		default:
			return GetPyNone();
		}
	}

	bool mvValueStorage::SetPyValue(const std::string& name, PyObject* value)
	{

		if (!HasValue(name))
			return false;

		switch (GetType(name))
		{

		case mvValueStorage::ValueTypes::Int:
			*GetValue<int>(name) = ToInt(value);
			return true;

		case mvValueStorage::ValueTypes::Int2:
		{
			std::vector<int> temp = ToIntVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 1)
					break;
				GetValue<std::array<int, 2>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Int3:
		{
			std::vector<int> temp = ToIntVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 2)
					break;
				GetValue<std::array<int, 3>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Int4:
		{
			std::vector<int> temp = ToIntVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 3)
					break;
				GetValue<std::array<int, 3>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Float:
			*GetValue<float>(name) = ToFloat(value);
			return true;

		case mvValueStorage::ValueTypes::Float2:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 1)
					break;
				GetValue<std::array<float, 2>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Float3:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 2)
					break;
				GetValue<std::array<float, 3>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Float4:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 3)
					break;
				GetValue<std::array<float, 4>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Color:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 3)
					break;
				GetValue<std::array<float, 4>>(name)->data()[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::String:
			*GetValue<std::string>(name) = ToString(value);
			return true;

		case mvValueStorage::ValueTypes::Bool:
			*GetValue<bool>(name) = ToBool(value);
			return true;

		case mvValueStorage::ValueTypes::FloatVect:
			*GetValue<std::vector<float>>(name) = ToFloatVect(value);
			return true;

		case mvValueStorage::ValueTypes::Time:
			*GetValue<tm>(name) = ToTime(value);
			ImPlot::GetGmtTime(*GetValue<ImPlotTime>(name), GetValue<tm>(name));
			return true;

		default:
			return false;
		}
	}

	void mvValueStorage::AddPyValue(const std::string& name, PyObject* value)
	{

		mvGlobalIntepreterLock gil;

		// bool
		if (PyBool_Check(value))
			AddValue<bool>(name, PyLong_AsLong(value));

			// int
		else if (PyLong_Check(value))
			AddValue<int>(name, PyLong_AsLong(value));

			// float
		else if (PyFloat_Check(value))
			AddValue<float>(name, (float)PyFloat_AsDouble(value));

			// string
		else if (PyUnicode_Check(value))
			AddValue<std::string>(name, _PyUnicode_AsString(value));

			// list
		else if (PyList_Check(value))
		{

			if (PyList_Size(value) == 0)
				return;
			else if (PyList_Size(value) == 1)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddValue<int>(name, PyLong_AsLong(item));
				else if (PyFloat_Check(item))
					AddValue<float>(name, (float)PyFloat_AsDouble(item));
				else
					return;
			}

			else if (PyList_Size(value) == 2)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddValue<std::array<int, 2>>(name, { ToInt(PyList_GetItem(value, 0)),
														 ToInt(PyList_GetItem(value, 1)) });
				else if (PyFloat_Check(item))
					AddValue<std::array<float, 2>>(name,
						{ ToFloat(PyList_GetItem(value, 0)),
						  ToFloat(PyList_GetItem(value, 1)) });
				else
					return;
			}

			else if (PyList_Size(value) == 3)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddValue<std::array<int, 3>>(name, { ToInt(PyList_GetItem(value, 0)),
														 ToInt(PyList_GetItem(value, 1)),
														 ToInt(PyList_GetItem(value, 2)) });
				else if (PyFloat_Check(item))
					AddValue<std::array<float, 3>>(name,
						{ ToFloat(PyList_GetItem(value, 0)),
						  ToFloat(PyList_GetItem(value, 1)),
						  ToFloat(PyList_GetItem(value, 2)) });
				else
					return;
			}

			else if (PyList_Size(value) == 4)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddValue<std::array<int, 4>>(name, { ToInt(PyList_GetItem(value, 0)),
														 ToInt(PyList_GetItem(value, 1)),
														 ToInt(PyList_GetItem(value, 2)),
														 ToInt(PyList_GetItem(value, 3)) });
				else if (PyFloat_Check(item))
					AddValue<std::array<float, 4>>(name,
						{ ToFloat(PyList_GetItem(value, 0)),
						  ToFloat(PyList_GetItem(value, 1)),
						  ToFloat(PyList_GetItem(value, 2)),
						  ToFloat(PyList_GetItem(value, 3)) });
				else
					return;
			}

			else
				AddValue<std::vector<float>>(name, ToFloatVect(value));
		}

			// tuple (color)
		else if (PyTuple_Check(value))
		{

			if (PyTuple_Size(value) == 0)
				return;
			else if (PyTuple_Size(value) == 1)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddValue<std::array<float, 4>>(
						name, { (float)PyFloat_AsDouble(item), 0.0f, 0.0f, 255.0f });
				else
					return;
			}

			else if (PyTuple_Size(value) == 2)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddValue<std::array<float, 4>>(
						name, { ToFloat(PyTuple_GetItem(value, 0)),
								ToFloat(PyTuple_GetItem(value, 1)), 0.0f, 255.0f });
				else
					return;
			}

			else if (PyTuple_Size(value) == 3)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddValue<std::array<float, 4>>(
						name, { ToFloat(PyTuple_GetItem(value, 0)),
								ToFloat(PyTuple_GetItem(value, 1)),
								ToFloat(PyTuple_GetItem(value, 1)), 255.0f });
				else
					return;
			}

			else if (PyTuple_Size(value) == 4)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddValue<std::array<float, 4>>(name,
						{ ToFloat(PyTuple_GetItem(value, 0)),
						  ToFloat(PyTuple_GetItem(value, 1)),
						  ToFloat(PyTuple_GetItem(value, 2)),
						  ToFloat(PyTuple_GetItem(value, 3)) });
				else
					return;
			}
		}
	}

	bool mvValueStorage::HasValue(const std::string& name)
	{
		return s_itemStorage.count(name) == 1;
	}

	mvValueStorage::ValueTypes mvValueStorage::GetType(const std::string& name)
	{
		if (HasValue(name))
		{
			return s_typeStorage[name];
		}
		return mvValueStorage::ValueTypes::None;
	}

	void mvValueStorage::IncrementRef(const std::string& name)
	{
		if (HasValue(name))
		{
			s_refStorage[name]++;
//    std::cout << "++ IncrementRef # item \t[ " << name.c_str()
//              << " ]\t = " << s_refStorage[name] << "\n";
		}
	}

	void mvValueStorage::DecrementRef(const std::string& name)
	{
		if (HasValue(name))
		{
			s_refStorage[name]--;
//	  std::cout << "-- DecrementRef # item \t[ " << name.c_str()
//				<< " ]\t = " << s_refStorage[name] << "\n";

			if (s_refStorage[name] <= 0)
			{
//      std::cout << "-- DecrementRef # item \t[ " << name.c_str()
//                << " ]\t dropped last reference."
//                << "\n";
				switch (GetType(name))
				{
				case mvValueStorage::ValueTypes::Color:
				{
					DeleteValue<std::array<float, 4>>(name);
					break;
				}
				case mvValueStorage::ValueTypes::TypeCount:
					break;
				case mvValueStorage::ValueTypes::None:
					break;
				case mvValueStorage::ValueTypes::Int:
					DeleteValue<int>(name);
				case mvValueStorage::ValueTypes::Int2:
					DeleteValue<std::array<int, 2>>(name);

				case mvValueStorage::ValueTypes::Int3:
					DeleteValue<std::array<int, 3>>(name);

				case mvValueStorage::ValueTypes::Int4:
					DeleteValue<std::array<int, 4>>(name);

				case mvValueStorage::ValueTypes::Float:
					DeleteValue<float>(name);

				case mvValueStorage::ValueTypes::Float2:
					DeleteValue<std::array<float, 2>>(name);

				case mvValueStorage::ValueTypes::Float3:
					DeleteValue<std::array<float, 3>>(name);

				case mvValueStorage::ValueTypes::Float4:
					DeleteValue<std::array<float, 4>>(name);

				case mvValueStorage::ValueTypes::String:
					DeleteValue<std::string>(name);

				case mvValueStorage::ValueTypes::Bool:
					DeleteValue<bool>(name);

				case mvValueStorage::ValueTypes::FloatVect:
					DeleteValue<std::vector<float>>(name);

				case mvValueStorage::ValueTypes::Time:
					DeleteValue<tm>(name);
				}
			}
		}
	}

	std::mutex mvValueStorage::s_mutex;
	std::unordered_map<std::string, mvValueStorage::ValueTypes>
		mvValueStorage::s_typeStorage;
	std::unordered_map<std::string, int> mvValueStorage::s_refStorage = {
		{ "common", 1 },
	};
	std::set<std::string> mvValueStorage::s_itemStorage = { "common" };

	template<>
	std::unordered_map<std::string, int> mvValueStorage::s_value_store<int> = {
		{ "common", {}}};
	template<>
	std::unordered_map<std::string, std::array<int, 2>>
		mvValueStorage::s_value_store<std::array<int, 2>> = {{ "common", { 0, 0 }}};
	template<>
	std::unordered_map<std::string, std::array<int, 3>>
		mvValueStorage::s_value_store<std::array<int, 3>> = {{ "common", { 0, 0, 0 }}};
	template<>
	std::unordered_map<std::string, std::array<int, 4>>
		mvValueStorage::s_value_store<std::array<int, 4>> = {
		{ "common", { 0, 0, 0, 0 }}};
	template<>
	std::unordered_map<std::string, float> mvValueStorage::s_value_store<float> = {
		{ "common", 0.0f }};
	template<>
	std::unordered_map<std::string, std::array<float, 2>>
		mvValueStorage::s_value_store<std::array<float, 2>> = {
		{ "common", { 0.0f, 0.0f }}};
	template<>
	std::unordered_map<std::string, std::array<float, 3>>
		mvValueStorage::s_value_store<std::array<float, 3>> = {
		{ "common", { 0.0f, 0.0f, 0.0f }}};
	template<>
	std::unordered_map<std::string, std::array<float, 4>>
		mvValueStorage::s_value_store<std::array<float, 4>> = {
		{ "common", { 0.0f, 0.0f }}};
	template<>
	std::unordered_map<std::string, bool> mvValueStorage::s_value_store<bool> = {
		{ "common", true }};
	template<>
	std::unordered_map<std::string, std::string>
		mvValueStorage::s_value_store<std::string> = {{ "common", "" }};
	template<>
	std::unordered_map<std::string, std::vector<float>>
		mvValueStorage::s_value_store<std::vector<float>> = {
		{ "common", { 0.0f, 0.0f }}};
	template<>
	std::unordered_map<std::string, tm> mvValueStorage::s_value_store<tm> = {
		{ "common", {}}};
	template<>
	std::unordered_map<std::string, ImPlotTime>
		mvValueStorage::s_value_store<ImPlotTime> = {{ "common", ImPlotTime() }};

} // namespace Marvel
