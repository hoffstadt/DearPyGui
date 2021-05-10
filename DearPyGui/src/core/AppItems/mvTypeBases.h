#pragma once

#include <implot.h>
#include <implot_internal.h>
#include <array>
#include "mvItemRegistry.h"
#include "cpp.hint"

//-----------------------------------------------------------------------------
// mvTypeBases
//
//     - This file contains abstract classes for concrete mvAppItem 
//       implementations which share common underlying data types.
//     
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvIntPtrBase
	//-----------------------------------------------------------------------------
	class mvIntPtrBase : public mvAppItem
	{

	public:

		mvIntPtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject*      getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<int> m_value = CreateRef<int>(0);
		int  m_disabled_value = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInt4PtrBase
	//-----------------------------------------------------------------------------
	class mvInt4PtrBase : public mvAppItem
	{

	public:

		mvInt4PtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<int, 4>> m_value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
		int  m_disabled_value[4] {};
	};

	//-----------------------------------------------------------------------------
	// mvFloatPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatPtrBase : public mvAppItem
	{

	public:

		mvFloatPtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<float> m_value = CreateRef<float>(0.0f);
		float  m_disabled_value = 0.0f;
	};

	//-----------------------------------------------------------------------------
	// mvDoublePtrBase
	//-----------------------------------------------------------------------------
	class mvDoublePtrBase : public mvAppItem
	{

	public:

		mvDoublePtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<double> m_value = CreateRef<double>(0.0);
		float  m_disabled_value = 0.0;
	};

	//-----------------------------------------------------------------------------
	// mvFloat4PtrBase
	//-----------------------------------------------------------------------------
	class mvFloat4PtrBase : public mvAppItem
	{

	public:

		mvFloat4PtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<float, 4>> m_value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
		float  m_disabled_value[4] {};
	};

	//-----------------------------------------------------------------------------
	// mvDouble4PtrBase
	//-----------------------------------------------------------------------------
	class mvDouble4PtrBase : public mvAppItem
	{

	public:

		mvDouble4PtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<double, 4>> m_value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
		double  m_disabled_value[4]{};
	};

	//-----------------------------------------------------------------------------
	// mvColorPtrBase
	//-----------------------------------------------------------------------------
	class mvColorPtrBase : public mvAppItem
	{

	public:

		mvColorPtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<float, 4>> m_value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, -1.0f});
		float  m_disabled_value[4]{};
	};

	//-----------------------------------------------------------------------------
	// mvBoolPtrBase
	//-----------------------------------------------------------------------------
	class mvBoolPtrBase : public mvAppItem
	{

	public:

		mvBoolPtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<bool> m_value = CreateRef<bool>(false);
		bool  m_disabled_value = false;
	};

	//-----------------------------------------------------------------------------
	// mvStringPtrBase
	//-----------------------------------------------------------------------------
	class mvStringPtrBase : public mvAppItem
	{

	public:

		mvStringPtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::string> m_value = CreateRef<std::string>("");
		std::string  m_disabled_value = "";
	};

	//-----------------------------------------------------------------------------
	// mvTimePtrBase
	//-----------------------------------------------------------------------------
	class mvTimePtrBase : public mvAppItem
	{

	public:

		mvTimePtrBase(const std::string& name);
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<tm>         m_value = CreateRef<tm>();
		mvRef<ImPlotTime> m_imvalue = CreateRef<ImPlotTime>();
	};

	//-----------------------------------------------------------------------------
	// mvFloatVectPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatVectPtrBase : public mvAppItem
	{

	public:

		mvFloatVectPtrBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::vector<float>> m_value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
	};

	//-----------------------------------------------------------------------------
	// mvSeriesBase
	//-----------------------------------------------------------------------------
	class mvSeriesBase : public mvAppItem
	{

	public:

		mvSeriesBase(const std::string& name);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		bool isParentCompatible(mvAppItemType type) override;

		const std::pair<double, double>& getMaxMin(int i) const;
		bool doesSeriesContributeToBounds() const { return m_contributeToBounds; }
		ImPlotYAxis getAxis() const { return m_axis; }

	protected:

		void calculateMaxMins();
		void resetMaxMins();

	protected:

		mvRef<std::vector<std::vector<double>>> m_value = CreateRef<std::vector<std::vector<double>>>(
			std::vector<std::vector<double>>{ std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{} });
		std::vector<std::pair<double, double>>   m_maxMins;
		bool                                   m_contributeToBounds = false;
		ImPlotYAxis                           m_axis = ImPlotYAxis_1;
	};

}