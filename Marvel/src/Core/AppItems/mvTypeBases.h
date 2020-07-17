#pragma once

#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItem.h"
#include "Core/mvUtilities.h"
#include "Core/StandardWindows/mvAppLog.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvNoneItemBase
	//-----------------------------------------------------------------------------
	class mvNoneItemBase : public mvAppItem
	{

	public:

		mvNoneItemBase(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name){}

		virtual void      setPyValue(PyObject* value) override{}
		virtual PyObject* getPyValue() const override { Py_RETURN_NONE;}

	};

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
			PyGILState_STATE gstate = PyGILState_Ensure();

			if (!PyLong_Check(value))
			{
				PyGILState_Release(gstate);
				mvAppLog::getLogger()->LogError(m_name + " type must be a bool.");
				return;
			}

			m_value = PyLong_AsLong(value); 

			PyGILState_Release(gstate);
		}

		virtual PyObject* getPyValue() const override
		{
			PyGILState_STATE gstate = PyGILState_Ensure();

			PyObject* pvalue = Py_BuildValue("i", m_value);

			PyGILState_Release(gstate);
			return pvalue;
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
			PyGILState_STATE gstate = PyGILState_Ensure();

			if (!PyUnicode_Check(value))
			{
				PyGILState_Release(gstate);
				mvAppLog::getLogger()->LogError(m_name + " type must be a string");
				return;
			}

			m_value = PyUnicode_AsUTF8(value);

			PyGILState_Release(gstate);
		}

		virtual PyObject* getPyValue() const override
		{
			PyGILState_STATE gstate = PyGILState_Ensure();

			PyObject* pvalue = Py_BuildValue("s", m_value.c_str());

			PyGILState_Release(gstate);
			return pvalue;
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
			m_value[0] = x;
			m_value[1] = y;
			m_value[2] = z;
			m_value[3] = w;
		}

		virtual void setPyValue(PyObject* value) override
		{

			PyGILState_STATE gstate = PyGILState_Ensure();

			if (m_valuecount == 1 && !PyLong_Check(value))
			{
				PyGILState_Release(gstate);
				mvAppLog::getLogger()->LogError(m_name + " type must be an integer.");
				return;
			}
			else if (m_valuecount != 1 && !PyList_Check(value))
			{
				PyGILState_Release(gstate);
				mvAppLog::getLogger()->LogError(m_name + " type must be an integer list.");
				return;
			}

			if (m_valuecount == 1)
				m_value[0] = PyLong_AsLong(value);

			else
			{
				for (int i = 0; i < PyList_Size(value); i++)
					m_value[i] = PyLong_AsLong(PyList_GetItem(value, i));
			}

			PyGILState_Release(gstate);
		}

		virtual PyObject* getPyValue() const override
		{
			PyGILState_STATE gstate = PyGILState_Ensure();

			if (m_valuecount == 1)
			{
				PyObject* pvalue = Py_BuildValue("i", m_value[0]);
				PyGILState_Release(gstate);
				return pvalue;
			}

			else
			{
				PyObject* value = PyList_New(m_valuecount);
				for (int i = 0; i < m_valuecount; i++)
					PyList_SetItem(value, i, PyLong_FromLong(m_value[i]));
				PyGILState_Release(gstate);
				return value;
			}

			PyGILState_Release(gstate);

		}

	protected:

		unsigned m_valuecount = 1;
		int      m_value[4];

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
			m_value[0] = x;
			m_value[1] = y;
			m_value[2] = z;
			m_value[3] = w;
		}

		virtual void setPyValue(PyObject* value) override
		{

			PyGILState_STATE gstate = PyGILState_Ensure();

			if (m_valuecount == 1 && !PyFloat_Check(value))
			{
				PyGILState_Release(gstate);
				mvAppLog::getLogger()->LogError(m_name + " type must be a float.");
				return;
			}
			else if (m_valuecount != 1 && !PyList_Check(value))
			{
				PyGILState_Release(gstate);
				mvAppLog::getLogger()->LogError(m_name + " type must be a float list.");
				return;
			}

			if(m_valuecount == 1)
				m_value[0] = PyFloat_AsDouble(value);

			else
			{
				for (int i = 0; i < PyList_Size(value); i++)
					m_value[i] = (float)PyFloat_AsDouble(PyList_GetItem(value, i));
			}

			PyGILState_Release(gstate);
		}

		virtual PyObject* getPyValue() const override
		{

			PyGILState_STATE gstate = PyGILState_Ensure();

			if (m_valuecount == 1)
			{
				PyObject* pvalue = Py_BuildValue("f", m_value[0]);
				PyGILState_Release(gstate);
				return pvalue;
			}

			else
			{
				PyObject* value = PyList_New(m_valuecount);
				for (int i = 0; i < m_valuecount; i++)
					PyList_SetItem(value, i, PyFloat_FromDouble(m_value[i]));

				PyGILState_Release(gstate);
				return value;
			}

		}

	protected:

		unsigned m_valuecount = 1;
		float    m_value[4];

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
			m_value[0] = color.r/255.0f;
			m_value[1] = color.g/255.0f;
			m_value[2] = color.b/255.0f;
			m_value[3] = color.a/255.0f;
		}

		virtual void setPyValue(PyObject* value) override
		{

			PyGILState_STATE gstate = PyGILState_Ensure();

			for (int i = 0; i < PyList_Size(value); i++)
				m_value[i] = PyLong_AsLong(PyList_GetItem(value, i))/255.0f;

			PyGILState_Release(gstate);

		}

		virtual PyObject* getPyValue() const override
		{

			PyGILState_STATE gstate = PyGILState_Ensure();

			PyObject* value = PyList_New(4);
			for (int i = 0; i < 4; i++)
				PyList_SetItem(value, i, PyLong_FromLong(m_value[i]*255));

			PyGILState_Release(gstate);
			return value;

		}

	protected:

		float m_value[4];

	};

}