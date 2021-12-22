#include "mvDrawText.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"

namespace Marvel {

	mvDrawText::mvDrawText(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawText::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawText*>(item);
		_pos = titem->_pos;
		_text = titem->_text;
		_color = titem->_color;
		_size = titem->_size;
	}

	void mvDrawText::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec4  tpos = drawInfo->transform * _pos;

		if (drawInfo->perspectiveDivide)
		{
			tpos.x = tpos.x / tpos.w;
			tpos.y = tpos.y / tpos.w;
			tpos.z = tpos.z / tpos.w;
		}

		if (drawInfo->depthClipping)
		{
			if (mvClipPoint(drawInfo->clipViewport, tpos)) return;
		}

		ImFont* fontptr = ImGui::GetFont();
		if (font)
			fontptr = static_cast<mvFont*>(font.get())->getFontPtr();

		if (ImPlot::GetCurrentContext()->CurrentPlot)
			drawlist->AddText(fontptr, ImPlot::GetCurrentContext()->Mx * (float)_size, ImPlot::PlotToPixels(tpos), _color, _text.c_str());
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddText(fontptr, (float)_size, tpos + start, _color, _text.c_str());
		}
	}

	void mvDrawText::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_pos = ToVec4(item);
				_pos.w = 1.0f;
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
		if (PyObject* item = PyDict_GetItemString(dict, "pos")) _pos = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);
		_pos.w = 1.0f;

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