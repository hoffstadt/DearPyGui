#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvUtilities.h"
#include "mvAppLog.h"
#include "mvValueStorage.h"

namespace Marvel {

	mvIntPtrBase::mvIntPtrBase(const std::string& name, int default_value, const std::string& dataSource)
		: 
		mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<int>(default_value);
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Int, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, default_value);

		m_core_config.source = dataSource;
	}
	
	mvIntPtrBase::~mvIntPtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvIntPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, 0);
		m_core_config.source = dataSource;
	}

	mvInt2PtrBase::mvInt2PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
		: 
		mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<int,2>>(std::array{ default_value[0], default_value[1] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Int2, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ default_value[0], default_value[1] });

		m_core_config.source = dataSource;
	}

	mvInt2PtrBase::~mvInt2PtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvInt2PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0, 0 });
		m_core_config.source = dataSource;
	}

	mvInt3PtrBase::mvInt3PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
		: 
		mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<int, 3>>(std::array{ default_value[0], default_value[1], default_value[2] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Int3, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ default_value[0], default_value[1], default_value[2] });

		m_core_config.source = dataSource;
	}

	mvInt3PtrBase::~mvInt3PtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvInt3PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0, 0, 0 });
		m_core_config.source = dataSource;
	}

	mvInt4PtrBase::mvInt4PtrBase(const std::string& name, int* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<int, 4>>(std::array{ default_value[0], default_value[1], default_value[2], default_value[3] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Int4, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ default_value[0], default_value[1], default_value[2], default_value[3] });

		m_core_config.source = dataSource;
	}

	mvInt4PtrBase::~mvInt4PtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvInt4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0,0,0,0 });
		m_core_config.source = dataSource;
	}

	mvFloatPtrBase::mvFloatPtrBase(const std::string& name, float default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<float>(default_value);
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Float, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, default_value);

		m_core_config.source = dataSource;
	}

	mvFloatPtrBase::~mvFloatPtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvFloatPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, 0.0f);
		m_core_config.source = dataSource;
	}

	mvFloat2PtrBase::mvFloat2PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<float, 2>>(std::array{ default_value[0], default_value[1] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Float2, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ default_value[0], default_value[1] });

		m_core_config.source = dataSource;
	}

	mvFloat2PtrBase::~mvFloat2PtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvFloat2PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0.0f, 0.0f });
		m_core_config.source = dataSource;
	}

	mvFloat3PtrBase::mvFloat3PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<float, 3>>(std::array{ default_value[0], default_value[1], default_value[2] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Float3, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ default_value[0], default_value[1], default_value[2] });

		m_core_config.source = dataSource;
	}

	mvFloat3PtrBase::~mvFloat3PtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvFloat3PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0.0f, 0.0f, 0.0f });
		m_core_config.source = dataSource;
	}

	mvFloat4PtrBase::mvFloat4PtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<float, 4>>(std::array{ default_value[0], default_value[1], default_value[2], default_value[3] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Float4, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource,
				std::array{ default_value[0], default_value[1], default_value[2], default_value[3] });

		m_core_config.source = dataSource;
	}

	mvFloat4PtrBase::~mvFloat4PtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvFloat4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0.0f, 0.0f, 0.0f, 0.0f });
		m_core_config.source = dataSource;
	}

	mvColorPtrBase::mvColorPtrBase(const std::string& name, float* default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::array<float, 4>>(std::array{ default_value[0], default_value[1], default_value[2], default_value[3] });
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Float4, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, 
				std::array{ default_value[0], default_value[1], default_value[2], default_value[3] });

		m_core_config.source = dataSource;
	}

	mvColorPtrBase::~mvColorPtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvColorPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::array{ 0.0f, 0.0f, 0.0f, 0.0f });
		m_core_config.source = dataSource;
	}

	mvBoolPtrBase::mvBoolPtrBase(const std::string& name, bool default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		m_description.valueType = StorageValueTypes::Bool;

		if (dataSource.empty())
		{
			m_value = std::make_shared<bool>(default_value);
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Bool, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, default_value);

		m_core_config.source = dataSource;

	}

	mvBoolPtrBase::~mvBoolPtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvBoolPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, false);
		m_core_config.source = dataSource;
	}

	mvStringPtrBase::mvStringPtrBase(const std::string& name, const std::string& default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::string>(default_value);
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::String, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, default_value);

		m_core_config.source = dataSource;
	}

	void mvStringPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::string(""));
		m_core_config.source = dataSource;
	}

	mvStringPtrBase::~mvStringPtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	mvTimePtrBase::mvTimePtrBase(const std::string& name, const tm& default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<tm>(default_value);
			m_imvalue = std::make_shared<ImPlotTime>(ImPlot::MkGmtTime(m_value.get()));
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::Time, this);
		}
		else
		{
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, default_value);
			m_imvalue = std::make_shared<ImPlotTime>(ImPlot::MkGmtTime(m_value.get()));
		}

		m_core_config.source = dataSource;
	}

	mvTimePtrBase::~mvTimePtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	mvFloatVectPtrBase::mvFloatVectPtrBase(const std::string& name, const std::vector<float>& default_value, const std::string& dataSource)
		: mvAppItem(name)
	{
		if (dataSource.empty())
		{
			m_value = std::make_shared<std::vector<float>>(default_value);
			mvApp::GetApp()->getValueStorage().RegisterExternalValue(name, StorageValueTypes::FloatVect, this);
		}
		else
			m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, default_value);

		m_core_config.source = dataSource;
	}

	mvFloatVectPtrBase::~mvFloatVectPtrBase()
	{
		if (m_core_config.source.empty())
			mvApp::GetApp()->getValueStorage().DeleteValue(m_core_config.name);
	}

	void mvFloatVectPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_core_config.source) return;
		m_value = mvApp::GetApp()->getValueStorage().add_value(dataSource, std::vector<float>{});
		m_core_config.source = dataSource;
	}

	mvBaseWindowAppitem::mvBaseWindowAppitem(const std::string& name)
		: 
		mvAppItem(name)
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
		m_core_config.width = width;
		m_dirty_size = true; 
	}

	void mvBaseWindowAppitem::setHeight(int height)
	{
		m_core_config.height = height;
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
			ImGui::SetNextWindowSize(ImVec2((float)m_core_config.width, (float)m_core_config.height));
			m_dirty_size = false;
		}

		if (m_dirty_pos)
		{
			ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
			m_dirty_pos = false;
		}

		if (!m_core_config.show)
			return false;

		if (!ImGui::Begin(m_label.c_str(), &m_core_config.show, m_windowflags))
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