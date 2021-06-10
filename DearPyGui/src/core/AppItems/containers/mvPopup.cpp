#include "mvPopup.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvPopup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" });
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_SHOW |
				MV_PARSER_ARG_POS)
			);

			parser.addArg<mvPyDataType::Integer>("mousebutton", mvArgType::KEYWORD_ARG, "1", "The mouse code that will trigger the popup.");
			parser.addArg<mvPyDataType::Bool>("modal", mvArgType::KEYWORD_ARG, "False");

			parser.finalize();

			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "close_popup", parser });
		}
	}

	mvPopup::mvPopup(mvUUID uuid)
		: mvBoolPtrBase(uuid)
	{
	}

	void mvPopup::setLabel(const std::string& value)
	{
		m_specificedlabel = value;
		m_label = value + "###" + std::to_string(m_uuid);
	}

	void mvPopup::closePopup()
	{
		m_close = true;
	}

	void mvPopup::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id(m_parentAddress);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_modal)
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(m_button))
				ImGui::OpenPopup(m_label.c_str());

			if (ImGui::BeginPopupModal(m_label.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{

				if (m_close)
				{
					ImGui::CloseCurrentPopup();
					m_close = false;
				}

				//we do this so that the children dont get the theme
				scope.cleanup();

				for (mvRef<mvAppItem> item : m_children[1])
				{
					if (!item->preDraw())
						continue;

					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

					item->postDraw();
				}

				ImGui::EndPopup();
			}
		}

		else
		{
			if (ImGui::BeginPopupContextItem(m_label.c_str(), m_button))
			{

				//we do this so that the children dont get the theme
				scope.cleanup();

				for (mvRef<mvAppItem> item : m_children[1])
				{
					// skip item if it's not shown
					if (!item->m_show)
						continue;

					// set item width
					if (item->m_width > 0)
						ImGui::SetNextItemWidth((float)item->m_width);

					item->draw(drawlist, x, y);

					item->getState().update();
				}

				// allows this item to have a render callback
				registerWindowFocusing();

				ImGui::EndPopup();
			}
		}
	}

	void mvPopup::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "parent"))
		{
			auto parentName = ToUUID(item);
			auto parent = mvApp::GetApp()->getItemRegistry().getItem(parentName);
			m_parentAddress = parent;
		}
		if (PyObject* item = PyDict_GetItemString(dict, "modal")) m_modal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "mousebutton")) m_button = ToInt(item);

	}

	void mvPopup::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "modal", ToPyBool(m_modal));
		PyDict_SetItemString(dict, "mousebutton", ToPyInt(m_button));
	}

	PyObject* mvPopup::close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID popup;

		if (!(mvApp::GetApp()->getParsers())["close_popup"].parse(args, kwargs, __FUNCTION__, &popup))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto item = mvApp::GetApp()->getItemRegistry().getItem(popup);

		if (item == nullptr)
		{
			mvThrowPythonError(1000, std::to_string(popup) + " popup does not exist.");
			return GetPyNone();
		}

		mvPopup* pop;
		if (item->getType() == mvAppItemType::mvPopup)
			pop = static_cast<mvPopup*>(item);
		else
		{
			mvThrowPythonError(1000, std::to_string(popup) + " is not a popup.");
			return GetPyNone();
		}

		pop->closePopup();

		return GetPyNone();
	}

}