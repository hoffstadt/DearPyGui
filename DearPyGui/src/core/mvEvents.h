#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <variant>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Marvel {

	// type alias
	using mvID = size_t;
	using mvVariant = std::variant<int, std::string>; // more types can be added
	
	// hashing function
	mvID SID(const std::string& value);

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

		static void Publish  (mvEvent event);
		static void Subscribe(mvEventHandler* handler, mvID type, mvID category = 0);

	private:

		static std::unordered_map<mvID, std::vector<mvEventHandler*>>& GetEventHandlers();
		static std::unordered_map<mvID, std::vector<mvEventHandler*>>& GetEventCategoryHandlers();
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
			if (m_event.type == type && type != 0)
			{
				m_event.handled |= func(m_event);
				return true;
			}

			if (m_event.category == category && category != 0)
			{
				m_event.handled |= func(m_event);
				return true;
			}
			return false;
		}

	private:

		mvEvent& m_event;
	};
}