#include "mvInterfaceCore.h"
#include "mvApp.h"
#include "Core/mvPythonExceptions.h"

namespace Marvel {

	bool AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before)
	{

		if (item == nullptr)
			return false;

		auto ma = mvApp::GetApp();

		// remove bad parent stack item
		if (item->getType() == mvAppItemType::Window && ma->topParent() != nullptr)
		{
			if (ma->topParent()->getName() != "MainWindow")
			{
				ma->popParent();
				ThrowPythonException("Adding window will remove last item in parent stack. Don't forget to end container types.");
			}
		}

		// window runtime adding
		if (item->getType() == mvAppItemType::Window && mvApp::IsAppStarted())
			return ma->addRuntimeItem("", "", item);

		// window compile adding
		else if (item->getType() == mvAppItemType::Window)
			return ma->addWindow(item);

		// typical run time adding
		else if ((!std::string(parent).empty() || !std::string(before).empty()) && mvApp::IsAppStarted())
			return ma->addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted() && ma->topParent() != nullptr)
			return ma->addRuntimeItem(ma->topParent()->getName(), before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted())
			return ma->addRuntimeItem("MainWindow", "", item);

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !mvApp::IsAppStarted())
			return ma->addRuntimeItem(parent, before, item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty())
			return ma->addItem(item);

		return false;
	}

}