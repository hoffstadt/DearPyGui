#pragma once

//-----------------------------------------------------------------------------
// mvTypeBases
//
//     - This file contains abstract classes for concrete mvAppItem 
//       implementations which share common underlying data types.
//     
//-----------------------------------------------------------------------------

#include <utility>
#include <implot.h>
#include <implot_internal.h>
#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvAppItem.h"
#include "Core/mvUtilities.h"
#include "mvAppLog.h"
#include "Core/mvDataStorage.h"

#include "Core/mvValueStorage.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvIntPtrBase
	//-----------------------------------------------------------------------------
	class mvIntPtrBase : public mvAppItem
	{

	public:

		mvIntPtrBase(const std::string& name, int default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue(default_value)
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddIntValue(name, default_value);
			else
				m_value = mvValueStorage::AddIntValue(dataSource, default_value);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = &m_backupvalue;

			m_dataSource = dataSource;
		}

		~mvIntPtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		int* m_value = nullptr;
		int m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvInt2PtrBase
	//-----------------------------------------------------------------------------
	class mvInt2PtrBase : public mvAppItem
	{

	public:

		mvInt2PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue({default_value[0], default_value[1]})
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddInt2Value(name, m_backupvalue);
			else
				m_value = mvValueStorage::AddInt2Value(dataSource, m_backupvalue);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = m_backupvalue.data();

			m_dataSource = dataSource;
		}

		~mvInt2PtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		int* m_value = nullptr;
		std::array<int, 2> m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvInt3PtrBase
	//-----------------------------------------------------------------------------
	class mvInt3PtrBase : public mvAppItem
	{

	public:

		mvInt3PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue({ default_value[0], default_value[1], default_value[2] })
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddInt3Value(name, m_backupvalue);
			else
				m_value = mvValueStorage::AddInt3Value(dataSource, m_backupvalue);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = m_backupvalue.data();

			m_dataSource = dataSource;
		}

		~mvInt3PtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		int* m_value = nullptr;
		std::array<int, 3> m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvInt4PtrBase
	//-----------------------------------------------------------------------------
	class mvInt4PtrBase : public mvAppItem
	{

	public:

		mvInt4PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue({ default_value[0], default_value[1], default_value[2] , default_value[3]})
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddInt4Value(name, m_backupvalue);
			else
				m_value = mvValueStorage::AddInt4Value(dataSource, m_backupvalue);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = m_backupvalue.data();

			m_dataSource = dataSource;
		}

		~mvInt4PtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		int* m_value = nullptr;
		std::array<int, 4> m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvFloatPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatPtrBase : public mvAppItem
	{

	public:

		mvFloatPtrBase(const std::string& name, float default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue(default_value)
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddFloatValue(name, default_value);
			else
				m_value = mvValueStorage::AddFloatValue(dataSource, default_value);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = &m_backupvalue;

			m_dataSource = dataSource;
		}

		~mvFloatPtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		float* m_value = nullptr;
		float m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvFloat2PtrBase
	//-----------------------------------------------------------------------------
	class mvFloat2PtrBase : public mvAppItem
	{

	public:

		mvFloat2PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue({ default_value[0], default_value[1] })
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddFloat2Value(name, m_backupvalue);
			else
				m_value = mvValueStorage::AddFloat2Value(dataSource, m_backupvalue);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = m_backupvalue.data();

			m_dataSource = dataSource;
		}

		~mvFloat2PtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		float* m_value = nullptr;
		std::array<float, 2> m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvFloat3PtrBase
	//-----------------------------------------------------------------------------
	class mvFloat3PtrBase : public mvAppItem
	{

	public:

		mvFloat3PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue({ default_value[0], default_value[1], default_value[2] })
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddFloat3Value(name, m_backupvalue);
			else
				m_value = mvValueStorage::AddFloat3Value(dataSource, m_backupvalue);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = m_backupvalue.data();

			m_dataSource = dataSource;
		}

		~mvFloat3PtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		float* m_value = nullptr;
		std::array<float, 3> m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvFloat4PtrBase
	//-----------------------------------------------------------------------------
	class mvFloat4PtrBase : public mvAppItem
	{

	public:

		mvFloat4PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue({ default_value[0], default_value[1], default_value[2], default_value[3] })
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddFloat4Value(name, m_backupvalue);
			else
				m_value = mvValueStorage::AddFloat4Value(dataSource, m_backupvalue);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = m_backupvalue.data();

			m_dataSource = dataSource;
		}

		~mvFloat4PtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		float* m_value = nullptr;
		std::array<float, 4> m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvBoolPtrBase
	//-----------------------------------------------------------------------------
	class mvBoolPtrBase : public mvAppItem
	{

	public:

		mvBoolPtrBase(const std::string& name, bool default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue(default_value)
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddBoolValue(name, default_value);
			else
				m_value = mvValueStorage::AddBoolValue(dataSource, default_value);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = &m_backupvalue;

			m_dataSource = dataSource;
		}

		~mvBoolPtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		bool* m_value = nullptr;
		bool m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvStringPtrBase
	//-----------------------------------------------------------------------------
	class mvStringPtrBase : public mvAppItem
	{

	public:

		mvStringPtrBase(const std::string& name, const std::string& default_value, const std::string& dataSource)
			: mvAppItem(name), m_backupvalue(default_value)
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddStringValue(name, default_value);
			else
				m_value = mvValueStorage::AddStringValue(dataSource, default_value);

			// TODO: throw warning
			if (m_value == nullptr)
				m_value = &m_backupvalue;

			m_dataSource = dataSource;
		}

		~mvStringPtrBase()
		{
			if (m_dataSource.empty())
				mvValueStorage::DecrementRef(m_name);
			else
				mvValueStorage::DecrementRef(m_dataSource);
		}

	protected:

		std::string* m_value = nullptr;
		std::string  m_backupvalue;
	};

	//-----------------------------------------------------------------------------
	// mvTimeItemBase
	//-----------------------------------------------------------------------------
	class mvTimeItemBase : public mvAppItem
	{

	public:


		mvTimeItemBase(const std::string& name, tm default_value)
			: mvAppItem(name), m_value(default_value)
		{
			m_imvalue = ImPlot::MkGmtTime(&m_value);
		}

		void setPyValue(PyObject* value) override
		{
			m_value = ToTime(value);
			m_imvalue = ImPlot::MakeTime(
				m_value.tm_year,
				m_value.tm_mon,
				m_value.tm_mday,
				m_value.tm_hour,
				m_value.tm_min,
				m_value.tm_sec);
		}

		[[nodiscard]] PyObject* getPyValue() const override
		{
			return ToPyTime(m_value);
		}

	protected:

		tm         m_value;
		ImPlotTime m_imvalue;
	
	};

}