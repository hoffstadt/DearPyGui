#include <utility>
#include "mvButton.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {
	void mvButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text.", "False"},
			{mvPythonDataType::Bool, "arrow", "Arrow button, must use with direction", "False"},
			{mvPythonDataType::Integer, "direction", "A cardinal direction", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a button.", "None", "Adding Widgets") });
	}

	mvButton::mvButton(const std::string& name)
		: mvAppItem(name)
	{
		m_description.disableAllowed = true;
	}

	void mvButton::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		int libIDCount = 0;
		int pushedIDs[ImGuiCol_COUNT];
		//this goes through the specific colors for the current item type and applies them
		for (auto& themeColor : getIndividualTheme().getColors()[getType()])
		{
			ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
			pushedIDs[libIDCount] = themeColor.first;
			libIDCount++;
		}
		//this goes through the specific colors for the current item type and applies them
		mvAppItem* widget = this;
		while (!widget->getDescription().root)
		{
			widget = widget->getParent();
			for (auto& themeColor : widget->getIndividualTheme().getColors()[getType()])
			{
				//checking if libID has been used
				int i = 0;
				while (i < libIDCount)
					if (pushedIDs[i] == themeColor.first)
						break;
					else 
						i++;
				//adds libID if it has not been found
				if (i == libIDCount)
				{
					ImGui::PushStyleColor((ImGuiCol)themeColor.first, themeColor.second.toVec4());
					pushedIDs[libIDCount] = themeColor.first;
					libIDCount++;
				}
			}
		}


		if (!m_enabled)
		{
			auto disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ButtonHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ButtonActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
		}

		if (m_small)
		{
			if (ImGui::SmallButton(m_label.c_str()))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

			//remember to pop
			ImGui::PopStyleColor(libIDCount);
			return;
		}

		if (m_arrow)
		{
			if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

			//remember to pop
			ImGui::PopStyleColor(libIDCount);
			return;
		}

		if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

		//remember to pop
		ImGui::PopStyleColor(libIDCount);
	}

	void mvButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "small")) m_small = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "arrow")) m_arrow = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "direction")) m_direction = ToInt(item);
	}

	void mvButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "small", ToPyBool(m_small));
		PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
		PyDict_SetItemString(dict, "direction", ToPyInt(m_direction));
	}


	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int smallb = false;
		int arrow = false;
		int direction = 2;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* label = "";
		int show = true;
		int enabled = true;

		if (!(*mvApp::GetApp()->getParsers())["add_button"].parse(args, kwargs, __FUNCTION__, &name, &smallb,
			&arrow, &direction, &callback, &callback_data, &tip, &parent, &before, &width, &height,
			&label, &show, &enabled))
			return GetPyNone();

		auto item = CreateRef<mvButton>(name);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}


}