#include "mvWindow.h"
#include <string>
#include <array>

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool error) :
		m_error(error), m_width(width), m_height(height)
	{

		m_frontDrawList = CreateRef<mvDrawList>();
		m_backDrawList = CreateRef<mvDrawList>();

		mvEventBus::Subscribe(this, mvEVT_RENDER);

		m_app = mvApp::GetApp();

		if (m_error)
		{
			mvAppLog::ShowMain();
			mvAppLog::setSize(width, height);
		}

	}

	bool mvWindow::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(mvWindow::onRender), mvEVT_RENDER);

		return event.handled;
	}

	bool mvWindow::onRender(mvEvent& event)
	{
		m_frontDrawList->draw(ImGui::GetForegroundDrawList(), 0.0f, 0.0f);
		m_backDrawList->draw(ImGui::GetBackgroundDrawList(), 0.0f, 0.0f);

		return true;
	}

}