#include "mvDatePicker.h"
#include "mvApp.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvDatePicker::mvDatePicker(const std::string& name, tm default_value)
		: mvTimePtrBase(name, default_value, name)
	{
	}

	void mvDatePicker::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (ImPlot::ShowDatePicker(m_name.c_str(), &m_level, m_imvalue, m_imvalue))
		{
			ImPlot::GetGmtTime(*m_imvalue, m_value);
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);
		}

	}

	void mvDatePicker::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "level")) m_level = ToInt(item);
	}

	void mvDatePicker::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "level", ToPyInt(m_level));
	}

}