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
		config.width = titem->config.width;
		config.height = titem->config.height;
	}

	void mvDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!config.show)
			return;

		_startx = (float)ImGui::GetCursorScreenPos().x;
		_starty = (float)ImGui::GetCursorScreenPos().y;

		ImDrawList* internal_drawlist = ImGui::GetWindowDrawList();

		ImGui::PushClipRect({ _startx, _starty }, { _startx + (float)config.width, _starty + (float)config.height }, true);

		for (auto& item : childslots[2])
		{
			// skip item if it's not shown
			if (!item->config.show)
				continue;

			item->draw(internal_drawlist, _startx, _starty);

			UpdateAppItemState(item->state);
		}

		ImGui::PopClipRect();

		if (ImGui::InvisibleButton(info.internalLabel.c_str(), ImVec2((float)config.width, (float)config.height), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle))
		{
			if(config.alias.empty())
				mvAddCallback(getCallback(false), uuid, nullptr, config.user_data);
			else
				mvAddCallback(getCallback(false), config.alias, nullptr, config.user_data);
		}

		UpdateAppItemState(state);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			GContext->input.mouseDrawingPos.x = (int)(mousepos.x - _startx);
			GContext->input.mouseDrawingPos.y = (int)(mousepos.y - _starty);
		}
	}

	void mvDrawlist::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				config.width = ToInt(item);
				break;

			case 1:
				config.height = ToInt(item);
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

		PyDict_SetItemString(dict, "width", mvPyObject(ToPyInt(config.width)));
		PyDict_SetItemString(dict, "height", mvPyObject(ToPyInt(config.height)));
	}

}
