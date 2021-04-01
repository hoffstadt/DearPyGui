#include "mvWindow.h"
#include <string>
#include <array>

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool error) :
		m_error(error), m_width(width), m_height(height)
	{

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
		return true;
	}

}