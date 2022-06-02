#include "mvTabBar.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvContainers.h"
#include "mvPythonExceptions.h"

void mvTabBar::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvTabBar*>(item);
	_flags = titem->_flags;
}

mvUUID mvTabBar::getSpecificValue()
{
	return _uiValue;
}

void mvTabBar::setValue(mvUUID value)
{
	_uiValue = value;
}

PyObject* mvTabBar::getPyValue()
{
	return ToPyUUID(*_value);
}

void mvTabBar::setPyValue(PyObject* value)
{
	*_value = ToUUID(value);
}

void mvTabBar::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<mvUUID>*>(item->getValue());
}

void mvTabBar::draw(ImDrawList* drawlist, float x, float y)
{

    if (!config.show)
        return;
        
	ScopedID id(uuid);

	ImGui::BeginGroup();

	if (ImGui::BeginTabBar(info.internalLabel.c_str(), _flags))
	{

        state.lastFrameUpdate = GContext->frame;
        state.visible = true; // BeginTabBar(...) only reaches this if visible
            
		for (auto& item : childslots[1])
		{

			if (*_value == item->uuid && _lastValue != *_value)
				static_cast<mvTab*>(item.get())->configData._flags |= ImGuiTabItemFlags_SetSelected;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			if (*_value == item->uuid)
				static_cast<mvTab*>(item.get())->configData._flags &= ~ImGuiTabItemFlags_SetSelected;
		}

		ImGui::EndTabBar();
	}

	ImGui::EndGroup();

	*_value = _uiValue;
	_lastValue = *_value;
}

void mvTabBar::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;
		 

	// helper for bit flipping
	auto flagop = [dict](const char* keyword, int flag, int& flags)
	{
		if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
	};

	// window flags
	flagop("reorderable", ImGuiTabBarFlags_Reorderable, _flags);

}

void mvTabBar::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
		 

	// helper to check and set bit
	auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
	{
		PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
	};

	// window flags
	checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, _flags);
}