#include "mvDatePicker.h"
#include "mvApp.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	void mvDatePicker::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_date_picker", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Dict, "default_value", "data dict", "{'month_day': 14, 'year':20, 'month':5}"},
			{mvPythonDataType::Integer, "level", "0-day, 1-month, 2-year", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a data selector widget.", "None", "Adding Widgets") });
	}

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


	PyObject* add_date_picker(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = nullptr;
		int level = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_date_picker"].parse(args, kwargs, __FUNCTION__,
			&name, &default_value, &level, &callback, &callback_data, &tip, &parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvDatePicker>(name, ToTime(default_value));
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

}