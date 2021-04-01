#include "mvTableColumn.h"
#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTableColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", ""},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "init_width_or_weight", "init_width_or_weight", "0.0"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "default_hide", "Default as a hidden/disabled column.", "False"},
			{mvPythonDataType::Bool, "default_sort", "Default as a sorting column.", "False"},
			{mvPythonDataType::Bool, "width_stretch", "Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).", "False"},
			{mvPythonDataType::Bool, "width_fixed", "Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).", "False"},
			{mvPythonDataType::Bool, "no_resize", "Disable manual resizing.", "False"},
			{mvPythonDataType::Bool, "no_reorder", "Disable manual reordering this column, this will also prevent other columns from crossing over this column.", "False"},
			{mvPythonDataType::Bool, "no_hide", "Disable ability to hide/disable this column.", "False"},
			{mvPythonDataType::Bool, "no_clip", "Disable clipping for this column (all NoClip columns will render in a same draw command).", "False"},
			{mvPythonDataType::Bool, "no_sort", "Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table).", "False"},
			{mvPythonDataType::Bool, "no_sort_ascending", "Disable ability to sort in the ascending direction.", "False"},
			{mvPythonDataType::Bool, "no_sort_descending", "Disable ability to sort in the descending direction.", "False"},
			{mvPythonDataType::Bool, "no_header_width", "Disable header text width contribution to automatic column width.", "False"},
			{mvPythonDataType::Bool, "prefer_sort_ascending", "Make the initial sort direction Ascending when first sorting on this column (default).", "False"},
			{mvPythonDataType::Bool, "prefer_sort_descending", "Make the initial sort direction Descending when first sorting on this column.", "False"},
			{mvPythonDataType::Bool, "indent_enable", "Use current Indent value when entering cell (default for column 0).", "False"},
			{mvPythonDataType::Bool, "indent_disable", "Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.", "False"},

			}, "Changes to next column.", "None", "Containers") });
	}

	mvTableColumn::mvTableColumn(const std::string& name, float init_width_or_weight)
		: mvAppItem(name)
	{
		m_init_width_or_weight = init_width_or_weight;
	}

	mvTableColumn::~mvTableColumn()
	{
		if (auto ptrParent = getParent())
			static_cast<mvTable*>(ptrParent)->decrementColumns();
	}

	void mvTableColumn::draw()
	{
		ImGui::TableSetupColumn(m_core_config.name.c_str(), m_flags, m_init_width_or_weight);
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

	PyObject* mvTableColumn::add_table_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		float init_width_or_weight = 0.0f;
		int show = true;
		const char* parent = "";
		const char* before = "";
		int default_hide = false;
		int default_sort = false;
		int width_stretch = false;
		int width_fixed = false;
		int no_resize = false;
		int no_reorder = false;
		int no_hide = false;
		int no_clip = false;
		int no_sort = false;
		int no_sort_ascending = false;
		int no_sort_descending = false;
		int no_header_width = false;
		int prefer_sort_ascending = false;
		int prefer_sort_descening = false;
		int indent_enable = false;
		int indent_disable = false;



		if (!(mvApp::GetApp()->getParsers())["add_table_column"].parse(args, kwargs, __FUNCTION__,
			&name,
			&init_width_or_weight, &show, &parent, &before,
			&default_hide,
			&default_sort,
			&width_stretch,
			&width_fixed,
			&no_resize,
			&no_reorder,
			&no_hide,
			&no_clip,
			&no_sort,
			&no_sort_ascending,
			&no_sort_descending,
			&no_header_width,
			&prefer_sort_ascending,
			&prefer_sort_descening,
			&indent_enable,
			&indent_disable
		))
			return ToPyBool(false);

		auto item = CreateRef<mvTableColumn>(name, init_width_or_weight);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		if (auto ptrParent = item->getParent())
			static_cast<mvTable*>(ptrParent)->incrementColumns();

		return ToPyString(name);
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