#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("header_row", mvArgType::KEYWORD, "True", "show headers at the top of the columns");

		parser.addArg<mvPyDataType::Integer>("inner_width", mvArgType::KEYWORD, "0");
		parser.addArg<mvPyDataType::Integer>("policy", mvArgType::KEYWORD, "0");

		parser.addArg<mvPyDataType::Bool>("resizable", mvArgType::KEYWORD, "False", "Enable resizing columns");
		parser.addArg<mvPyDataType::Bool>("reorderable", mvArgType::KEYWORD, "False", "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)");
		parser.addArg<mvPyDataType::Bool>("hideable", mvArgType::KEYWORD, "False", "Enable hiding/disabling columns in context menu.");
		parser.addArg<mvPyDataType::Bool>("sortable", mvArgType::KEYWORD, "False", "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.");
		parser.addArg<mvPyDataType::Bool>("context_menu_in_body", mvArgType::KEYWORD, "False", "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().");
		parser.addArg<mvPyDataType::Bool>("row_background", mvArgType::KEYWORD, "False", "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)");
		parser.addArg<mvPyDataType::Bool>("borders_innerH", mvArgType::KEYWORD, "False", "Draw horizontal borders between rows.");
		parser.addArg<mvPyDataType::Bool>("borders_outerH", mvArgType::KEYWORD, "False", "Draw horizontal borders at the top and bottom.");
		parser.addArg<mvPyDataType::Bool>("borders_innerV", mvArgType::KEYWORD, "False", "Draw vertical borders between columns.");
		parser.addArg<mvPyDataType::Bool>("borders_outerV", mvArgType::KEYWORD, "False", "Draw vertical borders on the left and right sides.");

		parser.addArg<mvPyDataType::Bool>("no_host_extendX", mvArgType::KEYWORD, "False", "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
		parser.addArg<mvPyDataType::Bool>("no_host_extendY", mvArgType::KEYWORD, "False", "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
		parser.addArg<mvPyDataType::Bool>("no_keep_columns_visible", mvArgType::KEYWORD, "False", "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.");
		parser.addArg<mvPyDataType::Bool>("precise_widths", mvArgType::KEYWORD, "False", "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
		parser.addArg<mvPyDataType::Bool>("no_clip", mvArgType::KEYWORD, "False", "Disable clipping rectangle for every individual columns.");
		parser.addArg<mvPyDataType::Bool>("pad_outerX", mvArgType::KEYWORD, "False", "Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.");
		parser.addArg<mvPyDataType::Bool>("no_pad_outerX", mvArgType::KEYWORD, "False", "Default if BordersOuterV is off. Disable outer-most padding.");
		parser.addArg<mvPyDataType::Bool>("no_pad_innerX", mvArgType::KEYWORD, "False", "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).");
		parser.addArg<mvPyDataType::Bool>("scrollX", mvArgType::KEYWORD, "False", "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX.");
		parser.addArg<mvPyDataType::Bool>("scrollY", mvArgType::KEYWORD, "False", "Enable horizontal vertical.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTable::mvTable(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvTable::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::BeginTable(m_name.c_str(), m_columns, m_flags, 
			ImVec2((float)m_width, (float)m_height), (float)m_inner_width))
		{

			for (auto& item : m_children[0])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}

			if (m_tableHeader)
				ImGui::TableHeadersRow();

			ImGui::TableNextColumn();

			for (auto& item : m_children[1])
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

	void mvTable::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvTableColumn)
			m_columns++;
	}

	void mvTable::onChildRemoved(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvTableColumn)
			m_columns--;
	}

	void mvTable::onChildrenRemoved()
	{
		m_columns = 0;
	}

	void mvTable::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "header_row")) m_tableHeader = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "inner_width")) m_inner_width = (int)ToFloat(item);

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

	void mvTable::getSpecificConfiguration(PyObject* dict)
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