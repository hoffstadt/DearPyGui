#include "mvInterfaceCore.h"
#include "mvApp.h"
#include "Core/mvPythonExceptions.h"

namespace Marvel {

	bool AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before)
	{

		if (item == nullptr)
			return false;

		//auto ma = mvApp::GetApp();
		mvItemRegistry& itemRegistry = mvApp::GetApp()->getItemRegistry();

		// remove bad parent stack item
		if (item->getDescription().isARoot() && itemRegistry.topParent() != nullptr)
		{
			itemRegistry.emptyParents();
			ThrowPythonException("Parent stack not empty. Adding window will empty the parent stack. Don't forget to end container types.");
		}

		if (item->getType() == mvAppItemType::Popup || item->getType() == mvAppItemType::Tooltip)
			return itemRegistry.addItemAfter(parent, item);

		// window runtime adding
		if (item->getDescription().isARoot() && mvApp::IsAppStarted())
			return itemRegistry.addRuntimeItem("", "", item);

		// window compile adding
		else if (item->getDescription().isARoot())
			return itemRegistry.addWindow(item);

		// typical run time adding
		else if ((!std::string(parent).empty() || !std::string(before).empty()) && mvApp::IsAppStarted())
			return itemRegistry.addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted() && itemRegistry.topParent() != nullptr)
			return itemRegistry.addRuntimeItem(itemRegistry.topParent()->getName(), before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted())
		{
			ThrowPythonException("Parent stack is empty. You must specify 'before' or 'parent' widget.");
			delete item;
			return false;
		}

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !mvApp::IsAppStarted())
			return itemRegistry.addRuntimeItem(parent, before, item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty())
			return itemRegistry.addItem(item);

		return false;
	}

}