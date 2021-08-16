#include "mvDragPayload.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDragPayload::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "User data payload for drag and drop operations.", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Object>("drag_data", mvArgType::KEYWORD_ARG, "None", "Drag data");
		parser.addArg<mvPyDataType::String>("payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'");

		parser.finalize();

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

			if(_parentPtr->getDragCallback())
				mvApp::GetApp()->getCallbackRegistry().addCallback(_parentPtr->getDragCallback(), _parent, GetPyNone(), _user_data);

			for (auto& childset : _children)
			{
				for (auto& item : childset)
				{

					if (!item->preDraw())
						continue;

					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

					item->postDraw();
				}
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