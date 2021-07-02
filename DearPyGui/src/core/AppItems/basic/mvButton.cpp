#include <utility>
#include "mvButton.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a button.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
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

		parser.addArg<mvPyDataType::Bool>("small", mvArgType::KEYWORD_ARG, "False", "Small button, useful for embedding in text.");
		parser.addArg<mvPyDataType::Bool>("arrow", mvArgType::KEYWORD_ARG, "False", "Arrow button, requires the direction keyword.");
		parser.addArg<mvPyDataType::Integer>("direction", mvArgType::KEYWORD_ARG, "0", "A cardinal direction for arrow.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvButton::mvButton(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
		m_state.m_applicableState = MV_STATE_HOVER | MV_STATE_ACTIVE | MV_STATE_FOCUSED | MV_STATE_CLICKED | MV_STATE_CONT_AVAIL |
			MV_STATE_VISIBLE | MV_STATE_ACTIVATED | MV_STATE_DEACTIVATED | MV_STATE_RECT_MIN | MV_STATE_RECT_MAX | MV_STATE_RECT_SIZE;
	}

	void mvButton::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------
		
		// show/hide
		if (!m_show)
			return;

		// focusing
		if (m_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			m_focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (m_dirtyPos)
			ImGui::SetCursorPos(m_state.getItemPos());

		// update widget's position state
		m_state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

		// set item width
		if (m_width != 0)
			ImGui::SetNextItemWidth((float)m_width);

		// set indent
		if (m_indent > 0.0f)
			ImGui::Indent(m_indent);

		// push font if a font object is attached
		if (m_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(m_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// handle enabled theming
		if (m_enabled)
		{
			// push class theme (if it exists)
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

			// push item theme (if it exists)
			if (m_theme)
				static_cast<mvTheme*>(m_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		// handled disabled theming
		else
		{
			// push class theme (if it exists)
			if (auto classTheme = getClassDisabledTheme())
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

			// push item theme (if it exists)
			if (m_disabledTheme)
				static_cast<mvTheme*>(m_disabledTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}


		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			// push imgui id to prevent name collisions
			ScopedID id(m_uuid);

			if (m_small_button)
			{
				if (ImGui::SmallButton(m_label.c_str()))
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
			}

			else if (m_arrow)
			{
				if (ImGui::ArrowButton(m_label.c_str(), m_direction))
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
			}

			else
			{
				if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
					mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
			}
		}

		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------
		m_state.m_hovered = ImGui::IsItemHovered();
		m_state.m_active = ImGui::IsItemActive();
		m_state.m_focused = ImGui::IsItemFocused();
		m_state.m_clicked = ImGui::IsItemClicked();
		m_state.m_visible = ImGui::IsItemVisible();
		m_state.m_activated = ImGui::IsItemActivated();
		m_state.m_deactivated = ImGui::IsItemDeactivated();
		m_state.m_rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
		m_state.m_rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
		m_state.m_rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
		m_state.m_contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (m_dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (m_indent > 0.0f)
			ImGui::Unindent(m_indent);

		// pop font off stack
		if (m_font)
			ImGui::PopFont();

		// handle popping styles
		if (m_enabled)
		{
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->customAction();

			if (m_theme)
				static_cast<mvTheme*>(m_theme.get())->customAction();
		}
		else
		{
			if (auto classTheme = getClassDisabledTheme())
				static_cast<mvTheme*>(classTheme.get())->customAction();

			if (m_disabledTheme)
				static_cast<mvTheme*>(m_disabledTheme.get())->customAction();
		}

		// handle widget's event handlers
		for (auto& item : m_children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// handle drag & drop payloads
		for (auto& item : m_children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// handle drag & drop if used
		if (m_dropCallback)
		{
			ScopedID id(m_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), m_uuid, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
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
		}

	}

	void mvButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "small", ToPyBool(m_small_button));
		PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
		PyDict_SetItemString(dict, "direction", ToPyInt(m_direction));
	}

}