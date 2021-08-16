#include "mvDrawText.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"

namespace Marvel {

	void mvDrawText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws a text on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("pos", mvArgType::REQUIRED_ARG, "...", "Top left point of bounding text rectangle.");
		parser.addArg<mvPyDataType::String>("text", mvArgType::REQUIRED_ARG, "...", "Text to draw.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");

		parser.addArg<mvPyDataType::Float>("size", mvArgType::KEYWORD_ARG, "10.0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawText::mvDrawText(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawText::draw(ImDrawList* drawlist, float x, float y)
	{
		ImFont* fontptr = ImGui::GetFont();
		if (_font)
			fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();

		if (ImPlot::GetCurrentContext()->CurrentPlot)
			drawlist->AddText(fontptr, ImPlot::GetCurrentContext()->Mx * (float)_size, ImPlot::PlotToPixels(_pos), _color, _text.c_str());
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddText(fontptr, (float)_size, _pos + start, _color, _text.c_str());
		}
	}

	void mvDrawText::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_pos = ToVec2(item);
				break;

			case 1:
				_text = ToString(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawText::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "text")) _text = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pos")) _pos = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);

	}

	void mvDrawText::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "text", mvPyObject(ToPyString(_text)));
		PyDict_SetItemString(dict, "pos", mvPyObject(ToPyPair(_pos.x, _pos.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "size", mvPyObject(ToPyFloat(_size)));
	}
}