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

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
			);

		parser.addArg<mvPyDataType::Bool>("small", mvArgType::KEYWORD_ARG, "False", "Small button, useful for embedding in text.");
		parser.addArg<mvPyDataType::Bool>("arrow", mvArgType::KEYWORD_ARG, "False", "Arrow button, must use with direction");
		parser.addArg<mvPyDataType::Integer>("direction", mvArgType::KEYWORD_ARG, "0", "A cardinal direction");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvButton::mvButton(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvButton::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_small_button)
		{
			if (ImGui::SmallButton(m_label.c_str()))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
			return;
		}

		if (m_arrow)
		{
			if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);

			return;
		}

		if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);

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