#include "mvText.h"
#include "mvApp.h"

namespace Marvel {

	void mvText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "wrap", "number of characters until wraping", "-1"},
			{mvPythonDataType::FloatList, "color", "color of the text (rgba)", "(0, 0, 0, -1)"},
			{mvPythonDataType::Bool, "bullet", "makes the text bulleted", "False"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::String, "default_value", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds text", "None", "Adding Widgets") });
	}

	void mvLabelText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_label_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value", "", "''"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "data source for shared data", "''"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds text with a label. Useful for output values.", "None", "Adding Widgets") });
	}

	static std::string FindRenderedTextEnd(const char* text, const char* text_end = nullptr)
	{
		int size = 0;

		const char* text_display_end = text;
		if (!text_end)
			text_end = (const char*)-1;

		while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		{
			text_display_end++;
			size++;
		}

		char* cvalue = new char[size + 1];
		for (int i = 0; i < size; i++)
			cvalue[i] = text[i];

		cvalue[size] = '\0';

		return std::string(cvalue);
	}

	mvText::mvText(const std::string& name, const std::string& default_value, const std::string& dataSource)
		: 
		mvStringPtrBase(name, default_value.empty() ? name : default_value, dataSource)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvText::draw()
	{

		if (m_color.specified)
			ImGui::PushStyleColor(ImGuiCol_Text, m_color);

		if (m_wrap >= 0)
			ImGui::PushTextWrapPos((float)m_wrap);

		if (m_bullet)
			ImGui::Bullet();

		//ImGui::Text("%s", m_value.c_str());
		ImGui::TextUnformatted(m_value->c_str()); // this doesn't have a buffer size limit

		if (m_wrap >= 0)
			ImGui::PopTextWrapPos();

		if (m_color.specified)
			ImGui::PopStyleColor();

	}

	void mvText::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "wrap")) m_wrap = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bullet")) m_bullet = ToBool(item);

	}

	void mvText::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "wrap", ToPyInt(m_wrap));
		PyDict_SetItemString(dict, "bullet", ToPyBool(m_bullet));
	}

	mvLabelText::mvLabelText(const std::string& name, const std::string& value, const std::string& dataSource)
		: 
		mvStringPtrBase(name, value, dataSource)
	{
		m_label = FindRenderedTextEnd(m_name.c_str());
	}

	void mvLabelText::draw()
	{

		if (m_color.specified)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, m_color);

			ImGui::TextUnformatted(m_value->c_str());

			ImGui::PopStyleColor();

			ImGui::SameLine();

			auto styleManager = m_styleManager.getScopedStyleManager();
			mvImGuiThemeScope scope(this);
			ImGui::TextUnformatted(m_label.c_str());
		}

		else
		{
			mvImGuiThemeScope scope(this);
			ImGui::LabelText(m_label.c_str(), m_value->c_str());
		}

	}

	void mvLabelText::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
	}

	void mvLabelText::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
	}

	PyObject* add_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int wrap = -1;
		int bullet = false;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* before = "";
		const char* parent = "";
		int show = true;
		const char* source = "";
		const char* default_value = "";


		if (!(*mvApp::GetApp()->getParsers())["add_text"].parse(args, kwargs, __FUNCTION__, &name, &wrap,
			&color, &bullet, &parent, &before, &source, &default_value, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvText>(name, default_value, source);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* value = "";
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		int show = true;


		if (!(*mvApp::GetApp()->getParsers())["add_label_text"].parse(args, kwargs, __FUNCTION__, &name, &value,
			&color, &parent, &before, &source, &label, &show))
			return ToPyBool(false);


		auto item = CreateRef<mvLabelText>(std::string(name), value, source);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}
}