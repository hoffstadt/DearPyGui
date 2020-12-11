#pragma once
#include <string>
#include <vector>
#include <stack>
#include <deque>
#include <unordered_map>
#include <functional>
#include <variant>
#include "mvCompileTimeCRC32.h"
#include "mvEventMacros.h"

#define BIND_EVENT_METH(x) std::bind(&x, this, std::placeholders::_1)

namespace Marvel {

	// forward declarations
	struct mvEvent;

	// type alias
	using mvID = size_t;
	using mvVariant = std::variant<
		int, 
		std::string, 
		bool, 
		float, 
		void*
	>; // more types can be added
	
	// hashing function
	constexpr const mvID SID(const char* value)
	{
		return crc32_rec(0, value);
	}


	//-----------------------------------------------------------------------------
	// variant helpers
	//-----------------------------------------------------------------------------
	const std::string& GetEString(mvEvent& event, const char* name);
	bool               GetEBool  (mvEvent& event, const char* name);
	int                GetEInt   (mvEvent& event, const char* name);
	float              GetEFloat (mvEvent& event, const char* name);

	//-----------------------------------------------------------------------------
	// mvEvent
	//-----------------------------------------------------------------------------
	struct mvEvent
	{

		mvID type;
		std::unordered_map<mvID, mvVariant> arguments;
		mvID category = 0;
		bool handled = false;
	};

	// variant helpers
	template<typename T>
	T GetEPtr(mvEvent& event, const char* name)
	{
		return static_cast<T>(std::get<void*>(event.arguments.at(SID(name))));
	}

	//-----------------------------------------------------------------------------
	// event argument creation helpers
	//-----------------------------------------------------------------------------
	template<typename T>
	std::pair<mvID, T> CreateEventArgument(const char* name, T value)
	{
		return std::make_pair(SID(name), value);
	}

	template<typename T>
	std::pair<mvID, void*> CreateEventPtrArgument(const char* name, T value)
	{
		return std::make_pair(SID(name), static_cast<void*>(value));
	}

	//-----------------------------------------------------------------------------
	// mvEventHandler
	//-----------------------------------------------------------------------------
	class mvEventHandler
	{

	public:

		virtual bool onEvent(mvEvent& event) = 0;

	};

	//-----------------------------------------------------------------------------
	// mvEventBus
	//-----------------------------------------------------------------------------
	class mvEventBus
	{

	public:

		static void PublishEndFrame(mvID category, mvID type, std::unordered_map<mvID, mvVariant> arguments = {});
		static void Publish        (mvID category, mvID type, std::unordered_map<mvID, mvVariant> arguments = {});
		static void Subscribe      (mvEventHandler* handler, mvID type = 0, mvID category = 0);
		static void UnSubscribe    (mvEventHandler* handler);

		// event bus events
		static bool OnEvent(mvEvent& event);
		static bool OnFrame(mvEvent& event);

	private:

		static void                                                    Publish(mvEvent event);
		static std::stack<mvEvent>&                                    GetEndFrameEvents();
		static std::unordered_map<mvID, std::vector<mvEventHandler*>>& GetEventHandlers();
		static std::unordered_map<mvID, std::vector<mvEventHandler*>>& GetEventCategoryHandlers();

		static std::deque<std::string> GetMessages();
	};

	//-----------------------------------------------------------------------------
	// mvEventDispatcher
	//-----------------------------------------------------------------------------
	class mvEventDispatcher
	{

	public:

		mvEventDispatcher(mvEvent& event)
			: m_event(event)
		{
		}

		template<typename F>
		bool dispatch(const F& func, mvID type = 0, mvID category = 0)
		{

			if (m_event.handled)
				return true;

			if (m_event.type == type && type != 0)
			{
				m_event.handled = func(m_event);
				return true;
			}

			if (m_event.category == category && category != 0)
			{
				m_event.handled = func(m_event);
				return true;
			}
			return false;
		}

	private:

		mvEvent& m_event;

	};
}

