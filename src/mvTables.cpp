#include "mvTables.h"
#include "mvContext.h"
#include "mvCore.h"
#include "mvItemRegistry.h"
#include "mvPyUtils.h"
#include "mvFontItems.h"
#include "mvThemes.h"

mvTableCell::mvTableCell(mvUUID uuid)
	: mvAppItem(uuid)
{

}

void mvTableCell::draw(ImDrawList* drawlist, float x, float y)
{
	ScopedID id(uuid);

	for (auto& item : childslots[1])
		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
}

mvTableColumn::mvTableColumn(mvUUID uuid)
	: mvAppItem(uuid)
{
}

void mvTableColumn::draw(ImDrawList* drawlist, float x, float y)
{
	_id = (ImGuiID)uuid;
	ImGui::TableSetupColumn(info.internalLabel.c_str(), _flags, _init_width_or_weight, _id);

	if (info.shownLastFrame)
	{
		info.shownLastFrame = false;
		ImGui::TableSetColumnEnabled(info.location, config.enabled);
	}

	if (info.hiddenLastFrame)
	{
		info.hiddenLastFrame = false;
		ImGui::TableSetColumnEnabled(info.location, config.enabled);
	}
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

mvTableRow::mvTableRow(mvUUID uuid)
	: mvAppItem(uuid)
{

}

mvTable::mvTable(mvUUID uuid)
	: mvAppItem(uuid)
{
}

void mvTable::draw(ImDrawList* drawlist, float x, float y)
{
	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------

	// show/hide
	if (!config.show)
		return;

	// push font if a font object is attached
	if (font)
	{
		ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(this);

	{
		ScopedID id(uuid);

		if (_columns == 0)
			return;

		auto row_renderer = [&](mvAppItem* row, mvAppItem* prev_visible_row=nullptr)
		{
			//TableNextRow() ends the previous row, if any, and determines background color for it.
			//We have to specify themes in a way that it captures the theme for the previous
			//*visible* row, not for the current one.  That's why we apply the theme *after*
			//TableNextRow() and leave it active through the next TableNextRow() call.
			ImGui::TableNextRow(0, (float)row->config.height);

			if (prev_visible_row)
				cleanup_local_theming(prev_visible_row);
			apply_local_theming(row);

			row->state.lastFrameUpdate = GContext->frame;
			row->state.visible = true;

			//int row_index = ImGui::TableGetRowIndex() + _tableHeader ? 1 : 0;
			int row_index = row->info.location;

			if (_rowColorsSet[row_index])
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, _rowColors[row_index]);

			if (_rowSelectionColorsSet[row_index])
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, _rowSelectionColors[row_index]);

			int column_index = -1;
			for (auto& cell : row->childslots[1])
			{

				// if tooltip, do not move column index
				if (cell->type == mvAppItemType::mvTooltip)
				{
					cell->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
					continue;
				}

				column_index++;

				if (column_index >= _columns)
					break;

				ImGui::TableSetColumnIndex(column_index);

				if (_columnColorsSet[column_index])
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, _columnColors[column_index]);

				if (_cellColorsSet[row_index][column_index])
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, _cellColors[row_index][column_index]);

                auto& columnItem = childslots[0][column_index];

                if(columnItem->config.enabled)
				{
					apply_local_theming(columnItem.get());
					apply_local_theming(cell.get());
				    cell->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
					cleanup_local_theming(cell.get());
					cleanup_local_theming(columnItem.get());
				}
			}
		};

		if (ImGui::BeginTable(info.internalLabel.c_str(), _columns, _flags,
			ImVec2((float)config.width, (float)config.height), (float)_inner_width))
		{
			state.lastFrameUpdate = GContext->frame;
			state.visible = true;

			ImGui::TableSetupScrollFreeze(_freezeColumns, _freezeRows);

			// setup columns
			for (auto& item : childslots[0])
			{
				// skip item if it's not shown
				if (!item->config.show)
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			}

			if (_tableHeader)
			{
				//ImGui::TableHeadersRow();
				ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
				for (int column = 0; column < childslots[0].size(); column++)
				{
					ImGui::TableSetColumnIndex(column);
					const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
					ImGui::PushID(column);
					ImGui::TableHeader(column_name);

					if (childslots[2][column])
					{
						// columns
						auto& item = childslots[2][column];
						// skip item if it's not shown
						if (!item->config.show)
							continue;

						item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
					}
					ImGui::PopID();
				}

			}

			if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			{
				if (sorts_specs->SpecsDirty)
				{
					if (sorts_specs->SpecsCount == 0)
						mvAddCallback(getCallback(false), uuid, GetPyNone(), config.user_data);
					else
					{

						// generate id map for columns
						std::unordered_map<ImGuiID, mvUUID> idMap;
						for (size_t i = 0; i < childslots[0].size(); i++)
							idMap[static_cast<mvTableColumn*>(childslots[0][i].get())->_id] = childslots[0][i]->uuid;

						std::vector<SortSpec> specs;
						for (int i = 0; i < sorts_specs->SpecsCount; i++)
						{
							const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[i];
							specs.push_back({ idMap[sort_spec->ColumnUserID], sort_spec->SortDirection == ImGuiSortDirection_Ascending ? 1 : -1 });
						}

						mvSubmitCallback([=]() {
							PyObject* pySpec = PyList_New(specs.size());
							for (size_t i = 0; i < specs.size(); i++)
							{
								PyObject* pySingleSpec = PyList_New(2);
								PyList_SetItem(pySingleSpec, 0, ToPyLong(specs[i].column));
								PyList_SetItem(pySingleSpec, 1, ToPyInt(specs[i].direction));
								PyList_SetItem(pySpec, i, pySingleSpec);
							}

							if (config.alias.empty())
								mvRunCallback(getCallback(false), uuid, pySpec, config.user_data);
							else
								mvRunCallback(getCallback(false), config.alias, pySpec, config.user_data);
							Py_XDECREF(pySpec);
							});
					}
					sorts_specs->SpecsDirty = false;
				}
			}

			std::shared_ptr<mvAppItem> prev_themed_row = nullptr;

			if (_rows != 0)
			{

				if (_imguiFilter.IsActive())
				{
					for (auto& row : childslots[1])
					{
						if (!_imguiFilter.PassFilter(row->config.filter.c_str()))
							continue;
						row_renderer(row.get());
					}
				}

				else if (_useClipper)
				{
					ImGuiListClipper clipper;
					clipper.Begin((int)childslots[1].size());

					while (clipper.Step())
					{
						for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
							row_renderer(childslots[1][row_n].get());

					}
					clipper.End();
				}
				else
				{
					for (auto& row : childslots[1])
					{
						if (row->config.show)
							row_renderer(row.get(), prev_themed_row.get());
							prev_themed_row = row;
					}
				}
			}

			// columns
			int columnnum = 0;
			for (auto& item : childslots[0])
			{
				ImGuiTableColumnFlags flags = ImGui::TableGetColumnFlags(columnnum);
				item->state.lastFrameUpdate = GContext->frame;
				item->state.visible = flags & ImGuiTableColumnFlags_IsVisible;
				item->state.hovered = flags & ImGuiTableColumnFlags_IsHovered;
                if(!item->config.enabled && item->state.visible) item->config.enabled = true;
				columnnum++;
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

			_scrollX = ImGui::GetScrollX();
			_scrollMaxX = ImGui::GetScrollMaxX();
			_scrollY = ImGui::GetScrollY();
			_scrollMaxY = ImGui::GetScrollMaxY();

			ImGui::EndTable();

			if (prev_themed_row)
				cleanup_local_theming(prev_themed_row.get());
		}
	}

	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(this);

}

