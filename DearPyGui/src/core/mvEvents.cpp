#include "mvEvents.h"

namespace Marvel {

	mvID SID(const std::string& value)
	{
		return std::hash<std::string>{}(value);
	}

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

	bool mvEventBus::OnEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(mvEventBus::OnFrame, SID("FRAME"));

		return false;
	}

	bool mvEventBus::OnFrame(mvEvent& event)
	{
		while (!GetEndFrameEvents().empty())
		{
			Publish(GetEndFrameEvents().top());
			GetEndFrameEvents().pop();
		}
		
		return false;
	}

	void mvEventBus::PublishEndFrame(const char* category, const char* type, std::unordered_map<mvID, mvVariant> arguments)
	{
		GetEndFrameEvents().push({ SID(type), arguments, SID(category) });
	}

	void mvEventBus::Publish(const char* category, const char* type, std::unordered_map<mvID, mvVariant> arguments)
	{
		Publish({ SID(type), arguments, SID(category) });
	}

	void mvEventBus::Publish(mvEvent event)
		{
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

	std::stack<mvEvent>& mvEventBus::GetEndFrameEvents()
	{
		static std::stack<mvEvent> events;
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

}