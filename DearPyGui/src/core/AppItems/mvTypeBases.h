#pragma once
#include <variant>
#include <implot.h>
#include <implot_internal.h>
#include "mvAppItem.h"
#include <any>
#include <core/Registries/mvValueStorage.h>
#include <core/mvApp.h>
#include "mvEventHandler.h"

//-----------------------------------------------------------------------------
// mvTypeBases
//
//     - This file contains abstract classes for concrete mvAppItem
//       implementations which share common underlying data types.
//
//-----------------------------------------------------------------------------

namespace Marvel
{

	//-----------------------------------------------------------------------------
	// mvPtrBase
	//-----------------------------------------------------------------------------
	template<typename P, size_t S>
	class mvPtrBase : public mvAppItem
	{
	};

	//-----------------------------------------------------------------------------
	// mvPtrBase Single Class
	//-----------------------------------------------------------------------------
	template<typename P>
	class mvPtrBase<P, 1> : public mvAppItem
	{
	public:
		mvPtrBase(const std::string& name, P default_value, const std::string& dataSource) : mvAppItem(name)
		{
			if (dataSource.empty())
				m_value = mvValueStorage::AddValue<P>(name, default_value);
			else
				m_value = mvValueStorage::AddValue<P>(dataSource, default_value);

			m_dataSource = dataSource;
		}

		P* GetValue()
		{
			return mvValueStorage::GetValue<P>(m_name);
		}
		bool SetValue(const P& value)
		{
			return mvValueStorage::UpdateValue<P>(m_name, value);
		}

		~mvPtrBase() override
		{
			mvValueStorage::DecrementRef(m_name);
			if (!m_dataSource.empty())
			{
				mvValueStorage::DecrementRef(m_dataSource);
			}
		}

		void setDataSource(const std::string& dataSource) override
		{
			if (dataSource == m_dataSource || mvValueStorage::GetType(dataSource) != mvValueStorage::GetType(m_name))
			{
				return;
			}
			if (dataSource == m_name)
			{
				m_value = mvValueStorage::GetValue<P>(m_name);
			}
			else
			{
				mvValueStorage::DecrementRef(m_dataSource);
				m_value = mvValueStorage::AddValue<P>(dataSource);
				m_dataSource = dataSource;
			}
		};

	protected:
		P* m_value;
		P m_disabled_value = *mvValueStorage::GetDefaultValue<P>();
	};

	//-----------------------------------------------------------------------------
	// mvPtrBase Primitive Array
	//-----------------------------------------------------------------------------
	template<typename P, size_t S>
	class mvPtrBase<P*, S> : public mvAppItem
	{
	public:
		mvPtrBase(const std::string& name, P* default_value, const std::string& dataSource) :
			mvAppItem(name)
		{
			std::array<P, S> vals;
			std::copy_n(default_value, S, vals.begin());
			if (dataSource.empty())
			{
				m_value = mvValueStorage::AddValue<std::array<P, S>>(name, vals)->data();
			}
			else
			{
				m_value = mvValueStorage::AddValue<std::array<P, S>>(dataSource, vals)->data();
			}

			m_dataSource = dataSource;
		};

		~mvPtrBase() override
		{
			mvValueStorage::DecrementRef(m_name);
			if (!m_dataSource.empty())
			{
				mvValueStorage::DecrementRef(m_dataSource);
			}
		};

		P* GetValue()
		{
			return mvValueStorage::GetValue<P>(m_name);
		}
		bool SetValue(const P& value)
		{
			return mvValueStorage::UpdateValue<P>(m_name, value);
		}

		void setDataSource(const std::string& dataSource) override
		{
			if (dataSource == m_dataSource || mvValueStorage::GetType(dataSource) != mvValueStorage::GetType(m_name))
			{
				return;
			}
			if (dataSource == m_name)
			{
				m_value = mvValueStorage::GetValue<P>(m_name);
			}
			else
			{
				mvValueStorage::DecrementRef(m_dataSource);
				m_value = mvValueStorage::AddValue<P>(dataSource);
				m_dataSource = dataSource;
			}
		};

	protected:
		P* m_value;
		P* m_disabled_value = mvValueStorage::GetDefaultValue<P>();
	};

	//-----------------------------------------------------------------------------
	// mvBaseWindowAppitem
	//-----------------------------------------------------------------------------
	class mvBaseWindowAppitem : public mvAppItem, public mvEventHandler
	{

	public:

		mvBaseWindowAppitem(const std::string& name);

		void addFlag(ImGuiWindowFlags flag);
		void removeFlag(ImGuiWindowFlags flag);
		void setWindowPos(float x, float y);
		bool prerender();
		mvVec2 getWindowPos() const;

		void setWidth(int width) override;
		void setHeight(int height) override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	protected:

		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		int              m_xpos = 200;
		int              m_ypos = 200;
		bool             m_dirty_pos = true;
		bool             m_dirty_size = true;
	};

}