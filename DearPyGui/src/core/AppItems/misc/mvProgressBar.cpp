#include <utility>
#include "mvProgressBar.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	void mvProgressBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_progress_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value", "value from 0 to 1", "0.0"},
			{mvPythonDataType::String, "overlay", "overlayed text", "''"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "data source for shared data", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a progress bar.", "None", "Adding Widgets") });
	}

	mvProgressBar::mvProgressBar(const std::string& name, float default_value, const std::string& dataSource)
		: mvFloatPtrBase(name, default_value, dataSource)
	{
	}

	void mvProgressBar::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		ImGui::ProgressBar(*m_value, ImVec2((float)m_width, (float)m_height), m_overlay.c_str());

	}

	void mvProgressBar::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
	}

	void mvProgressBar::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
	}

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* overlay = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int width = 0;
		int height = 0;
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&overlay, &tip, &parent, &before, &source, &width, &height, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvProgressBar>(name, default_value, source);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

}