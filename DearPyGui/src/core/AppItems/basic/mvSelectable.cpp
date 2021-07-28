#include <utility>
#include "mvSelectable.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
namespace Marvel {

	void mvSelectable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Adds a selectable.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("default_value", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("span_columns", mvArgType::KEYWORD_ARG, "False", "Span the width of all columns if placed in a table.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvSelectable::mvSelectable(mvUUID uuid)
		: mvBoolPtrBase(uuid)
	{
	}

	void mvSelectable::setEnabled(bool value)
	{
		if (value == _enabled)
			return;

		if (value)
			_flags &= ~ImGuiSelectableFlags_Disabled;

		else
			_flags |= ImGuiSelectableFlags_Disabled;

		_enabled = value;
	}

	void mvSelectable::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id(_uuid);

		if (ImGui::Selectable(_label.c_str(), _value.get(), _flags, ImVec2((float)_width, (float)_height)))
		{
			auto value = *_value;
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyBool(value), _user_data);
				});
		}

	}

	void mvSelectable::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags, bool flip)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("span_columns", ImGuiSelectableFlags_SpanAllColumns, _flags, false);

	}

	void mvSelectable::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags, bool flip)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("span_columns", ImGuiSelectableFlags_SpanAllColumns, _flags, false);
	}

}