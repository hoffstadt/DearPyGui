#include "mvCppPyObject.h"
#include "mvApp.h"

PyContextManager::~PyContextManager()
{
	if (m_moved)
	{
		std::lock_guard<std::mutex> lk(Marvel::mvApp::GetApp()->GetApp()->getMutex());
		Marvel::mvApp::GetApp()->getItemRegistry().popParent();
	}
}