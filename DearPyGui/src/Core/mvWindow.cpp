#include "mvWindow.h"

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool editor, bool error, bool doc) :
		m_width(width), m_height(height), m_editor(editor), m_error(error), m_doc(doc)
	{
		m_app = mvApp::GetAppStandardWindow();
		m_appEditor = new mvAppEditor();
		m_documentation = mvDocWindow::GetWindow();

		if (m_error)
		{
			mvAppLog::ShowMain();
			mvAppLog::setSize(width, height);
		}

		else if (m_doc)
		{
			m_documentation->setToMainMode();
			m_documentation->setSize(width, height);
		}
	}

	mvWindow::~mvWindow()
	{
		delete m_appEditor;
		m_appEditor = nullptr;
	}

}