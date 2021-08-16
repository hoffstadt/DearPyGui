#include "mvDrawLine.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws a line on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("p1", mvArgType::REQUIRED_ARG, "...", "Start of line.");
		parser.addArg<mvPyDataType::FloatList>("p2", mvArgType::REQUIRED_ARG, "...", "End of line.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawLine::mvDrawLine(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawLine::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawLine*>(item);
		_p1 = titem->_p1;
		_p2 = titem->_p2;
		_color = titem->_color;
		_thickness = titem->_thickness;
	}

	void mvDrawLine::draw(ImDrawList* drawlist, float x, float y)
	{


		if(ImPlot::GetCurrentContext()->CurrentPlot)
			drawlist->AddLine(ImPlot::PlotToPixels(_p1), ImPlot::PlotToPixels(_p2), _color, 
				ImPlot::GetCurrentContext()->Mx * _thickness);
		else
		{
			ImVec2 start = { x, y };
			drawlist->AddLine(_p1 + start, _p2 + start, _color, _thickness);
		}

	}

	void mvDrawLine::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_p1 = ToVec2(item);
				break;

			case 1:
				_p2 = ToVec2(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawLine::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);

	}

	void mvDrawLine::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
		PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	}

}