#include "mvTableColumn.h"
#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTableColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{


		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("init_width_or_weight", mvArgType::KEYWORD, "0.0");
		parser.addArg<mvPyDataType::Bool>("default_hide", mvArgType::KEYWORD, "False", "Default as a hidden/disabled column.");
		parser.addArg<mvPyDataType::Bool>("default_sort", mvArgType::KEYWORD, "False", "Default as a sorting column.");
		parser.addArg<mvPyDataType::Bool>("width_stretch", mvArgType::KEYWORD, "False", "Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).");
		parser.addArg<mvPyDataType::Bool>("width_fixed", mvArgType::KEYWORD, "False", "Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).");
		parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD, "False", "Disable manual resizing.");
		parser.addArg<mvPyDataType::Bool>("no_reorder", mvArgType::KEYWORD, "False", "Disable manual reordering this column, this will also prevent other columns from crossing over this column.");
		parser.addArg<mvPyDataType::Bool>("no_hide", mvArgType::KEYWORD, "False", "Disable ability to hide/disable this column.");
		parser.addArg<mvPyDataType::Bool>("no_clip", mvArgType::KEYWORD, "False", "Disable clipping for this column (all NoClip columns will render in a same draw command).");
		parser.addArg<mvPyDataType::Bool>("no_sort", mvArgType::KEYWORD, "False", "Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table).");
		parser.addArg<mvPyDataType::Bool>("no_sort_ascending", mvArgType::KEYWORD, "False", "Disable ability to sort in the ascending direction.");
		parser.addArg<mvPyDataType::Bool>("no_sort_descending", mvArgType::KEYWORD, "False", "Disable ability to sort in the descending direction.");
		parser.addArg<mvPyDataType::Bool>("no_header_width", mvArgType::KEYWORD, "False", "Disable header text width contribution to automatic column width.");
		parser.addArg<mvPyDataType::Bool>("prefer_sort_ascending", mvArgType::KEYWORD, "False", "Make the initial sort direction Ascending when first sorting on this column (default).");
		parser.addArg<mvPyDataType::Bool>("prefer_sort_descending", mvArgType::KEYWORD, "False", "Make the initial sort direction Descending when first sorting on this column.");
		parser.addArg<mvPyDataType::Bool>("indent_enable", mvArgType::KEYWORD, "False", "Use current Indent value when entering cell (default for column 0).");
		parser.addArg<mvPyDataType::Bool>("indent_disable", mvArgType::KEYWORD, "False", "Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTableColumn::mvTableColumn(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvTableColumn::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::TableSetupColumn(m_name.c_str(), m_flags, m_init_width_or_weight);
	}

	bool mvTableColumn::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTable)
			return true;

		mvThrowPythonError(1000, "mvTableColumn parent must be a table.");
		MV_ITEM_REGISTRY_ERROR("mvTableColumn parent must be a table.");
		assert(false);
		return false;
	}

	void mvTableColumn::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "init_width_or_weight")) m_init_width_or_weight = ToFloat(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("default_hide", ImGuiTableColumnFlags_DefaultHide, m_flags);
		flagop("default_sort", ImGuiTableColumnFlags_DefaultSort, m_flags);
		flagop("width_stretch", ImGuiTableColumnFlags_WidthStretch, m_flags);
		flagop("width_fixed", ImGuiTableColumnFlags_WidthFixed, m_flags);
		flagop("no_resize", ImGuiTableColumnFlags_NoResize, m_flags);
		flagop("no_reorder", ImGuiTableColumnFlags_NoReorder, m_flags);
		flagop("no_hide", ImGuiTableColumnFlags_NoHide, m_flags);
		flagop("no_clip", ImGuiTableColumnFlags_NoClip, m_flags);
		flagop("no_sort", ImGuiTableColumnFlags_NoSort, m_flags);
		flagop("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, m_flags);
		flagop("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, m_flags);
		flagop("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, m_flags);
		flagop("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, m_flags);
		flagop("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, m_flags);
		flagop("indent_enable", ImGuiTableColumnFlags_IndentEnable, m_flags);
		flagop("indent_disable", ImGuiTableColumnFlags_IndentDisable, m_flags);

	}

	void mvTableColumn::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "init_width_or_weight", ToPyFloat(m_init_width_or_weight));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("default_hide", ImGuiTableColumnFlags_DefaultHide, m_flags);
		checkbitset("default_sort", ImGuiTableColumnFlags_DefaultSort, m_flags);
		checkbitset("width_stretch", ImGuiTableColumnFlags_WidthStretch, m_flags);
		checkbitset("width_fixed", ImGuiTableColumnFlags_WidthFixed, m_flags);
		checkbitset("no_resize", ImGuiTableColumnFlags_NoResize, m_flags);
		checkbitset("no_reorder", ImGuiTableColumnFlags_NoReorder, m_flags);
		checkbitset("no_hide", ImGuiTableColumnFlags_NoHide, m_flags);
		checkbitset("no_clip", ImGuiTableColumnFlags_NoClip, m_flags);
		checkbitset("no_sort", ImGuiTableColumnFlags_NoSort, m_flags);
		checkbitset("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, m_flags);
		checkbitset("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, m_flags);
		checkbitset("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, m_flags);
		checkbitset("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, m_flags);
		checkbitset("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, m_flags);
		checkbitset("indent_enable", ImGuiTableColumnFlags_IndentEnable, m_flags);
		checkbitset("indent_disable", ImGuiTableColumnFlags_IndentDisable, m_flags);
	}

}