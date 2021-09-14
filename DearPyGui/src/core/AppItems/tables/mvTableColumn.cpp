#include "mvTableColumn.h"
#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvTableColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::Float, "init_width_or_weight", mvArgType::KEYWORD_ARG, "0.0" });
		args.push_back({ mvPyDataType::Bool, "default_hide", mvArgType::KEYWORD_ARG, "False", "Default as a hidden/disabled column." });
		args.push_back({ mvPyDataType::Bool, "default_sort", mvArgType::KEYWORD_ARG, "False", "Default as a sorting column." });
		args.push_back({ mvPyDataType::Bool, "width_stretch", mvArgType::KEYWORD_ARG, "False", "Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp)." });
		args.push_back({ mvPyDataType::Bool, "width_fixed", mvArgType::KEYWORD_ARG, "False", "Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable)." });
		args.push_back({ mvPyDataType::Bool, "no_resize", mvArgType::KEYWORD_ARG, "False", "Disable manual resizing." });
		args.push_back({ mvPyDataType::Bool, "no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable manual reordering this column, this will also prevent other columns from crossing over this column." });
		args.push_back({ mvPyDataType::Bool, "no_hide", mvArgType::KEYWORD_ARG, "False", "Disable ability to hide/disable this column." });
		args.push_back({ mvPyDataType::Bool, "no_clip", mvArgType::KEYWORD_ARG, "False", "Disable clipping for this column (all NoClip columns will render in a same draw command)." });
		args.push_back({ mvPyDataType::Bool, "no_sort", mvArgType::KEYWORD_ARG, "False", "Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table)." });
		args.push_back({ mvPyDataType::Bool, "no_sort_ascending", mvArgType::KEYWORD_ARG, "False", "Disable ability to sort in the ascending direction." });
		args.push_back({ mvPyDataType::Bool, "no_sort_descending", mvArgType::KEYWORD_ARG, "False", "Disable ability to sort in the descending direction." });
		args.push_back({ mvPyDataType::Bool, "no_header_width", mvArgType::KEYWORD_ARG, "False", "Disable header text width contribution to automatic column width." });
		args.push_back({ mvPyDataType::Bool, "prefer_sort_ascending", mvArgType::KEYWORD_ARG, "True", "Make the initial sort direction Ascending when first sorting on this column (default)." });
		args.push_back({ mvPyDataType::Bool, "prefer_sort_descending", mvArgType::KEYWORD_ARG, "False", "Make the initial sort direction Descending when first sorting on this column." });
		args.push_back({ mvPyDataType::Bool, "indent_enable", mvArgType::KEYWORD_ARG, "False", "Use current Indent value when entering cell (default for column 0)." });
		args.push_back({ mvPyDataType::Bool, "indent_disable", mvArgType::KEYWORD_ARG, "False", "Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored." });

		mvPythonParserSetup setup;
		setup.about = "Adds a table column.";
		setup.category = { "Tables", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvTableColumn::mvTableColumn(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvTableColumn::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvTableColumn*>(item);
		_flags = titem->_flags;
		_init_width_or_weight = titem->_init_width_or_weight;
	}

	void mvTableColumn::draw(ImDrawList* drawlist, float x, float y)
	{
		_id = _uuid;
		ImGui::TableSetupColumn(_internalLabel.c_str(), _flags, _init_width_or_weight, _id);

		if (wasShownLastFrameReset())
			ImGui::TableSetColumnEnabled(_location, _enabled);
		else if(wasHiddenLastFrameReset())
			ImGui::TableSetColumnEnabled(_location, _enabled);
	}

	void mvTableColumn::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "init_width_or_weight")) _init_width_or_weight = ToFloat(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("default_hide", ImGuiTableColumnFlags_DefaultHide, _flags);
		flagop("default_sort", ImGuiTableColumnFlags_DefaultSort, _flags);
		flagop("width_stretch", ImGuiTableColumnFlags_WidthStretch, _flags);
		flagop("width_fixed", ImGuiTableColumnFlags_WidthFixed, _flags);
		flagop("no_resize", ImGuiTableColumnFlags_NoResize, _flags);
		flagop("no_reorder", ImGuiTableColumnFlags_NoReorder, _flags);
		flagop("no_hide", ImGuiTableColumnFlags_NoHide, _flags);
		flagop("no_clip", ImGuiTableColumnFlags_NoClip, _flags);
		flagop("no_sort", ImGuiTableColumnFlags_NoSort, _flags);
		flagop("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, _flags);
		flagop("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, _flags);
		flagop("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, _flags);
		flagop("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, _flags);
		flagop("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, _flags);
		flagop("indent_enable", ImGuiTableColumnFlags_IndentEnable, _flags);
		flagop("indent_disable", ImGuiTableColumnFlags_IndentDisable, _flags);

	}

	void mvTableColumn::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_init_width_or_weight = ToPyFloat(_init_width_or_weight);

		PyDict_SetItemString(dict, "init_width_or_weight", py_init_width_or_weight);

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			mvPyObject py_value = ToPyBool(flags & flag);
			PyDict_SetItemString(dict, keyword, py_value);
		};

		checkbitset("default_hide", ImGuiTableColumnFlags_DefaultHide, _flags);
		checkbitset("default_sort", ImGuiTableColumnFlags_DefaultSort, _flags);
		checkbitset("width_stretch", ImGuiTableColumnFlags_WidthStretch, _flags);
		checkbitset("width_fixed", ImGuiTableColumnFlags_WidthFixed, _flags);
		checkbitset("no_resize", ImGuiTableColumnFlags_NoResize, _flags);
		checkbitset("no_reorder", ImGuiTableColumnFlags_NoReorder, _flags);
		checkbitset("no_hide", ImGuiTableColumnFlags_NoHide, _flags);
		checkbitset("no_clip", ImGuiTableColumnFlags_NoClip, _flags);
		checkbitset("no_sort", ImGuiTableColumnFlags_NoSort, _flags);
		checkbitset("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, _flags);
		checkbitset("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, _flags);
		checkbitset("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, _flags);
		checkbitset("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, _flags);
		checkbitset("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, _flags);
		checkbitset("indent_enable", ImGuiTableColumnFlags_IndentEnable, _flags);
		checkbitset("indent_disable", ImGuiTableColumnFlags_IndentDisable, _flags);
	}

}