#include "mvDrawArrow.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawArrow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::FloatList, "p1", mvArgType::REQUIRED_ARG, "...", "Arrow tip." });
		args.push_back({ mvPyDataType::FloatList, "p2", mvArgType::REQUIRED_ARG, "...", "Arrow tail." });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
		args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4" });

		mvPythonParserSetup setup;
		setup.about = "Adds an arrow.";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDrawArrow::mvDrawArrow(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		updatePoints();
	}

	void mvDrawArrow::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawArrow*>(item);
		_p1 = titem->_p1;
		_p2 = titem->_p2;
		_color = titem->_color;
		_thickness = titem->_thickness;
		_size = titem->_size;
		_points[0] = titem->_points[0];
		_points[1] = titem->_points[1];
		_points[2] = titem->_points[2];
	}

	void mvDrawArrow::updatePoints()
	{
		float xsi = _p1.x;
		float xfi = _p2.x;
		float ysi = _p1.y;
		float yfi = _p2.y;

		// length of arrow head
		double xoffset = _size;
		double yoffset = _size;

		// get pointer angle w.r.t +X (in radians)
		double angle = 0.0;
		if (xsi >= xfi && ysi >= yfi) {
			angle = atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi < xfi && ysi >= yfi) {
			angle = M_PI + atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi < xfi && ysi < yfi) {
			angle = -M_PI + atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi >= xfi && ysi < yfi) {
			angle = atan((ysi - yfi) / (xsi - xfi));
		}

		// arrow head points
		auto x1 = (float)(xsi - xoffset * cos(angle));
		auto y1 = (float)(ysi - yoffset * sin(angle));

		_points[0] = { xsi, ysi, 0.0f, 1.0f };
		_points[1] = { (float)(x1 - 0.5 * _size * sin(angle)), (float)(y1 + 0.5 * _size * cos(angle)), 0.0f, 1.0f };
		_points[2] = { (float)(x1 + 0.5 * _size * cos((M_PI / 2.0) - angle)), (float)(y1 - 0.5 * _size * sin((M_PI / 2.0) - angle)), 0.0f, 1.0f };

	}

	void mvDrawArrow::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec4  tp1 = _p1;
		mvVec4  tp2 = _p2;

		mvVec4  tpp1 = _points[0];
		mvVec4  tpp2 = _points[1];
		mvVec4  tpp3 = _points[2];

		if (!_transformIsIdentity)
		{
			tp1 = _transform * _p1;
			tp2 = _transform * _p2;
			tpp1 = _transform * _points[0];
			tpp2 = _transform * _points[1];
			tpp3 = _transform * _points[2];
		}

		if (_perspectiveDivide)
		{
			tp1.x = tp1.x / tp1.w;
			tp2.x = tp2.x / tp2.w;
			tpp1.x = tpp1.x / tpp1.w;
			tpp2.x = tpp2.x / tpp2.w;
			tpp3.x = tpp3.x / tpp3.w;

			tp1.y = tp1.y / tp1.w;
			tp2.y = tp2.y / tp2.w;
			tpp1.y = tpp1.y / tpp1.w;
			tpp2.y = tpp2.y / tpp2.w;
			tpp3.y = tpp3.y / tpp3.w;

			tp1.z = tp1.z / tp1.w;
			tp2.z = tp2.z / tp2.w;
			tpp1.z = tpp1.z / tpp1.w;
			tpp2.z = tpp2.z / tpp2.w;
			tpp3.z = tpp3.z / tpp3.w;
		}

		if (_depthClipping)
		{
			if (mvClipPoint(_clipViewport, tp1)) return;
			if (mvClipPoint(_clipViewport, tp2)) return;
			if (mvClipPoint(_clipViewport, tpp1)) return;
			if (mvClipPoint(_clipViewport, tpp2)) return;
			if (mvClipPoint(_clipViewport, tpp3)) return;
		}
		
		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddLine(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp1), _color, (float)ImPlot::GetCurrentContext()->Mx*_thickness);
			drawlist->AddTriangle(ImPlot::PlotToPixels(tpp1), ImPlot::PlotToPixels(tpp2), ImPlot::PlotToPixels(tpp3), _color, (float)ImPlot::GetCurrentContext()->Mx*_thickness);
			drawlist->AddTriangleFilled(ImPlot::PlotToPixels(tpp1), ImPlot::PlotToPixels(tpp2), ImPlot::PlotToPixels(tpp3), _color);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddLine(tp1 + start, tp2 + start, _color, _thickness);
			drawlist->AddTriangle(tpp1 + start, tpp2 + start, tpp3 + start, _color, _thickness);
			drawlist->AddTriangleFilled(tpp1 + start, tpp2 + start, tpp3 + start, _color);
		}
	}

	void mvDrawArrow::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_p1 = ToVec4(item);
				_p2.w = 1.0f;
				break;

			case 1:
				_p2 = ToVec4(item);
				_p2.w = 1.0f;
				break;

			default:
				break;
			}
		}

		updatePoints();
	}

	void mvDrawArrow::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);

		_p1.w = 1.0f;
		_p2.w = 1.0f;

		updatePoints();

	}

	void mvDrawArrow::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
		PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
		PyDict_SetItemString(dict, "size", mvPyObject(ToPyFloat(_size)));
	}

}