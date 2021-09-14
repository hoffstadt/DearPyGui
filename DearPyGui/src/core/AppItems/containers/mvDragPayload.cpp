#include "mvDragPayload.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDragPayload::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::Object, "drag_data", mvArgType::KEYWORD_ARG, "None", "Drag data" });
		args.push_back({ mvPyDataType::String, "payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'" });

		mvPythonParserSetup setup;
		setup.about = "User data payload for drag and drop operations.";
		setup.category = { "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
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
		_payloadType = titem->_payloadType;
	}

	void mvDragPayload::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload(_payloadType.c_str(), this, sizeof(mvDragPayload));

			if (_parentPtr->_dragCallback)
			{
				if(_parentPtr->_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().addCallback(_parentPtr->_dragCallback, _parent, GetPyNone(), _user_data);
				else
					mvApp::GetApp()->getCallbackRegistry().addCallback(_parentPtr->_dragCallback, _parentPtr->_alias, GetPyNone(), _user_data);
			}

			for (auto& childset : _children)
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
	}

	void mvDragPayload::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}