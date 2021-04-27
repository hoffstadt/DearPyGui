#include "mvText.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::POSITIONAL_ARG, "''");

		parser.addArg<mvPyDataType::Integer>("wrap", mvArgType::KEYWORD_ARG, "-1", "number of characters until wraping");

		parser.addArg<mvPyDataType::Bool>("bullet", mvArgType::KEYWORD_ARG, "False", "makes the text bulleted");

		parser.addArg<mvPyDataType::FloatList>("color", mvArgType::KEYWORD_ARG, "(-1, -1, -1, -1)", "color of the text (rgba)");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	void mvLabelText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::FloatList>("color", mvArgType::KEYWORD_ARG, "(-1, -1, -1, -1)", "color of the text (rgba)");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvText::mvText(const std::string& name)
		: 
		mvStringPtrBase(name)
	{
		*m_value = name;
	}

	void mvText::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_color.r > 0.0f)
			ImGui::PushStyleColor(ImGuiCol_Text, m_color.toVec4());

		if (m_wrap >= 0)
			ImGui::PushTextWrapPos((float)m_wrap);

		if (m_bullet)
			ImGui::Bullet();

		//ImGui::Text("%s", m_value.c_str());
		ImGui::TextUnformatted(m_value->c_str()); // this doesn't have a buffer size limit

		if (m_wrap >= 0)
			ImGui::PopTextWrapPos();

		if (m_color.r > 0.0f)
			ImGui::PopStyleColor();

	}

	mvLabelText::mvLabelText(const std::string& name)
		: 
		mvStringPtrBase(name)
	{
	}

	void mvLabelText::draw(ImDrawList* drawlist, float x, float y)
	{

		if (m_color.r > 0.0f)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, m_color.toVec4());

			ImGui::TextUnformatted(m_value->c_str());

			ImGui::PopStyleColor();

			ImGui::SameLine();

			mvImGuiThemeScope scope(this);
			ImGui::TextUnformatted(m_specificedlabel.c_str());
		}

		else
		{
			mvImGuiThemeScope scope(this);
			ImGui::LabelText(m_specificedlabel.c_str(), m_value->c_str());
		}

	}

	void mvText::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				*m_value = ToString(item);
				break;

			default:
				break;
			}
		}
	}

	void mvText::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "wrap")) m_wrap = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bullet")) m_bullet = ToBool(item);

	}

	void mvText::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "wrap", ToPyInt(m_wrap));
		PyDict_SetItemString(dict, "bullet", ToPyBool(m_bullet));
	}

	void mvLabelText::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
	}

	void mvLabelText::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
	}

}