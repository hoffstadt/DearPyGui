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

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");

		parser.addArg<mvPyDataType::Bool>("small", mvArgType::KEYWORD, "False", "Small button, useful for embedding in text.");
		parser.addArg<mvPyDataType::Bool>("arrow", mvArgType::KEYWORD, "False", "Arrow button, must use with direction");
		parser.addArg<mvPyDataType::Integer>("direction", mvArgType::KEYWORD, "0", "A cardinal direction");

		parser.finalize();

		parsers->insert({ s_command, parser });
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

	void mvButton::handleSpecificKeywordArgs(PyObject* dict)
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

	void mvButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "small", ToPyBool(m_small_button));
		PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
		PyDict_SetItemString(dict, "direction", ToPyInt(MV_ENCODE_CONSTANT(m_direction, 0)));
	}

}