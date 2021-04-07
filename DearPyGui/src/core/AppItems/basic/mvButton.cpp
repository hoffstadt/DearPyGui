#include <utility>
#include "mvButton.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text.", "False"},
			{mvPythonDataType::Bool, "arrow", "Arrow button, must use with direction", "False"},
			{mvPythonDataType::Integer, "direction", "A cardinal direction", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a button.", "None", "Adding Widgets") });
	}

	mvButton::mvButton(const std::string& name)
		: 
		mvAppItem(name)
	{
	}

	void mvButton::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_small_button)
		{
			if (ImGui::SmallButton(m_label.c_str()))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
			return;
		}

		if (m_arrow)
		{
			if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

			return;
		}

		if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

	}

	void mvButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "small")) m_small_button = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "arrow")) m_arrow = ToBool(item);

		if (PyObject* item = PyDict_GetItemString(dict, "direction"))
		{
			m_direction = ToInt(item);
			DecodelibID(m_direction, &m_direction);
		}

	}

	void mvButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "small", ToPyBool(m_small_button));
		PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
		PyDict_SetItemString(dict, "direction", ToPyInt(MV_ENCODE_CONSTANT(m_direction, 0)));
	}

}