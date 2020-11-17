#include "mvTypeBases.h"
#include <utility>
#include "PythonUtilities/mvPythonTranslator.h"
#include "mvApp.h"
#include "core/mvUtilities.h"
#include "mvAppLog.h"
#include "Registries/mvDataStorage.h"
#include "Registries/mvValueStorage.h"

namespace Marvel {

	mvIntPtrBase::mvIntPtrBase(const std::string& name, int default_value, const std::string& dataSource)
		: 
		mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddIntValue(name, default_value);
		else
			m_value = mvValueStorage::AddIntValue(dataSource, default_value);

		m_dataSource = dataSource;
	}

	mvIntPtrBase::~mvIntPtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvIntPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddIntValue(dataSource);
		m_dataSource = dataSource;
	}

	mvInt2PtrBase::mvInt2PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
		: 
		mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddInt2Value(name, { default_value[0], default_value[1] });
		else
			m_value = mvValueStorage::AddInt2Value(dataSource, { default_value[0], default_value[1] });

		m_dataSource = dataSource;
	}

	mvInt2PtrBase::~mvInt2PtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvInt2PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddInt2Value(dataSource);
		m_dataSource = dataSource;
	}

	mvInt3PtrBase::mvInt3PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
		: 
		mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddInt3Value(name, { default_value[0], default_value[1], default_value[2] });
		else
			m_value = mvValueStorage::AddInt3Value(dataSource, { default_value[0], default_value[1], default_value[2] });

		m_dataSource = dataSource;
	}

	mvInt3PtrBase::~mvInt3PtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvInt3PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddInt3Value(dataSource);
		m_dataSource = dataSource;
	}

	mvInt4PtrBase::mvInt4PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddInt4Value(name, { default_value[0], default_value[1], default_value[2], default_value[3] });
		else
			m_value = mvValueStorage::AddInt4Value(dataSource, { default_value[0], default_value[1], default_value[2], default_value[3] });

		m_dataSource = dataSource;
	}

	mvInt4PtrBase::~mvInt4PtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvInt4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddInt4Value(dataSource);
		m_dataSource = dataSource;
	}

	mvFloatPtrBase::mvFloatPtrBase(const std::string& name, float default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddFloatValue(name, default_value);
		else
			m_value = mvValueStorage::AddFloatValue(dataSource, default_value);

		m_dataSource = dataSource;
	}

	mvFloatPtrBase::~mvFloatPtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvFloatPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddFloatValue(dataSource);
		m_dataSource = dataSource;
	}

	mvFloat2PtrBase::mvFloat2PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddFloat2Value(name, { default_value[0], default_value[1] });
		else
			m_value = mvValueStorage::AddFloat2Value(dataSource, { default_value[0], default_value[1] });

		m_dataSource = dataSource;
	}

	mvFloat2PtrBase::~mvFloat2PtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvFloat2PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddFloat2Value(dataSource);
		m_dataSource = dataSource;
	}

	mvFloat3PtrBase::mvFloat3PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddFloat3Value(name, { default_value[0], default_value[1], default_value[2] });
		else
			m_value = mvValueStorage::AddFloat3Value(dataSource, { default_value[0], default_value[1], default_value[2] });

		m_dataSource = dataSource;
	}

	mvFloat3PtrBase::~mvFloat3PtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvFloat3PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddFloat3Value(dataSource);
		m_dataSource = dataSource;
	}

	mvFloat4PtrBase::mvFloat4PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddFloat4Value(name, { default_value[0], default_value[1], default_value[2], default_value[3] });
		else
			m_value = mvValueStorage::AddFloat4Value(dataSource, { default_value[0], default_value[1], default_value[2], default_value[3] });

		m_dataSource = dataSource;
	}

	mvFloat4PtrBase::~mvFloat4PtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvFloat4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddFloat4Value(dataSource);
		m_dataSource = dataSource;
	}

	mvColorPtrBase::mvColorPtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddColorValue(name, { default_value[0], default_value[1], default_value[2], default_value[3] });
		else
			m_value = mvValueStorage::AddColorValue(dataSource, { default_value[0], default_value[1], default_value[2], default_value[3] });

		m_dataSource = dataSource;
	}

	mvColorPtrBase::~mvColorPtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvColorPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddFloat4Value(dataSource);
		m_dataSource = dataSource;
	}

	mvBoolPtrBase::mvBoolPtrBase(const std::string& name, bool default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddBoolValue(name, default_value);
		else
			m_value = mvValueStorage::AddBoolValue(dataSource, default_value);

		m_dataSource = dataSource;

	}

	mvBoolPtrBase::~mvBoolPtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvBoolPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddBoolValue(dataSource);
		m_dataSource = dataSource;
	}

	mvStringPtrBase::mvStringPtrBase(const std::string& name, const std::string& default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddStringValue(name, default_value);
		else
			m_value = mvValueStorage::AddStringValue(dataSource, default_value);

		m_dataSource = dataSource;
	}

	mvStringPtrBase::~mvStringPtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvStringPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddStringValue(dataSource);
		m_dataSource = dataSource;
	}

	mvTimePtrBase::mvTimePtrBase(const std::string& name, const tm& default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = mvValueStorage::AddTimeValue(name, default_value);
			m_imvalue = mvValueStorage::GetImTimeValue(name);
		}
		else
		{
			m_value = mvValueStorage::AddTimeValue(dataSource, default_value);
			m_imvalue = mvValueStorage::GetImTimeValue(dataSource);
		}

		m_dataSource = dataSource;
	}

	mvTimePtrBase::~mvTimePtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	mvFloatVectPtrBase::mvFloatVectPtrBase(const std::string& name, const std::vector<float>& default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
			m_value = mvValueStorage::AddFloatVectorValue(name, default_value);
		else
			m_value = mvValueStorage::AddFloatVectorValue(dataSource, default_value);

		m_dataSource = dataSource;
	}

	mvFloatVectPtrBase::~mvFloatVectPtrBase()
	{
		if (m_dataSource.empty())
			mvValueStorage::DecrementRef(m_name);
		else
			mvValueStorage::DecrementRef(m_dataSource);
	}

	void mvFloatVectPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_dataSource) return;
		mvValueStorage::DecrementRef(m_dataSource);
		m_value = mvValueStorage::AddFloatVectorValue(dataSource);
		m_dataSource = dataSource;
	}

	mvBaseWindowAppitem::mvBaseWindowAppitem(const std::string& name)
		: 
		mvAppItem(name), mvEventHandler()
	{
		m_description.root = true;
	}

	void mvBaseWindowAppitem::addFlag(ImGuiWindowFlags flag) 
	{ 
		m_windowflags |= flag; 
	}

	void mvBaseWindowAppitem::removeFlag(ImGuiWindowFlags flag) 
	{ 
		m_windowflags &= ~flag; 
	}

	void mvBaseWindowAppitem::setWindowPos(float x, float y)
	{
		m_xpos = (int)x;
		m_ypos = (int)y;
		m_dirty_pos = true;
	}

	void mvBaseWindowAppitem::setWidth(int width)
	{ 
		m_width = width; 
		m_dirty_size = true; 
	}

	void mvBaseWindowAppitem::setHeight(int height)
	{
		m_height = height; 
		m_dirty_size = true; 
	}

	mvVec2 mvBaseWindowAppitem::getWindowPos() const
	{
		return { (float)m_xpos, (float)m_ypos };
	}

	bool mvBaseWindowAppitem::prerender()
	{

		if (m_dirty_size)
		{
			ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
			m_dirty_size = false;
		}

		if (m_dirty_pos)
		{
			ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
			m_dirty_pos = false;
		}

		if (!m_show)
			return false;

		if (!ImGui::Begin(m_label.c_str(), &m_show, m_windowflags))
		{
			ImGui::End();
			return false;
		}

		return true;
	}

	void mvBaseWindowAppitem::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) setWindowPos((float)ToInt(item), (float)m_ypos);
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) setWindowPos((float)m_xpos, (float)ToInt(item));

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		flagop("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		flagop("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
		flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		flagop("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);

	}

	void mvBaseWindowAppitem::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "x_pos", ToPyInt(m_xpos));
		PyDict_SetItemString(dict, "y_pos", ToPyInt(m_ypos));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		checkbitset("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		checkbitset("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		checkbitset("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		checkbitset("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
		checkbitset("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		checkbitset("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);
	}

}