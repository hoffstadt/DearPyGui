#include "mvDrawlist.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvViewport.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvDrawlist::mvDrawlist(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDrawlist::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawlist*>(item);
		_width = titem->_width;
		_height = titem->_height;
	}

	void mvDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!_show)
			return;

		_startx = (float)ImGui::GetCursorScreenPos().x;
		_starty = (float)ImGui::GetCursorScreenPos().y;

		ImDrawList* internal_drawlist = ImGui::GetWindowDrawList();

		ImGui::PushClipRect({ _startx, _starty }, { _startx + (float)_width, _starty + (float)_height }, true);

		for (auto& item : _children[2])
		{
			// skip item if it's not shown
			if (!item->_show)
				continue;

			item->draw(internal_drawlist, _startx, _starty);

			UpdateAppItemState(item->_state);
		}

		ImGui::PopClipRect();

		if (ImGui::InvisibleButton(_internalLabel.c_str(), ImVec2((float)_width, (float)_height), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle))
		{
			if(_alias.empty())
				mvAddCallback(getCallback(false), _uuid, nullptr, _user_data);
			else
				mvAddCallback(getCallback(false), _alias, nullptr, _user_data);
		}

		UpdateAppItemState(_state);

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			GContext->input.mouseDrawingPos.x = (int)(mousepos.x - _startx);
			GContext->input.mouseDrawingPos.y = (int)(mousepos.y - _starty);
		}
	}

	void mvDrawlist::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(_type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_width = ToInt(item);
				break;

			case 1:
				_height = ToInt(item);
				break;

			default:
				break;
			}
		}

	}

	void mvDrawlist::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "width", mvPyObject(ToPyInt(_width)));
		PyDict_SetItemString(dict, "height", mvPyObject(ToPyInt(_height)));
	}

}
