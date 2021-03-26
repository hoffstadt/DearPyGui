#pragma once

#include <deque>
#include <imgui.h>
#include "mvEvents.h"
#include "mvLog.h"

#define MV_ELT_MESSAGE(CODE) MV_EVENT_TRACE("{} : {}", #CODE, CODE)
#define MV_EL_MESSAGE(CODE) MV_EVENT_TRACE("{} : {}", #CODE, CODE)

namespace Marvel {

	class mvEventListener : public mvEventHandler
	{

	public:

		mvEventListener()
		{
			mvEventBus::Subscribe(this);
		}

		bool onEvent(mvEvent& event) override
		{
			mvEventDispatcher dispatcher(event);
			dispatcher.dispatch(BIND_EVENT_METH(mvEventListener::onInput), 0, mvEVT_CATEGORY_INPUT);
			dispatcher.dispatch(BIND_EVENT_METH(mvEventListener::onResize), mvEVT_VIEWPORT_RESIZE);

			return false;
		}

		bool onResize(mvEvent& event)
		{
			MV_EL_MESSAGE(mvEVT_VIEWPORT_RESIZE);
			return false;
		}

		bool onInput(mvEvent& event)
		{
			switch (event.type)
			{
			case mvEVT_KEY_PRESS:
				MV_EL_MESSAGE(mvEVT_KEY_PRESS);
				break;

			case mvEVT_KEY_DOWN:
				MV_EL_MESSAGE(mvEVT_KEY_DOWN);
				break;

			case mvEVT_KEY_RELEASE:
				MV_EL_MESSAGE(mvEVT_KEY_RELEASE);
				break;

			case mvEVT_MOUSE_WHEEL:
				MV_EL_MESSAGE(mvEVT_MOUSE_WHEEL);
				break;

			case mvEVT_MOUSE_DRAG:
				MV_EL_MESSAGE(mvEVT_MOUSE_DRAG);
				break;

			case mvEVT_MOUSE_CLICK:
				MV_EL_MESSAGE(mvEVT_MOUSE_CLICK);
				break;

			case mvEVT_MOUSE_DOWN:
				MV_EL_MESSAGE(mvEVT_MOUSE_DOWN);
				break;

			case mvEVT_MOUSE_DBL_CLK:
				MV_EL_MESSAGE(mvEVT_MOUSE_DBL_CLK);
				break;

			case mvEVT_MOUSE_RELEASE:
				MV_EL_MESSAGE(mvEVT_MOUSE_RELEASE);
				break;

			case mvEVT_MOUSE_MOVE:
				MV_ELT_MESSAGE(mvEVT_MOUSE_MOVE);
				break;

			default:
				return false;
			}
			return false;
		}

	};

}
