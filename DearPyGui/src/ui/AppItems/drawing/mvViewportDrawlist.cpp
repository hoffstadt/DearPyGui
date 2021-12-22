#include "mvViewportDrawlist.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvViewport.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvViewportDrawlist::mvViewportDrawlist(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvViewportDrawlist::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvViewportDrawlist*>(item);
		_front = titem->_front;
	}

	void mvViewportDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{

		ImDrawList* internal_drawlist = _front ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList();

		for (auto& item : childslots[2])
		{
			// skip item if it's not shown
			if (!item->config.show)
				continue;

			item->draw(internal_drawlist, 0.0f, 0.0f);

			UpdateAppItemState(item->state);
		}

	}

	void mvViewportDrawlist::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "front")) _front = ToBool(item);

	}

	void mvViewportDrawlist::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "front", mvPyObject(ToPyBool(_front)));
	}

}
