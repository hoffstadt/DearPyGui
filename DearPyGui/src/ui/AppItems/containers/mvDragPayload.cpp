#include "mvDragPayload.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void 
	apply_drag_drop(mvAppItem* item)
	{
		for (auto& item :item-> childslots[3])
			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		if (item->config.dropCallback)
		{
			ScopedID id(item->uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item->config.payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if (item->config.alias.empty())
						mvAddCallback(item->config.dropCallback, item->uuid, payloadActual->getDragData(), nullptr);
					else
						mvAddCallback(item->config.dropCallback, item->config.alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void
	apply_drag_drop_nodraw(mvAppItem* item)
	{
		if (item->config.dropCallback)
		{
			ScopedID id(item->uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item->config.payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if (item->config.alias.empty())
						mvAddCallback(item->config.dropCallback, item->uuid, payloadActual->getDragData(), nullptr);
					else
						mvAddCallback(item->config.dropCallback, item->config.alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	mvDragPayload::mvDragPayload(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDragPayload::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDragPayload*>(item);
		if (titem->_dragData)
		{
			Py_XINCREF(titem->_dragData);
			_dragData = titem->_dragData;
		}
		if (titem->_dropData)
		{
			Py_XINCREF(titem->_dropData);
			_dropData = titem->_dropData;
		}
		_payloadType = titem->_payloadType;
	}

	void mvDragPayload::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload(_payloadType.c_str(), this, sizeof(mvDragPayload));

			if (info.parentPtr->config.dragCallback)
			{
				if(info.parentPtr->config.alias.empty())
					mvAddCallback(info.parentPtr->config.dragCallback, config.parent, _dragData, config.user_data);
				else
					mvAddCallback(info.parentPtr->config.dragCallback, info.parentPtr->config.alias, _dragData, config.user_data);
			}

			for (auto& childset : childslots)
			{
				for (auto& item : childset)
					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			}

			ImGui::EndDragDropSource();
		}
	}

	void mvDragPayload::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "payload_type")) _payloadType = ToString(item);

		if (PyObject* item = PyDict_GetItemString(dict, "drag_data"))
		{
			if (_dragData)
				Py_XDECREF(_dragData);

			Py_XINCREF(item);
			_dragData = item;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "drop_data"))
		{
			if (_dropData)
				Py_XDECREF(_dropData);

			Py_XINCREF(item);
			_dropData = item;
		}
	}

	void mvDragPayload::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}