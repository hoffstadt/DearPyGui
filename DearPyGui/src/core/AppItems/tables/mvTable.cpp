#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvTableColumn.h"

namespace Marvel {

	void mvTable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Tables", "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("header_row", mvArgType::KEYWORD_ARG, "True", "show headers at the top of the columns");

		parser.addArg<mvPyDataType::Integer>("inner_width", mvArgType::KEYWORD_ARG, "0");
		parser.addArg<mvPyDataType::Integer>("policy", mvArgType::KEYWORD_ARG, "0");

		parser.addArg<mvPyDataType::Bool>("sort_multi", mvArgType::KEYWORD_ARG, "False", "Hold shift when clicking headers to sort on multiple column.");
		parser.addArg<mvPyDataType::Bool>("sort_tristate", mvArgType::KEYWORD_ARG, "False", "Allow no sorting, disable default sorting.");

		parser.addArg<mvPyDataType::Bool>("resizable", mvArgType::KEYWORD_ARG, "False", "Enable resizing columns");
		parser.addArg<mvPyDataType::Bool>("reorderable", mvArgType::KEYWORD_ARG, "False", "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)");
		parser.addArg<mvPyDataType::Bool>("hideable", mvArgType::KEYWORD_ARG, "False", "Enable hiding/disabling columns in context menu.");
		parser.addArg<mvPyDataType::Bool>("sortable", mvArgType::KEYWORD_ARG, "False", "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.");
		parser.addArg<mvPyDataType::Bool>("context_menu_in_body", mvArgType::KEYWORD_ARG, "False", "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().");
		parser.addArg<mvPyDataType::Bool>("row_background", mvArgType::KEYWORD_ARG, "False", "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)");
		parser.addArg<mvPyDataType::Bool>("borders_innerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders between rows.");
		parser.addArg<mvPyDataType::Bool>("borders_outerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders at the top and bottom.");
		parser.addArg<mvPyDataType::Bool>("borders_innerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders between columns.");
		parser.addArg<mvPyDataType::Bool>("borders_outerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders on the left and right sides.");

		parser.addArg<mvPyDataType::Bool>("no_host_extendX", mvArgType::KEYWORD_ARG, "False", "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
		parser.addArg<mvPyDataType::Bool>("no_host_extendY", mvArgType::KEYWORD_ARG, "False", "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
		parser.addArg<mvPyDataType::Bool>("no_keep_columns_visible", mvArgType::KEYWORD_ARG, "False", "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.");
		parser.addArg<mvPyDataType::Bool>("precise_widths", mvArgType::KEYWORD_ARG, "False", "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
		parser.addArg<mvPyDataType::Bool>("no_clip", mvArgType::KEYWORD_ARG, "False", "Disable clipping rectangle for every individual columns.");
		parser.addArg<mvPyDataType::Bool>("pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.");
		parser.addArg<mvPyDataType::Bool>("no_pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is off. Disable outer-most padding.");
		parser.addArg<mvPyDataType::Bool>("no_pad_innerX", mvArgType::KEYWORD_ARG, "False", "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).");
		parser.addArg<mvPyDataType::Bool>("scrollX", mvArgType::KEYWORD_ARG, "False", "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX.");
		parser.addArg<mvPyDataType::Bool>("scrollY", mvArgType::KEYWORD_ARG, "False", "Enable vertical scrolling.");

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

			// columns
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

			if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			{
				if (sorts_specs->SpecsDirty)
				{
					if (sorts_specs->SpecsCount == 0)
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, nullptr);
					else
					{

						// generate id map for columns
						std::unordered_map<ImGuiID, std::string> idMap;
						for (int i = 0; i < m_children[0].size(); i++)
							idMap[static_cast<mvTableColumn*>(m_children[0][i].get())->m_id] = m_children[0][i]->getName();

						std::vector<SortSpec> specs;
						for (int i = 0; i < sorts_specs->SpecsCount; i++)
						{
							const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[i];
							specs.push_back({ idMap[sort_spec->ColumnUserID], sort_spec->SortDirection == ImGuiSortDirection_Ascending ? 1 : -1 });
						}

						mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
							PyObject* pySpec = PyList_New(specs.size());
							for (int i = 0; i < specs.size(); i++)
							{
								PyObject* pySingleSpec = PyList_New(2);
								PyList_SetItem(pySingleSpec, 0, ToPyString(specs[i].column));
								PyList_SetItem(pySingleSpec, 1, ToPyInt(specs[i].direction));
								PyList_SetItem(pySpec, i, pySingleSpec);
							}
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_name, pySpec);
							Py_XDECREF(pySpec);
							});
					}
					sorts_specs->SpecsDirty = false;
				}
			}

			ImGui::TableNextColumn();

			for (auto& item : m_children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}

			ImGuiListClipper clipper;
			clipper.Begin(m_children[2].size());
			while (clipper.Step())
			{
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
				{
					auto& item = m_children[2][row_n];
					if (!item->preDraw())
						continue;

					ImGui::TableNextRow();
					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

					item->postDraw();
				}
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
		flagop("sort_multi", ImGuiTableFlags_SortMulti, m_flags);
		flagop("sort_tristate", ImGuiTableFlags_SortTristate, m_flags);

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
		checkbitset("sort_multi", ImGuiTableFlags_SortMulti, m_flags);
		checkbitset("sort_tristate", ImGuiTableFlags_SortTristate, m_flags);
		
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