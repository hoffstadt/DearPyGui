#include "mvSameLine.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvSameLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'sameline'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "xoffset", "offset from containing window", "0.0"},
			{mvPythonDataType::Float, "spacing", "offset from previous widget", "-1.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.",
		"None", "Adding Widgets") });
	}


	mvSameLine::mvSameLine(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvSameLine::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::SameLine(m_xoffset, m_spacing);
	}

	void mvSameLine::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) m_xoffset = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "spacing")) m_spacing = ToFloat(item);

	}

	void mvSameLine::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "xoffset", ToPyFloat(m_xoffset));
		PyDict_SetItemString(dict, "spacing", ToPyFloat(m_spacing));
	}

}