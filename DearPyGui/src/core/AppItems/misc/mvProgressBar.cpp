#include <utility>
#include "mvProgressBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvProgressBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value", "value from 0 to 1", "0.0"},
			{mvPythonDataType::String, "overlay", "overlayed text", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "data source for shared data", "''"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a progress bar.", "None", "Adding Widgets") });
	}

	mvProgressBar::mvProgressBar(const std::string& name)
		: mvFloatPtrBase(name)
	{
	}

	void mvProgressBar::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

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

}