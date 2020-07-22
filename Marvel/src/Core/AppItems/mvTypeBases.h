#pragma once

//-----------------------------------------------------------------------------
// mvTypeBases
//
//     - This file contains abstract classes for concrete mvAppItem 
//       implementations which share common underlying data types.
//     
//-----------------------------------------------------------------------------

#include "Core/PythonInterfaces/mvInterfaceRegistry.h"
#include "Core/PythonInterfaces/mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItem.h"
#include "Core/mvUtilities.h"
#include "Core/StandardWindows/mvAppLog.h"

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

		virtual void setPyValue(PyObject* value) override 
		{ 
			m_value = mvPythonTranslator::ToBool(value, m_name + " requires a bool value."); 
		}

		virtual PyObject* getPyValue() const override
		{
			return mvPythonTranslator::ToPyBool(m_value);
		}

		inline bool getValue() const { return m_value; }
		inline void setValue(bool value) { m_value = value; }

	protected:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvStringItemBase
	//-----------------------------------------------------------------------------
	class mvStringItemBase : public mvAppItem
	{

	public:

		mvStringItemBase(const std::string& parent, const std::string& name, const std::string& value)
			: mvAppItem(parent, name), m_value(value)
		{
		}

		virtual void setPyValue(PyObject* value) override
		{
			m_value = mvPythonTranslator::ToString(value, m_name + " requires a string value.");
		}

		virtual PyObject* getPyValue() const override
		{
			return mvPythonTranslator::ToPyString(m_value);
		}

		inline const std::string& getValue() const { return m_value; }
		inline void setValue(const std::string& value) { m_value = value; }

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

		virtual void setPyValue(PyObject* value) override
		{

			if (m_valuecount == 1)
				m_value[0] = mvPythonTranslator::ToInt(value);

			else
				m_value = mvPythonTranslator::ToIntVect(value, " requires a list or tuple of integers.");

		}

		virtual PyObject* getPyValue() const override
		{
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

		virtual void setPyValue(PyObject* value) override
		{
			if (m_valuecount == 1)
				m_value[0] = mvPythonTranslator::ToFloat(value);

			else
				m_value = mvPythonTranslator::ToFloatVect(value, " requires a list or tuple of floats.");
		}

		virtual PyObject* getPyValue() const override
		{

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
			m_value.push_back(color.r/255.0f);
			m_value.push_back(color.g/255.0f);
			m_value.push_back(color.b/255.0f);
			m_value.push_back(color.a/255.0f);
		}

		virtual void setPyValue(PyObject* value) override
		{
			auto ints = mvPythonTranslator::ToIntVect(value, " requires a list or tuple of integers");
			for (int i = 0; i < ints.size(); i++)
			{
				if (i > 3)
					break;
				m_value[i] = ints[i] / 255.0f;
			}
				
		}

		virtual PyObject* getPyValue() const override
		{
			std::vector<int> ints;
			for (const auto& item : m_value)
				ints.push_back(item * 255);
			return mvPythonTranslator::ToPyList(ints);
		}

	protected:

		std::vector<float> m_value;

	};

}