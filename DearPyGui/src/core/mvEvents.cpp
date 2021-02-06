#include "mvEvents.h"
#include "imgui.h"
#include <utility>
#include "mvProfiler.h"

namespace Marvel {

	const std::string& GetEString(mvEvent& event, const char* name)
	{
		return std::get<std::string>(event.arguments.at(SID(name)));
	}

	bool GetEBool(mvEvent& event, const char* name)
	{
		return std::get<bool>(event.arguments.at(SID(name)));
	}

	int GetEInt(mvEvent& event, const char* name)
	{
		return std::get<int>(event.arguments.at(SID(name)));
	}

	long GetELong(mvEvent& event, const char* name)
	{
		return std::get<long>(event.arguments.at(SID(name)));
	}

	float GetEFloat(mvEvent& event, const char* name)
	{
		return std::get<float>(event.arguments.at(SID(name)));
	}

	mvColor GetEColor(mvEvent& event, const char* name)
	{
		return std::get<mvColor>(event.arguments.at(SID(name)));
	}

	void mvEventBus::ShowDebug()
	{
		if (ImGui::Begin("Event Bus"))
		{
			std::unordered_map<mvID, std::vector<mvEventHandler*>>& handlerGroups = mvEventBus::GetEventHandlers();

			for (auto& group : handlerGroups)
			{
				if (ImGui::CollapsingHeader(std::to_string(group.first).c_str()))
				{
					for (auto handler : group.second)
					{
						ImGui::Text("%d", handler);
					}
					
				}

			}
		}
		ImGui::End();
	}

	bool mvEventBus::OnEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(mvEventBus::OnFrame, SID("END_FRAME"));

		return false;
	}

	bool mvEventBus::OnFrame(mvEvent& event)
	{
		while (!GetEndFrameEvents().empty())
		{
			Publish(GetEndFrameEvents().front());
			GetEndFrameEvents().pop();
		}
		
		return false;
	}

	void mvEventBus::PublishEndFrame(mvID category, mvID type, std::unordered_map<mvID, mvVariant> arguments)
	{
		GetEndFrameEvents().push({ type, std::move(arguments), category });
	}

	void mvEventBus::Publish(mvID category, mvID type, std::unordered_map<mvID, mvVariant> arguments)
	{
		Publish({ type, std::move(arguments), category });
	}

	void mvEventBus::Publish(mvEvent event)
	{

		for (mvEventHandler* handler : GetEventHandlers()[0])
			handler->onEvent(event);

		if (event.category == SID("GLOBAL"))
			OnEvent(event);

		if (event.type == 0)
			return;

		if (GetEventHandlers().find(event.type) != GetEventHandlers().end())
		{
			for (mvEventHandler* handler : GetEventHandlers()[event.type])
			{
				if (handler->onEvent(event))
					break;
			}
		}

		if (event.category == 0)
			return;

		if (GetEventCategoryHandlers().find(event.category) != GetEventCategoryHandlers().end())
		{
			for (mvEventHandler* handler : GetEventCategoryHandlers()[event.category])
			{
				if (handler->onEvent(event))
					break;
			}
		}

	}

	void mvEventBus::Subscribe(mvEventHandler* handler, mvID type, mvID category)
	{
		if (type == 0 && category == 0)
		{
			if (GetEventHandlers().find(0) == GetEventHandlers().end())
				GetEventHandlers()[0] = { handler };
			else
				GetEventHandlers()[0].push_back(handler);

			if (GetEventCategoryHandlers().find(0) == GetEventCategoryHandlers().end())
				GetEventCategoryHandlers()[0] = { handler };
			else
				GetEventCategoryHandlers()[0].push_back(handler);
			return;
		}

		if (type != 0)
		{
			if (GetEventHandlers().find(type) == GetEventHandlers().end())
				GetEventHandlers()[type] = { handler };
			else
				GetEventHandlers()[type].push_back(handler);
		}

		if (category == 0)
			return;

		if (GetEventCategoryHandlers().find(category) == GetEventCategoryHandlers().end())
			GetEventCategoryHandlers()[category] = { handler };
		else
			GetEventCategoryHandlers()[category].push_back(handler);
	}

	void mvEventBus::UnSubscribe(mvEventHandler* handler)
	{

		for (auto& eventHandlerBucket : GetEventHandlers())
		{
			std::vector<mvEventHandler*> old = eventHandlerBucket.second;
			eventHandlerBucket.second.clear();

			for (mvEventHandler* eventHandler : old)
			{
				if (eventHandler != handler)
					eventHandlerBucket.second.push_back(eventHandler);
			}
		}

		for (auto& eventHandlerBucket : GetEventCategoryHandlers())
		{
			std::vector<mvEventHandler*> old = eventHandlerBucket.second;
			eventHandlerBucket.second.clear();

			for (mvEventHandler* eventHandler : old)
			{
				if (eventHandler != handler)
					eventHandlerBucket.second.push_back(eventHandler);
			}
		}

	}

	void mvEventBus::Reset()
	{
		GetEventHandlers().clear();
		GetEventCategoryHandlers().clear();
		while (!GetEndFrameEvents().empty())
			GetEndFrameEvents().pop();
	}

	std::queue<mvEvent>& mvEventBus::GetEndFrameEvents()
	{
		static std::queue<mvEvent> events;
		return events;
	}

	std::unordered_map<mvID, std::vector<mvEventHandler*>>& mvEventBus::GetEventHandlers()
	{
		static std::unordered_map<mvID, std::vector<mvEventHandler*>> eventHandlers;
		return eventHandlers;
	}

	std::unordered_map<mvID, std::vector<mvEventHandler*>>& mvEventBus::GetEventCategoryHandlers()
	{
		static std::unordered_map<mvID, std::vector<mvEventHandler*>> eventCategoryHandlers;
		return eventCategoryHandlers;
	}

	std::deque<std::string> mvEventBus::GetMessages()
	{
		static std::deque<std::string> messages;
		return messages;
	}
}