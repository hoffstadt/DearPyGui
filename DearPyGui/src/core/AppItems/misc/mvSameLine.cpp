#include "mvSameLine.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvSameLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Float>("xoffset", mvArgType::KEYWORD, "0.0", "offset from containing window");
		parser.addArg<mvPyDataType::Float>("spacing", mvArgType::KEYWORD, "-1.0", "offset from previous widget");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvSameLine::mvSameLine(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvSameLine::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::SameLine(m_xoffset, m_spacing);
	}

	void mvSameLine::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) m_xoffset = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "spacing")) m_spacing = ToFloat(item);

	}

	void mvSameLine::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "xoffset", ToPyFloat(m_xoffset));
		PyDict_SetItemString(dict, "spacing", ToPyFloat(m_spacing));
	}

}