void mvTable::onChildAdd(std::shared_ptr<mvAppItem> item)
{

	if (item->type == mvAppItemType::mvTableColumn)
	{
		_columns++;
		_columnColors.push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
		_columnColorsSet.push_back(false);

		for (int i = 0; i < childslots[1].size(); i++)
		{
			while (_cellColors[i].size() <= _columns)
			{
				_cellColorsSet[i].push_back(false);
				_cellColors[i].push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
			}
		}
		childslots[2].push_back(nullptr);
	}

	else if (item->type == mvAppItemType::mvTableRow)
	{
		_rows++;
		int location = item->info.location;
		_rowColors.insert(_rowColors.begin() + location, ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
		_rowColorsSet.insert(_rowColorsSet.begin() + location, false);
		_rowSelectionColors.insert(_rowSelectionColors.begin() + location, ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
		_rowSelectionColorsSet.insert(_rowSelectionColorsSet.begin() + location, false);

		// TODO: OMFG, this needs to be fixed, too.
		_cellColorsSet.push_back({});
		_cellColors.push_back({});
		for (int i = 0; i < _columns; i++)
		{
			_cellColorsSet.back().push_back(false);
			_cellColors.back().push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
		}
	}
}

void mvTable::onChildRemoved(std::shared_ptr<mvAppItem> item)
{
	int location = item->info.location;
	if (item->type == mvAppItemType::mvTableColumn)
	{
		childslots[2][location] = nullptr;
		_columns--;
		_columnColors.erase(_columnColors.begin() + location);
		_columnColorsSet.erase(_columnColorsSet.begin() + location);
	}
	else if (item->type == mvAppItemType::mvTableRow)
	{
		_rows--;
		_rowColors.erase(_rowColors.begin() + location);
		_rowColorsSet.erase(_rowColorsSet.begin() + location);
		_rowSelectionColors.erase(_rowSelectionColors.begin() + location);
		_rowSelectionColorsSet.erase(_rowSelectionColorsSet.begin() + location);
		_cellColorsSet.erase(_cellColorsSet.begin() + location);
		_cellColors.erase(_cellColors.begin() + location);
	}
}

void mvTable::onChildrenRemoved()
{
	_columns = (int)childslots[0].size();
	_rows = (int)childslots[1].size();

	_columnColors.clear();
	_columnColorsSet.clear();
	_rowColors.clear();
	_rowColorsSet.clear();
	_rowSelectionColors.clear();
	_rowSelectionColorsSet.clear();

	for (int i = 0; i < _columns; i++)
	{
		_columnColors.push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
		_columnColorsSet.push_back(false);
	}
}

void mvTable::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "freeze_rows")) _freezeRows = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "freeze_columns")) _freezeColumns = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "header_row")) _tableHeader = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "clipper")) _useClipper = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "inner_width")) _inner_width = (int)ToFloat(item);

	// helper for bit flipping
	auto flagop = [dict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	flagop("resizable", ImGuiTableFlags_Resizable, _flags);
	flagop("reorderable", ImGuiTableFlags_Reorderable, _flags);
	flagop("hideable", ImGuiTableFlags_Hideable, _flags);
	flagop("sortable", ImGuiTableFlags_Sortable, _flags);
	flagop("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, _flags);
	flagop("row_background", ImGuiTableFlags_RowBg, _flags);
	flagop("borders_innerH", ImGuiTableFlags_BordersInnerH, _flags);
	flagop("borders_outerH", ImGuiTableFlags_BordersOuterH, _flags);
	flagop("borders_innerV", ImGuiTableFlags_BordersInnerV, _flags);
	flagop("borders_outerV", ImGuiTableFlags_BordersOuterV, _flags);
	flagop("no_host_extendX", ImGuiTableFlags_NoHostExtendX, _flags);
	flagop("no_host_extendY", ImGuiTableFlags_NoHostExtendY, _flags);
	flagop("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, _flags);
	flagop("precise_widths", ImGuiTableFlags_PreciseWidths, _flags);
	flagop("no_clip", ImGuiTableFlags_NoClip, _flags);
	flagop("pad_outerX", ImGuiTableFlags_PadOuterX, _flags);
	flagop("no_pad_outerX", ImGuiTableFlags_NoPadOuterX, _flags);
	flagop("no_pad_innerX", ImGuiTableFlags_NoPadInnerX, _flags);
	flagop("scrollX", ImGuiTableFlags_ScrollX, _flags);
	flagop("scrollY", ImGuiTableFlags_ScrollY, _flags);
	flagop("sort_multi", ImGuiTableFlags_SortMulti, _flags);
	flagop("sort_tristate", ImGuiTableFlags_SortTristate, _flags);
	flagop("no_saved_settings", ImGuiTableFlags_NoSavedSettings, _flags);

	if (PyObject* item = PyDict_GetItemString(dict, "policy"))
	{

		int policy = ToInt(item);

		// remove old flags
		_flags &= ~ImGuiTableFlags_SizingFixedFit;
		_flags &= ~ImGuiTableFlags_SizingFixedSame;
		_flags &= ~ImGuiTableFlags_SizingStretchProp;
		_flags &= ~ImGuiTableFlags_SizingStretchSame;

		if (policy == ImGuiTableFlags_SizingFixedFit)
			_flags |= ImGuiTableFlags_SizingFixedFit;
		else if (policy == ImGuiTableFlags_SizingFixedSame)
			_flags |= ImGuiTableFlags_SizingFixedSame;
		else if (policy == ImGuiTableFlags_SizingStretchProp)
			_flags |= ImGuiTableFlags_SizingStretchProp;
		else
			_flags |= ImGuiTableFlags_SizingStretchSame;

	}

	//if (_flags & ImGuiTableFlags_BordersInnerV)
	//	_flags &= ~ImGuiTableFlags_Resizable;
}

void mvTable::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_freeze_rows = ToPyInt(_freezeRows);
	mvPyObject py_freeze_columns = ToPyInt(_freezeColumns);
	mvPyObject py_inner_width = ToPyInt(_inner_width);
	mvPyObject py_header_row = ToPyBool(_tableHeader);
	mvPyObject py_clipper = ToPyBool(_useClipper);

	PyDict_SetItemString(dict, "freeze_rows", py_freeze_rows);
	PyDict_SetItemString(dict, "freeze_columns", py_freeze_columns);
	PyDict_SetItemString(dict, "inner_width", py_inner_width);
	PyDict_SetItemString(dict, "header_row", py_header_row);
	PyDict_SetItemString(dict, "clipper", py_clipper);

	// helper to check and set bit
	auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
	{
		mvPyObject py_value = ToPyBool(flags & flag);
		PyDict_SetItemString(dict, keyword, py_value);
	};

	checkbitset("resizable", ImGuiTableFlags_Resizable, _flags);
	checkbitset("reorderable", ImGuiTableFlags_Reorderable, _flags);
	checkbitset("hideable", ImGuiTableFlags_Hideable, _flags);
	checkbitset("sortable", ImGuiTableFlags_Sortable, _flags);
	checkbitset("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, _flags);
	checkbitset("row_background", ImGuiTableFlags_RowBg, _flags);
	checkbitset("borders_innerH", ImGuiTableFlags_BordersInnerH, _flags);
	checkbitset("borders_outerH", ImGuiTableFlags_BordersOuterH, _flags);
	checkbitset("borders_innerV", ImGuiTableFlags_BordersInnerV, _flags);
	checkbitset("borders_outerV", ImGuiTableFlags_BordersOuterV, _flags);
	checkbitset("no_host_extendX", ImGuiTableFlags_NoHostExtendX, _flags);
	checkbitset("no_host_extendY", ImGuiTableFlags_NoHostExtendY, _flags);
	checkbitset("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, _flags);
	checkbitset("precise_widths", ImGuiTableFlags_PreciseWidths, _flags);
	checkbitset("no_clip", ImGuiTableFlags_NoClip, _flags);
	checkbitset("pad_outerX", ImGuiTableFlags_PadOuterX, _flags);
	checkbitset("no_pad_outerX", ImGuiTableFlags_NoPadOuterX, _flags);
	checkbitset("no_pad_innerX", ImGuiTableFlags_NoPadInnerX, _flags);
	checkbitset("scrollX", ImGuiTableFlags_ScrollX, _flags);
	checkbitset("scrollY", ImGuiTableFlags_ScrollY, _flags);
	checkbitset("sort_multi", ImGuiTableFlags_SortMulti, _flags);
	checkbitset("sort_tristate", ImGuiTableFlags_SortTristate, _flags);
	checkbitset("no_saved_settings", ImGuiTableFlags_NoSavedSettings, _flags);

	int flag = 0;

	if (_flags & ImGuiTableFlags_SizingFixedFit)
		flag = ImGuiTableFlags_SizingFixedFit;
	else if (_flags & ImGuiTableFlags_SizingFixedSame)
		flag = ImGuiTableFlags_SizingFixedSame;
	else if (_flags & ImGuiTableFlags_SizingStretchProp)
		flag = ImGuiTableFlags_SizingStretchProp;
	else if (_flags & ImGuiTableFlags_SizingStretchSame)
		flag = ImGuiTableFlags_SizingStretchSame;

	mvPyObject py_policy = ToPyInt(flag);
	PyDict_SetItemString(dict, "policy", py_policy);
}

PyObject* mvTable::getPyValue()
{
	return ToPyString(std::string(_imguiFilter.InputBuf));
}

void mvTable::setPyValue(PyObject* value)
{
	auto str_value = ToString(value);

	int i = 0;
	for (auto& character : str_value)
	{
		if (i > 254)
			break;

		_imguiFilter.InputBuf[i] = character;

		++i;
	}
	_imguiFilter.InputBuf[i] = 0;
	_imguiFilter.Build();
}
