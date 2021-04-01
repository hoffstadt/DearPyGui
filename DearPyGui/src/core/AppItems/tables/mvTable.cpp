#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "header_row", "show headers at the top of the columns", "True"},
			{mvPythonDataType::Integer, "width", "", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Integer, "inner_width", "", "0"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "resizable", "Enable resizing columns", "False"},
	        {mvPythonDataType::Bool, "reorderable", "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)", "False"},
	        {mvPythonDataType::Bool, "hideable", "Enable hiding/disabling columns in context menu.", "False"},
	        {mvPythonDataType::Bool, "sortable", "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.", "False"},
	        {mvPythonDataType::Bool, "context_menu_in_body", "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().", "False"},
			{mvPythonDataType::Bool, "row_background", "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)", "False"},
	        {mvPythonDataType::Bool, "borders_innerH", "Draw horizontal borders between rows.", "False"},
	        {mvPythonDataType::Bool, "borders_outerH", "Draw horizontal borders at the top and bottom.", "False"},
	        {mvPythonDataType::Bool, "borders_innerV", "Draw vertical borders between columns.", "False"},
	        {mvPythonDataType::Bool, "borders_outerV", "Draw vertical borders on the left and right sides.", "False"},
	        {mvPythonDataType::Integer, "policy", "sizing policy", "0"},
			{mvPythonDataType::Bool, "no_host_extendX", "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.", "False"},
			{mvPythonDataType::Bool, "no_host_extendY", "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.", "False"},
			{mvPythonDataType::Bool, "no_keep_columns_visible", "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.", "False"},
			{mvPythonDataType::Bool, "precise_widths", "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.", "False"},
			{mvPythonDataType::Bool, "no_clip", "Disable clipping rectangle for every individual columns.", "False"},
	        {mvPythonDataType::Bool, "pad_outerX", "Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.", "False"},
	        {mvPythonDataType::Bool, "no_pad_outerX", "Default if BordersOuterV is off. Disable outer-most padding.", "False"},
	        {mvPythonDataType::Bool, "no_pad_innerX", "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).", "False"},
	        {mvPythonDataType::Bool, "scrollX", "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX.", "False"},
	        {mvPythonDataType::Bool, "scrollY", "Enable horizontal vertical.", "False"},

		}, "Adds managed columns.", "None", "Containers") });
	}

	mvTable::mvTable(const std::string& name, int inner_width)
		: mvAppItem(name)
	{
		m_description.container = true;
		m_inner_width = inner_width;
	}

	const std::string& mvTable::getLastColumnAdded() const
	{
		return m_lastColumnAdded;
	}

	void mvTable::setLastColumnAdded(const std::string& name)
	{
		m_lastColumnAdded = name;
	}

	void mvTable::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::BeginTable(m_name.c_str(), m_columns, m_flags, 
			ImVec2(m_width, m_height), m_inner_width))
		{

			for (auto& item : m_children0)
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw(drawlist, x, y);

				item->getState().update();
			}

			if (m_tableHeader)
				ImGui::TableHeadersRow();

			ImGui::TableNextColumn();

			for (auto& item : m_children1)
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw(drawlist, x, y);

				item->getState().update();
			}
			ImGui::EndTable();
		}

	}

	void mvTable::incrementColumns()
	{
		m_columns++;
	}

	void mvTable::decrementColumns()
	{
		m_columns--;
	}

	PyObject* mvTable::add_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int header_row = true;
		int width = 0;
		int height = 0;
		int inner_width = 0;
		int show = true;
		const char* parent = "";
		const char* before = "";
		int resizable = false;  
		int reorderable = false;
		int hideable = false;   
		int sortable = false;   
		int context_menu_in_body = false;		  
		int row_background = false;
		int borders_innerH = false;
		int borders_outerH = false;
		int borders_innerV = false;
		int borders_outerV = false;
		int policy = 0;
		int no_host_extendX = false;
		int no_host_extendY = false;
		int no_keep_columns_visible = false; 
		int precise_widths = false;
		int no_clip = false;
		int pad_outerX = false;
		int no_pad_outerX = false; 
		int no_pad_innerX = false; 
		int scrollX = false; 
		int scrollY = false; 

		if (!(mvApp::GetApp()->getParsers())["add_table"].parse(args, kwargs, __FUNCTION__,
			&name, &header_row, &width, &height, &inner_width, &show, &parent, &before,
			&resizable,
			&reorderable,
			&hideable,
			&sortable,
			&context_menu_in_body,
			&row_background,
			&borders_innerH,
			&borders_outerH,
			&borders_innerV,
			&borders_outerV,
			&policy,
			&no_host_extendX,
			&no_host_extendY,
			&no_keep_columns_visible,
			&precise_widths,
			&no_clip,
			&pad_outerX,
			&no_pad_outerX,
			&no_pad_innerX,
			&scrollX,
			&scrollY
		))
			return ToPyBool(false);

		auto item = CreateRef<mvTable>(name, inner_width);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return ToPyString(name);
	}

	void mvTable::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "header_row")) m_tableHeader = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "inner_width")) m_inner_width = ToFloat(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("resizable", ImGuiTableFlags_Resizable, m_flags);
		flagop("reorderable", ImGuiTableFlags_Reorderable, m_flags);
		flagop("hideable", ImGuiTableFlags_Hideable, m_flags);
		flagop("sortable", ImGuiTableFlags_Sortable, m_flags);
		flagop("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, m_flags);
		flagop("row_background", ImGuiTableFlags_RowBg, m_flags);
		flagop("borders_innerH", ImGuiTableFlags_BordersInnerH, m_flags);
		flagop("borders_outerH", ImGuiTableFlags_BordersOuterH, m_flags);
		flagop("borders_innerV", ImGuiTableFlags_BordersInnerV, m_flags);
		flagop("borders_outerV", ImGuiTableFlags_BordersOuterV, m_flags);
		flagop("no_host_extendX", ImGuiTableFlags_NoHostExtendX, m_flags);
		flagop("no_host_extendY", ImGuiTableFlags_NoHostExtendY, m_flags);
		flagop("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, m_flags);
		flagop("precise_widths", ImGuiTableFlags_PreciseWidths, m_flags);
		flagop("no_clip", ImGuiTableFlags_NoClip, m_flags);
		flagop("pad_outerX", ImGuiTableFlags_PadOuterX, m_flags);
		flagop("no_pad_outerX", ImGuiTableFlags_NoPadOuterX, m_flags);
		flagop("no_pad_innerX", ImGuiTableFlags_NoPadInnerX, m_flags);
		flagop("scrollX", ImGuiTableFlags_ScrollX, m_flags);
		flagop("scrollY", ImGuiTableFlags_ScrollY, m_flags);

		if (PyObject* item = PyDict_GetItemString(dict, "policy"))
		{
			int policy = ToInt(item);

			// remove old flags
			m_flags &= ~ImGuiTableFlags_SizingFixedFit;
			m_flags &= ~ImGuiTableFlags_SizingFixedSame;
			m_flags &= ~ImGuiTableFlags_SizingStretchProp;
			m_flags &= ~ImGuiTableFlags_SizingStretchSame;

			if (policy == ImGuiTableFlags_SizingFixedFit)
				m_flags |= ImGuiTableFlags_SizingFixedFit;
			else if(policy == ImGuiTableFlags_SizingFixedSame)
				m_flags |= ImGuiTableFlags_SizingFixedSame;
			else if (policy == ImGuiTableFlags_SizingStretchProp)
				m_flags |= ImGuiTableFlags_SizingStretchProp;
			else
				m_flags |= ImGuiTableFlags_SizingStretchSame;

		}
}

	void mvTable::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "inner_width", ToPyInt(m_inner_width));
		PyDict_SetItemString(dict, "header_row", ToPyBool(m_tableHeader));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		checkbitset("resizable", ImGuiTableFlags_Resizable, m_flags);
		checkbitset("reorderable", ImGuiTableFlags_Reorderable, m_flags);
		checkbitset("hideable", ImGuiTableFlags_Hideable, m_flags);
		checkbitset("sortable", ImGuiTableFlags_Sortable, m_flags);
		checkbitset("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, m_flags);
		checkbitset("row_background", ImGuiTableFlags_RowBg, m_flags);
		checkbitset("borders_innerH", ImGuiTableFlags_BordersInnerH, m_flags);
		checkbitset("borders_outerH", ImGuiTableFlags_BordersOuterH, m_flags);
		checkbitset("borders_innerV", ImGuiTableFlags_BordersInnerV, m_flags);
		checkbitset("borders_outerV", ImGuiTableFlags_BordersOuterV, m_flags);
		checkbitset("no_host_extendX", ImGuiTableFlags_NoHostExtendX, m_flags);
		checkbitset("no_host_extendY", ImGuiTableFlags_NoHostExtendY, m_flags);
		checkbitset("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, m_flags);
		checkbitset("precise_widths", ImGuiTableFlags_PreciseWidths, m_flags);
		checkbitset("no_clip", ImGuiTableFlags_NoClip, m_flags);
		checkbitset("pad_outerX", ImGuiTableFlags_PadOuterX, m_flags);
		checkbitset("no_pad_outerX", ImGuiTableFlags_NoPadOuterX, m_flags);
		checkbitset("no_pad_innerX", ImGuiTableFlags_NoPadInnerX, m_flags);
		checkbitset("scrollX", ImGuiTableFlags_ScrollX, m_flags);
		checkbitset("scrollY", ImGuiTableFlags_ScrollY, m_flags);
		
		if(m_flags & ImGuiTableFlags_SizingFixedFit)
			PyDict_SetItemString(dict, "policy", ToPyInt(ImGuiTableFlags_SizingFixedFit));
		else if (m_flags & ImGuiTableFlags_SizingFixedSame)
			PyDict_SetItemString(dict, "policy", ToPyInt(ImGuiTableFlags_SizingFixedSame));
		else if (m_flags & ImGuiTableFlags_SizingStretchProp)
			PyDict_SetItemString(dict, "policy", ToPyInt(ImGuiTableFlags_SizingStretchProp));
		else if (m_flags & ImGuiTableFlags_SizingStretchSame)
			PyDict_SetItemString(dict, "policy", ToPyInt(ImGuiTableFlags_SizingStretchSame));
	}

}