#include "mvAppItem.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel{

	mvAppItem::mvAppItem(const std::string& name)
	{
		m_name = name;
		m_label = name + "###" + name;
		m_state.setParent(this);
	}

	void mvAppItem::checkConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		auto configKeys = ToStringVect(PyDict_Keys(dict));
		auto parserKeywords = (*mvApp::GetApp()->getParsers())[getParserCommand()].getKeywords();
		if (parserKeywords.empty())
		{
			ThrowPythonException("\"" + m_name + "\" could not find a parser that matched \"" + getParserCommand() + "\".");
			return;
		}
		for (const auto& key : configKeys)
		{
			size_t i = 0;
			while (i < parserKeywords.size() - 1)
			{
				if (key == parserKeywords[i])
				{
					break;
				}
				i++;
			}
			if (i == parserKeywords.size() - 1)
			{
				ThrowPythonException("\"" + key + "\" configuration does not exist in \"" + m_name + "\".");
			}
		}
	}

	void mvAppItem::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "name")) m_name = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "label")) setLabel(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "tip")) m_tip =ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "width")) setWidth(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "height")) setHeight(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "show")) m_show = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "source")) setDataSource(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) setEnabled(ToBool(item));
	}

	void mvAppItem::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "name",    ToPyString(m_name));
		PyDict_SetItemString(dict, "label",   ToPyString(m_label));
		PyDict_SetItemString(dict, "source",  ToPyString(m_dataSource));
		PyDict_SetItemString(dict, "tip",     ToPyString(m_tip));
		PyDict_SetItemString(dict, "show",    ToPyBool  (m_show));
		PyDict_SetItemString(dict, "enabled", ToPyBool  (m_enabled));
		PyDict_SetItemString(dict, "width",   ToPyInt   (m_width));
		PyDict_SetItemString(dict, "height",  ToPyInt   (m_height));
	}

	void mvAppItem::registerWindowFocusing()
	{
		if (ImGui::IsWindowFocused())
		{

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != m_name)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", m_name) });

		}
	}

	void mvAppItem::setCallback(PyObject* callback) 
	{ 
		if (callback == Py_None)
		{
			m_callback = nullptr;
			return;
		}
		m_callback = callback;
	}

	void mvAppItem::resetState()
	{
		m_state.reset();
		for (auto& item : m_children)
			item->resetState();

	}

	bool  mvAppItem::moveChildUp(const std::string& name)
	{
		bool found = false;
		int index = 0;

		// check children
		for (size_t i = 0; i<m_children.size(); i++)
		{

			if (m_children[i]->m_name == name)
			{
				found = true;
				index = (int)i;
				break;
			}

			if (m_children[i]->getDescription().container)
			{
				found = m_children[i]->moveChildUp(name);
				if (found)
					return true;
			}

		}

		if (found)
		{
			if (index > 0)
			{
				auto upperitem = m_children[index - 1];
				auto loweritem = m_children[index];

				m_children[index] = upperitem;
				m_children[index-1] = loweritem;
			}

			return true;
		}

		return false;

	}

	bool  mvAppItem::moveChildDown(const std::string& name)
	{
		bool found = false;
		size_t index = 0;

		// check children
		for (size_t i = 0; i < m_children.size(); i++)
		{

			if (m_children[i]->m_name == name)
			{
				found = true;
				index = i;
				break;
			}

			if (m_children[i]->getDescription().container)
			{
				found = m_children[i]->moveChildDown(name);
				if (found)
					return true;
			}

		}

		if (found)
		{
			if (index < m_children.size()-1)
			{
				auto upperitem = m_children[index];
				auto loweritem = m_children[index+1];

				m_children[index] = loweritem;
				m_children[index + 1] = upperitem;
			}

			return true;
		}

		return false;
	}

	bool mvAppItem::addRuntimeChild(const std::string& parent, const std::string& before, mvRef<mvAppItem> item)
	{
		if (before.empty() && parent.empty())
			return false;

		//this is the container, add item to beginning.
		if (before.empty())
		{
			if (parent == m_name)
			{
				m_children.push_back(item);
				item->m_parent = this;
				return true;
			}

			else
			{
				// check children
				for (auto& child : m_children)
				{
					if (child->getDescription().container)
					{
						// parent found
						if (child->addRuntimeChild(parent, before, item))
							return true;
					}
				}
			}
		}

		else
		{
			bool beforeFound = false;

			// check children
			for (auto& child : m_children)
			{

				if (child->m_name == before)
				{
					beforeFound = true;
					break;
				}

			}

			// after item is in this container
			if (beforeFound)
			{
				item->m_parent = this;

				std::vector<mvRef<mvAppItem>> oldchildren = m_children;
				m_children.clear();

				for (auto child : oldchildren)
				{
					if (child->m_name == before)
						m_children.push_back(item);
					m_children.push_back(child);

				}

				return true;
			}
		}

		// check children
		for (auto& child : m_children)
		{
			if (child->getDescription().container)
			{
				// parent found
				if (child->addRuntimeChild(parent, before, item))
					return true;
			}
		}

		return false;
	}

	bool mvAppItem::addChildAfter(const std::string& prev, mvRef<mvAppItem> item)
	{
		if (prev.empty())
			return false;


		bool prevFound = false;

		// check children
		for (auto& child : m_children)
		{

			if (child->m_name == prev)
			{
				item->m_parent = this;
				prevFound = true;
				break;
			}

		}

		// prev item is in this container
		if (prevFound)
		{
			//item->setParent(this);

			std::vector<mvRef<mvAppItem>> oldchildren = m_children;
			m_children.clear();

			for (auto& child : oldchildren)
			{
				m_children.push_back(child);
				if (child->m_name == prev)
					m_children.push_back(item);
			}

			return true;
		}
		

		// check children
		for (auto& child : m_children)
		{
			if (child->getDescription().container)
			{
				// parent found
				if (child->addChildAfter(prev, item))
					return true;
			}
		}

		return false;
	}

	bool mvAppItem::deleteChild(const std::string& name)
	{

		bool childfound = false;
		bool itemDeleted = false;

		for (auto& item : m_children)
		{
			if (item->m_name == name)
			{
				childfound = true;
				break;
			}

			if (item->getDescription().container)
			{
				itemDeleted = item->deleteChild(name);
				if (itemDeleted)
					break;
			}
		}

		if (childfound)
		{
			std::vector<mvRef<mvAppItem>> oldchildren = m_children;

			m_children.clear();

			for (auto& item : oldchildren)
			{
				if (item->m_name == name)
				{
					itemDeleted = true;
					continue;
				}

				m_children.push_back(item);
			}
		}

		return itemDeleted;
	}

	void mvAppItem::deleteChildren()
	{
		m_children.clear();
	}

	void mvAppItem::setLabel(const std::string& value)
	{
		m_label = value + "###" + m_name;
	}

	mvRef<mvAppItem> mvAppItem::stealChild(const std::string& name)
	{
		mvRef<mvAppItem> stolenChild = nullptr;

		bool childfound = false;

		for (auto& item : m_children)
		{
			if (item->m_name == name)
			{
				childfound = true;
				break;
			}

			if (item->getDescription().container)
			{
				stolenChild = item->stealChild(name);
				if (stolenChild)
					return stolenChild;
			}
		}

		if (childfound)
		{
			std::vector<mvRef<mvAppItem>> oldchildren = m_children;

			m_children.clear();

			for (auto& item : oldchildren)
			{
				if (item->m_name == name)
				{
					stolenChild = item;
					continue;
				}

				m_children.push_back(item);
			}
		}

		return stolenChild;
	}

	mvRef<mvAppItem> mvAppItem::getChild(const std::string& name)
	{
		for (auto& item : m_children)
		{
			if (item->m_name == name)
				return item;

			if (item->getDescription().container)
			{
				auto child = item->getChild(name);
				if (child)
					return child;
			}
		}

		return nullptr;
	}

	mvAppItem::~mvAppItem()
	{
		deleteChildren();

		mvGlobalIntepreterLock gil;

		if (m_callback)
			Py_DECREF(m_callback);
		if (m_callbackData)
			Py_DECREF(m_callbackData);
	}

	PyObject* mvAppItem::getCallback(bool ignore_enabled)
	{
		if (m_enabled)
			return m_callback;
		return ignore_enabled ? m_callback : nullptr;
	}

}
