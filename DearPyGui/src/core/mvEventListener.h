#pragma once

#include <deque>
#include "mvEvents.h"
#include <imgui.h>

#define MV_EL_MESSAGE(CODE) addMessage("%0s : %1d\n", #CODE, CODE)

namespace Marvel {

	class mvEventListener : public mvEventHandler
	{

	public:

		mvEventListener()
		{
			mvEventBus::Subscribe(this);
			LineOffsets.push_back(0);
		}

		bool onEvent(mvEvent& event) override
		{
			mvEventDispatcher dispatcher(event);
			dispatcher.dispatch(BIND_EVENT_METH(mvEventListener::onRender), mvEVT_RENDER);
			dispatcher.dispatch(BIND_EVENT_METH(mvEventListener::onInput), 0, mvEVT_CATEGORY_INPUT);
			dispatcher.dispatch(BIND_EVENT_METH(mvEventListener::onResize), mvEVT_VIEWPORT_RESIZE);

			return false;
		}

		bool onRender(mvEvent& event)
		{
			int i = 0;
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();

			for (int line_no = 0; line_no < LineOffsets.size(); line_no++)
			{

				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;

				ImGui::GetForegroundDrawList()->AddText(ImVec2(10, i), IM_COL32(0, 255, 255, 255), line_start, line_end);
				i += 15;

			}


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
				MV_EL_MESSAGE(mvEVT_MOUSE_MOVE);
				break;

			default:
				return false;
			}
			return false;
		}


	private:

		void addMessage(const char* fmt, ...)
		{

			static std::mutex mutex;

			std::lock_guard<std::mutex> lock(mutex);

			if (LineOffsets.size() > 25)
			{
				Buf.clear();
				LineOffsets.clear();
				LineOffsets.push_back(0);
			}

			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size + 1);

		}

		//std::deque<std::string> m_messages;
		ImGuiTextBuffer  Buf;
		ImVector<int>    LineOffsets;

	};

}
