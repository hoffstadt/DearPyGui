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

		mvIntPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject*      getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<int> _value = CreateRef<int>(0);
		int  _disabled_value = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInt4PtrBase
	//-----------------------------------------------------------------------------
	class mvInt4PtrBase : public mvAppItem
	{

	public:

		mvInt4PtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
		int  _disabled_value[4] {};
	};

	//-----------------------------------------------------------------------------
	// mvFloatPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatPtrBase : public mvAppItem
	{

	public:

		mvFloatPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<float> _value = CreateRef<float>(0.0f);
		float  _disabled_value = 0.0f;
	};

	//-----------------------------------------------------------------------------
	// mvDoublePtrBase
	//-----------------------------------------------------------------------------
	class mvDoublePtrBase : public mvAppItem
	{

	public:

		mvDoublePtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<double> _value = CreateRef<double>(0.0);
		float  _disabled_value = 0.0;
	};

	//-----------------------------------------------------------------------------
	// mvFloat4PtrBase
	//-----------------------------------------------------------------------------
	class mvFloat4PtrBase : public mvAppItem
	{

	public:

		mvFloat4PtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
		float  _disabled_value[4] {};
	};

	//-----------------------------------------------------------------------------
	// mvDouble4PtrBase
	//-----------------------------------------------------------------------------
	class mvDouble4PtrBase : public mvAppItem
	{

	public:

		mvDouble4PtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<double, 4>> _value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
		double  _disabled_value[4]{};
	};

	//-----------------------------------------------------------------------------
	// mvColorPtrBase
	//-----------------------------------------------------------------------------
	class mvColorPtrBase : public mvAppItem
	{

	public:

		mvColorPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
		float  _disabled_value[4]{};
	};

	//-----------------------------------------------------------------------------
	// mvBoolPtrBase
	//-----------------------------------------------------------------------------
	class mvBoolPtrBase : public mvAppItem
	{

	public:

		mvBoolPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<bool> _value = CreateRef<bool>(false);
		bool  _disabled_value = false;
	};

	//-----------------------------------------------------------------------------
	// mvStringPtrBase
	//-----------------------------------------------------------------------------
	class mvStringPtrBase : public mvAppItem
	{

	public:

		mvStringPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void      setPyValue(PyObject* value) override;

	protected:

		mvRef<std::string> _value = CreateRef<std::string>("");
		std::string  _disabled_value = "";
	};

	//-----------------------------------------------------------------------------
	// mvUUIDPtrBase
	//-----------------------------------------------------------------------------
	class mvUUIDPtrBase : public mvAppItem
	{

	public:

		mvUUIDPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void      setPyValue(PyObject* value) override;

	protected:

		mvRef<mvUUID> _value = CreateRef<mvUUID>(0);
		mvUUID  _disabled_value = 0;
	};

	//-----------------------------------------------------------------------------
	// mvTimePtrBase
	//-----------------------------------------------------------------------------
	class mvTimePtrBase : public mvAppItem
	{

	public:

		mvTimePtrBase(mvUUID uuid);
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<tm>         _value = CreateRef<tm>();
		mvRef<ImPlotTime> _imvalue = CreateRef<ImPlotTime>();
	};

	//-----------------------------------------------------------------------------
	// mvFloatVectPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatVectPtrBase : public mvAppItem
	{

	public:

		mvFloatVectPtrBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
	};

	//-----------------------------------------------------------------------------
	// mvSeriesBase
	//-----------------------------------------------------------------------------
	class mvSeriesBase : public mvAppItem
	{

	public:

		mvSeriesBase(mvUUID uuid);
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;


	protected:

		mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
			std::vector<std::vector<double>>{ std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{} });
	};

}