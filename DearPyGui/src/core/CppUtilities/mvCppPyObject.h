#pragma once
#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include "mvCore.h"
#include <time.h>

class PyContextManager
{
public:

	PyContextManager() = default;

	PyContextManager(const PyContextManager& other) = delete;
	PyContextManager& operator=(const PyContextManager& other) = delete;
	PyContextManager& operator=(PyContextManager&& other) = delete;

	PyContextManager(PyContextManager&& other) noexcept
	{
		m_moved = true;
	}

	~PyContextManager();


private:

	bool m_moved = false;
};

class PyObject
{

	public:

		using PyVariant = std::variant<
			bool,
			int,
			float,
			const char*,
			std::string,
			Marvel::mvVec2,
			Marvel::mvVec4,
			Marvel::mvColor,
			tm,
			std::pair<std::string, std::string>,
			std::pair<int, int>,
			std::pair<int, float>,
			std::pair<float, float>,
			std::tuple<int, float, float>,
			std::vector<Marvel::mvVec2>,
			std::vector<int>,
			std::vector<float>,
			std::vector<std::string>,
			std::vector<std::vector<std::string>>,
			std::vector<std::pair<std::string, std::string>>,
			std::vector<std::pair<int, int>>,
			std::unordered_map<std::string, PyObject>,
			void*,
			std::nullptr_t>;

		PyObject() = default;

		PyObject(PyVariant var)
			:
			data(var)
		{
		}

		PyObject(const std::unordered_map<std::string, PyObject>& var)
			:
			data(var)
		{
		}

		PyVariant data;

};

