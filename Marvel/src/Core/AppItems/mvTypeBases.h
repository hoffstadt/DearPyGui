#pragma once

#include "Core/AppItems/mvAppItem.h"
#include "Core/Concurrency/mvThreadPool.h"
#include "Core/mvUtilities.h"

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
			std::lock_guard<std::mutex> lock(m_wmutex);
			m_value = PyLong_AsLong(value); 
		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);
			PyObject* pvalue = Py_BuildValue("i", m_value);
			return pvalue;
		}

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

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
			std::lock_guard<std::mutex> lock(m_wmutex);
			m_value = PyUnicode_AsUTF8(value);
		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);
			PyObject* pvalue = Py_BuildValue("s", m_value.c_str());
			return pvalue;
		}

		inline void setValue(const std::string& value) { m_value = value; }
		inline const std::string& getValue() const { return m_value; }

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
			std::lock_guard<std::mutex> lock(m_wmutex);

			if (m_valuecount == 1)
				m_value[0] = PyLong_AsLong(value);

			else
			{
				for (int i = 0; i < PyTuple_Size(value); i++)
					m_value[i] = PyLong_AsLong(PyTuple_GetItem(value, i));
			}
		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);

			if (m_valuecount == 1)
			{
				PyObject* pvalue = Py_BuildValue("i", m_value[0]);
				return pvalue;
			}

			else
			{
				PyObject* value = PyTuple_New(m_valuecount);
				for (int i = 0; i < m_valuecount; i++)
					PyTuple_SetItem(value, i, PyLong_FromLong(m_value[i]));
				return value;
			}

		}

		inline void setValue(int value, int y=0, int z=0, int w=0)
		{
			m_value[0] = value;
			m_value[1] = y;
			m_value[2] = z;
			m_value[3] = w;
		}

		inline const int* getValue() const { return &m_value[0]; }

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
			std::lock_guard<std::mutex> lock(m_wmutex);

			if(m_valuecount == 1)
				m_value[0] = PyFloat_AsDouble(value);

			else
			{
				for (int i = 0; i < PyTuple_Size(value); i++)
					m_value[i] = (float)PyFloat_AsDouble(PyTuple_GetItem(value, i));
			}
		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);

			if (m_valuecount == 1)
			{
				PyObject* pvalue = Py_BuildValue("f", m_value[0]);
				return pvalue;
			}

			else
			{
				PyObject* value = PyTuple_New(m_valuecount);
				for (int i = 0; i < m_valuecount; i++)
					PyTuple_SetItem(value, i, PyFloat_FromDouble(m_value[i]));
				return value;
			}

		}

		inline void setValue(float value, float y=0.0f, float z=0.0f, float w=0.0f) 
		{
			m_value[0] = value; 
			m_value[1] = y; 
			m_value[2] = z; 
			m_value[3] = w; 
		}

		inline const float* getValue() const { return &m_value[0]; }

	protected:

		unsigned m_valuecount = 1;
		float    m_value[4];

	};

	//-----------------------------------------------------------------------------
	// mvImageItemBase
	//-----------------------------------------------------------------------------
	class mvImageItemBase : public mvAppItem
	{

	public:

		mvImageItemBase(const std::string& parent, const std::string& name, const std::string& value)
			: mvAppItem(parent, name), m_value(value)
		{
		}

		virtual void setPyValue(PyObject* value) override
		{
			std::lock_guard<std::mutex> lock(m_wmutex);
			m_value = PyUnicode_AsUTF8(value);
			if (!m_value.empty())
				LoadTextureFromFile(m_value.c_str(), &m_texture, &m_width, &m_height);

		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);
			PyObject* pvalue = Py_BuildValue("s", m_value.c_str());
			return pvalue;
		}

		inline void setValue(const std::string& value) { m_value = value; }
		inline const std::string& getValue() const { return m_value; }

	protected:

		std::string m_value;
		void*       m_texture = nullptr;

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
			std::lock_guard<std::mutex> lock(m_wmutex);

			for (int i = 0; i < PyTuple_Size(value); i++)
				m_value[i] = PyLong_AsLong(PyTuple_GetItem(value, i))/255.0f;

		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);

			PyObject* value = PyTuple_New(4);
			for (int i = 0; i < 4; i++)
				PyTuple_SetItem(value, i, PyLong_FromLong(m_value[i]*255));
			return value;

		}

	protected:

		float m_value[4];

	};

}