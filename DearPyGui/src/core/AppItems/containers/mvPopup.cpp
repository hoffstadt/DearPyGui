#include "mvPopup.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvPopup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_popup", mvPythonParser({
			{mvPythonDataType::String, "popupparent", "Parent that the popup will be assigned to."},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "mousebutton", "The mouse code that will trigger the popup.", "1"},
			{mvPythonDataType::Bool, "modal", "", "False"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width", "", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end.",
		"None", "Containers") });

		parsers->insert({ "close_popup", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Closes a popup.") });
	}

	mvPopup::mvPopup(const std::string& name, mvAppItem* parentAddress)
		: mvBoolPtrBase(name, false), m_parentAddress(parentAddress)
	{
		m_description.container = true;
		m_description.addAfterRequired = true;
	}

	void mvPopup::closePopup()
	{
		m_close = true;
	}

	void mvPopup::draw()
	{

		ScopedID id(m_parentAddress);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_modal)
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(m_button))
				ImGui::OpenPopup(m_core_config.name.c_str());

			if (ImGui::BeginPopupModal(m_core_config.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{

				if (m_close)
				{
					ImGui::CloseCurrentPopup();
					m_close = false;
				}

				//we do this so that the children dont get the theme
				scope.cleanup();

				for (mvRef<mvAppItem> item : m_children)
				{
					// skip item if it's not shown
					if (!item->m_core_config.show)
						continue;

					// set item width
					if (item->m_core_config.width != 0)
						ImGui::SetNextItemWidth((float)item->m_core_config.width);

					item->draw();

					item->getState().update();
				}

				ImGui::EndPopup();
			}
		}

		else
		{
			if (ImGui::BeginPopupContextItem(m_core_config.name.c_str(), m_button))
			{

				//we do this so that the children dont get the theme
				scope.cleanup();

				for (mvRef<mvAppItem> item : m_children)
				{
					// skip item if it's not shown
					if (!item->m_core_config.show)
						continue;

					// set item width
					if (item->m_core_config.width > 0)
						ImGui::SetNextItemWidth((float)item->m_core_config.width);

					item->draw();

					item->getState().update();
				}

				// allows this item to have a render callback
				registerWindowFocusing();

				ImGui::EndPopup();
			}
		}
	}

#ifndef MV_CPP

	void mvPopup::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "modal")) m_modal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "mousebutton")) m_button = ToInt(item);

	}

	void mvPopup::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "modal", ToPyBool(m_modal));
		PyDict_SetItemString(dict, "mousebutton", ToPyInt(m_button));
	}

	PyObject* add_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popupparent;
		const char* name;
		int mousebutton = 1;
		int modal = false;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent,
			&name, &mousebutton, &modal, &parent, &before, &width, &height, &show))
			return ToPyBool(false);

		auto PopupParent = mvApp::GetApp()->getItemRegistry().getItem(popupparent);

		auto item = CreateRef<mvPopup>(name, PopupParent.get());

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, popupparent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;

		if (!(mvApp::GetApp()->getParsers())["close_popup"].parse(args, kwargs, __FUNCTION__, &popup))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		auto item = mvApp::GetApp()->getItemRegistry().getItem(popup);

		if (item == nullptr)
		{
			std::string message = popup;
			ThrowPythonException(message + " popup does not exist.");
			return GetPyNone();
		}

		mvPopup* pop;
		if (item->getType() == mvAppItemType::mvPopup)
			pop = static_cast<mvPopup*>(item.get());
		else
		{
			ThrowPythonException(std::string(popup) + " is not a popup.");
			return GetPyNone();
		}

		pop->closePopup();

		return GetPyNone();
	}

#endif
}