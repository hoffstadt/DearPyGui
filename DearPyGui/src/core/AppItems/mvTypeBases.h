#pragma once

#include "mvAppItem.h"
#include <implot.h>
#include <implot_internal.h>
#include <array>

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

		mvIntPtrBase(const std::string& name, int default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject*      getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<int> m_value;
		int  m_disabled_value = 0;
	};

	//-----------------------------------------------------------------------------
	// mvInt4PtrBase
	//-----------------------------------------------------------------------------
	class mvInt4PtrBase : public mvAppItem
	{

	public:

		mvInt4PtrBase(const std::string& name, int* default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<int, 4>> m_value;
		int  m_disabled_value[4] {};
	};

	//-----------------------------------------------------------------------------
	// mvFloatPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatPtrBase : public mvAppItem
	{

	public:

		mvFloatPtrBase(const std::string& name, float default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<float> m_value;
		float  m_disabled_value = 0.0;
	};

	//-----------------------------------------------------------------------------
	// mvFloat4PtrBase
	//-----------------------------------------------------------------------------
	class mvFloat4PtrBase : public mvAppItem
	{

	public:

		mvFloat4PtrBase(const std::string& name, float* default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<float, 4>> m_value;
		float  m_disabled_value[4] {};
	};

	//-----------------------------------------------------------------------------
	// mvColorPtrBase
	//-----------------------------------------------------------------------------
	class mvColorPtrBase : public mvAppItem
	{

	public:

		mvColorPtrBase(const std::string& name, const float* default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::array<float, 4>> m_value;
		float  m_disabled_value[4]{};
	};

	//-----------------------------------------------------------------------------
	// mvBoolPtrBase
	//-----------------------------------------------------------------------------
	class mvBoolPtrBase : public mvAppItem
	{

	public:

		mvBoolPtrBase(const std::string& name, bool default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<bool> m_value;
		bool  m_disabled_value = false;
	};

	//-----------------------------------------------------------------------------
	// mvStringPtrBase
	//-----------------------------------------------------------------------------
	class mvStringPtrBase : public mvAppItem
	{

	public:

		mvStringPtrBase(const std::string& name, const std::string& default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::string> m_value;
		std::string  m_disabled_value = "";
	};

	//-----------------------------------------------------------------------------
	// mvTimePtrBase
	//-----------------------------------------------------------------------------
	class mvTimePtrBase : public mvAppItem
	{

	public:

		mvTimePtrBase(const std::string& name, const tm& default_value);
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<tm>         m_value ;
		mvRef<ImPlotTime> m_imvalue;
	};

	//-----------------------------------------------------------------------------
	// mvFloatVectPtrBase
	//-----------------------------------------------------------------------------
	class mvFloatVectPtrBase : public mvAppItem
	{

	public:

		mvFloatVectPtrBase(const std::string& name, const std::vector<float>& default_value);
		void setDataSource(const std::string& dataSource) override;
		mvValueVariant getValue() override { return m_value; }
		PyObject* getPyValue() override;
		void           setPyValue(PyObject* value) override;

	protected:

		mvRef<std::vector<float>> m_value;
	};

	//-----------------------------------------------------------------------------
	// mvBaseWindowAppitem
	//-----------------------------------------------------------------------------
	class mvBaseWindowAppitem : public mvAppItem
	{

	public:


		mvBaseWindowAppitem(const std::string& name);

		void   addFlag     (ImGuiWindowFlags flag);
		void   removeFlag  (ImGuiWindowFlags flag);
		void   setWindowPos(float x, float y);
		bool   prerender   ();
		mvVec2 getWindowPos() const;

		void setWidth          (int width)      override;
		void setHeight         (int height)     override;
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