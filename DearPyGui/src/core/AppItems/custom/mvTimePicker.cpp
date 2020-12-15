#include "mvTimePicker.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvTimePicker::mvTimePicker(const std::string& name, tm default_value)
		: 
		mvTimePtrBase(name, default_value, name)
	{
	}

	void mvTimePicker::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		ImPlot::GetStyle().Use24HourClock = m_hour24;

		if (ImPlot::ShowTimePicker(m_name.c_str(), m_imvalue))
		{
			ImPlot::GetGmtTime(*m_imvalue, m_value);
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);
		}

	}

	void mvTimePicker::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "hour24")) m_hour24 = ToBool(item);
	}

	void mvTimePicker::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "hour24", ToPyBool(m_hour24));
	}

}