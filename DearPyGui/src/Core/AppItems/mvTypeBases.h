#pragma once

//-----------------------------------------------------------------------------
// mvTypeBases
//
//     - This file contains abstract classes for concrete mvAppItem 
//       implementations which share common underlying data types.
//     
//-----------------------------------------------------------------------------

#include <utility>
#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvAppItem.h"
#include "Core/mvUtilities.h"
#include "mvAppLog.h"
#include "Core/mvDataStorage.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvBoolItemBase
	//-----------------------------------------------------------------------------
	class mvBoolItemBase : public mvAppItem
	{

	public:

		mvBoolItemBase(const std::string& parent, const std::string& name, bool value)
			: mvAppItem(parent, name), m_value(value)
		{
		}

		void setPyValue(PyObject* value) override
		{ 
			m_value = mvPythonTranslator::ToBool(value, m_name + " requires a bool value."); 
		}

		[[nodiscard]] PyObject* getPyValue() const override
		{
			return mvPythonTranslator::ToPyBool(m_value);
		}

		[[nodiscard]] bool getValue() const { return m_value; }
		void               setValue(bool value) { m_value = value; }

	protected:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvStringItemBase
	//-----------------------------------------------------------------------------
	class mvStringItemBase : public mvAppItem
	{

	public:

		mvStringItemBase(const std::string& parent, const std::string& name, std::string  value)
			: mvAppItem(parent, name), m_value(std::move(value))
		{
		}

		void setPyValue(PyObject* value) override
		{
			m_value = mvPythonTranslator::ToString(value, m_name + " requires a string value.");
		}

		[[nodiscard]] PyObject* getPyValue() const override
		{
			return mvPythonTranslator::ToPyString(m_value);
		}

		[[nodiscard]] const std::string& getValue() const { return m_value; }
		inline void                      setValue(const std::string& value) { m_value = value; }

	protected:

		std::string m_value;

	};

	//-----------------------------------------------------------------------------
	// mvIntItemBase
	//-----------------------------------------------------------------------------
	class mvIntItemBase : public mvAppItem
	{

	public:

		mvIntItemBase(const std::string& parent, const std::string& name, unsigned count,
			int x, int y=0, int z=0, int w=0)
			: mvAppItem(parent, name), m_valuecount(count)
		{
			m_value.push_back(x);
			m_value.push_back(y);
			m_value.push_back(z);
			m_value.push_back(w);
		}

		void setPyValue(PyObject* value) override
		{

			if (m_valuecount == 1)
				m_value[0] = mvPythonTranslator::ToInt(value);

			else
				m_value = mvPythonTranslator::ToIntVect(value, " requires a list or tuple of integers.");

		}

		[[nodiscard]] PyObject* getPyValue() const override
		{

			if (!m_dataSource.empty())
			{
				if (m_valuecount == 1)
					return mvPythonTranslator::ToPyInt(m_value[0]);

				else
				{

					if (!mvDataStorage::HasData(m_dataSource))
						mvDataStorage::AddData(m_dataSource, mvPythonTranslator::ToPyList(m_value));
					else
						mvPythonTranslator::UpdatePyIntList(mvDataStorage::GetData(m_dataSource), m_value);

					return mvDataStorage::GetData(m_dataSource);
				}
			}

			if (m_valuecount == 1)
				return mvPythonTranslator::ToPyInt(m_value[0]);

			else
				return mvPythonTranslator::ToPyList(m_value);
		}

	protected:

		unsigned         m_valuecount = 1;
		std::vector<int> m_value;

	};

	//-----------------------------------------------------------------------------
	// mvFloatItemBase
	//-----------------------------------------------------------------------------
	class mvFloatItemBase : public mvAppItem
	{

	public:

		mvFloatItemBase(const std::string& parent, const std::string& name, unsigned count, 
			float x, float y=0.0f, float z=0.0f, float w=0.0f)
			: mvAppItem(parent, name), m_valuecount(count)
		{
			m_value.push_back(x);
			m_value.push_back(y);
			m_value.push_back(z);
			m_value.push_back(w);
		}

		void setPyValue(PyObject* value) override
		{
			if (m_valuecount == 1)
				m_value[0] = mvPythonTranslator::ToFloat(value);

			else
				m_value = mvPythonTranslator::ToFloatVect(value, " requires a list or tuple of floats.");

		}

		[[nodiscard]] PyObject* getPyValue() const override
		{

			if (!m_dataSource.empty())
			{
				if (m_valuecount == 1)
					return mvPythonTranslator::ToPyFloat(m_value[0]);

				else
				{
					if (!mvDataStorage::HasData(m_dataSource))
						mvDataStorage::AddData(m_dataSource, mvPythonTranslator::ToPyList(m_value));
					else
						mvPythonTranslator::UpdatePyFloatList(mvDataStorage::GetData(m_dataSource), m_value);

					return mvDataStorage::GetData(m_dataSource);
				}
			}

			if (m_valuecount == 1)
				return mvPythonTranslator::ToPyFloat(m_value[0]);

			else
				return mvPythonTranslator::ToPyList(m_value);
		}

	protected:

		unsigned           m_valuecount = 1;
		std::vector<float> m_value;

	};


	//-----------------------------------------------------------------------------
	// mvColorItemBase
	//-----------------------------------------------------------------------------
	class mvColorItemBase : public mvAppItem
	{

	public:

		mvColorItemBase(const std::string& parent, const std::string& name, mvColor color)
			: mvAppItem(parent, name)
		{
			m_value.push_back((float)color.r/255.0f);
			m_value.push_back((float)color.g/255.0f);
			m_value.push_back((float)color.b/255.0f);
			m_value.push_back((float)color.a/255.0f);
		}

		void setPyValue(PyObject* value) override
		{
			auto ints = mvPythonTranslator::ToFloatVect(value, " requires a list or tuple of integers");
			for (size_t i = 0; i < ints.size(); i++)
			{
				if (i > 3)
					break;
				m_value[i] = ints[i] / 255.0f;
			}		
		}

		[[nodiscard]] PyObject* getPyValue() const override
		{
			std::vector<int> ints;
			for (const auto& item : m_value)
				ints.push_back(item * 255);

			if (!m_dataSource.empty())
			{
				if (!mvDataStorage::HasData(m_dataSource))
					mvDataStorage::AddData(m_dataSource, mvPythonTranslator::ToPyList(ints));
				else
					mvPythonTranslator::UpdatePyIntList(mvDataStorage::GetData(m_dataSource), ints);

				return mvDataStorage::GetData(m_dataSource);
			}

			return mvPythonTranslator::ToPyList(ints);
		}

	protected:

		std::vector<float> m_value;

	};

}