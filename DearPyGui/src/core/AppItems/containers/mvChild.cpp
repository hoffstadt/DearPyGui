#include "mvChild.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvChild::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID,
			"Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end.", 
			{ "Containers", "Widgets" },
			true);

		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("border", mvArgType::KEYWORD_ARG, "True", "Shows/Hides the border around the sides.");
		parser.addArg<mvPyDataType::Bool>("autosize_x", mvArgType::KEYWORD_ARG, "False", "Autosize the window to fit it's items in the x.");
		parser.addArg<mvPyDataType::Bool>("autosize_y", mvArgType::KEYWORD_ARG, "False", "Autosize the window to fit it's items in the y.");
		parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars (window can still scroll with mouse or programmatically).");
		parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear (off by default).");
		parser.addArg<mvPyDataType::Bool>("menubar", mvArgType::KEYWORD_ARG, "False", "Shows/Hides the menubar at the top.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvChild::mvChild(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvChild::addFlag(ImGuiWindowFlags flag)
	{
		_windowflags |= flag;
	}

	void mvChild::removeFlag(ImGuiWindowFlags flag)
	{
		_windowflags &= ~flag;
	}

	void mvChild::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		ImGui::BeginChild(_internalLabel.c_str(), ImVec2(_autosize_x ? 0 : (float)_width, _autosize_y ? 0 : (float)_height), _border, _windowflags);

		for (auto& item : _children[1])
		{

			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			if (item->isTracked())
			{
				ImGui::SetScrollHereX(item->getTrackOffset());
				ImGui::SetScrollHereY(item->getTrackOffset());
			}

			item->postDraw();
		}

		if (_scrollXSet)
		{
			if (_scrollX < 0.0f)
				ImGui::SetScrollHereX(1.0f);
			else
				ImGui::SetScrollX(_scrollX);
			_scrollXSet = false;
		}

		if (_scrollYSet)
		{
			if (_scrollY < 0.0f)
				ImGui::SetScrollHereY(1.0f);
			else
				ImGui::SetScrollY(_scrollY);
			_scrollYSet = false;
		}

		// allows this item to have a render callback
		registerWindowFocusing();

		_scrollX = ImGui::GetScrollX();
		_scrollY = ImGui::GetScrollY();
		_scrollMaxX = ImGui::GetScrollMaxX();
		_scrollMaxY = ImGui::GetScrollMaxY();

		ImGui::EndChild();
	}

	void mvChild::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "border")) _border = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_x")) _autosize_x = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_y")) _autosize_y = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, _windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, _windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
	}

	void mvChild::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "border", ToPyBool(_border));
		PyDict_SetItemString(dict, "autosize_x", ToPyBool(_autosize_x));
		PyDict_SetItemString(dict, "autosize_y", ToPyBool(_autosize_y));

		// helper for bit flipping
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, _windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, _windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
	}

}