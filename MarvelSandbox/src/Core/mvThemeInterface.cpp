#include "mvThemeInterface.h"

#include "mvApp.h"

namespace Marvel {

	PyObject* getValue(PyObject* self, PyObject* args)
	{
		const char* name;

		PyArg_ParseTuple(args, "s", &name);

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return Py_None;

		return item->getPyValue();
	}

	void CreateThemeInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addColorEdit4, "Not Documented");
		pyModule.addMethod(addSeperator, "Not Documented");
		pyModule.addMethod(setTheme, "Not Documented");
		pyModule.addMethod(setMainCallback, "Not Documented");
		pyModule.addMethod(setItemCallback, "Not Documented");
		pyModule.addMethod(setItemTip, "Not Documented");
		pyModule.addMethod(setItemWidth, "Not Documented");
		pyModule.addMethod(addButton, "Not Documented");
		pyModule.addMethod(addInputText, "Not Documented");
		pyModule.addMethod(addInputInt, "Not Documented");
		pyModule.addMethod(addInputFloat, "Not Documented");
		pyModule.addMethod(addRadioButton, "Not Documented");
		pyModule.addMethod(addCheckbox, "Not Documented");
		pyModule.addMethod(addGroup, "Not Documented");
		pyModule.addMethod(endGroup, "Not Documented");
		pyModule.addMethod(addChild, "Not Documented");
		pyModule.addMethod(endChild, "Not Documented");
		pyModule.addMethod(addTabBar, "Not Documented");
		pyModule.addMethod(addTab, "Not Documented");
		pyModule.addMethod(endTab, "Not Documented");
		pyModule.addMethod(endTabBar, "Not Documented");
		pyModule.addMethod(addMenuBar, "Not Documented");
		pyModule.addMethod(addMenu, "Not Documented");
		pyModule.addMethod(endMenu, "Not Documented");
		pyModule.addMethod(endMenuBar, "Not Documented");
		pyModule.addMethod(addMenuItem, "Not Documented");
		pyModule.addMethod(addSpacing, "Not Documented");
		pyModule.addMethod(addSameLine, "Not Documented");
		pyModule.addMethod(addTooltip, "Not Documented");
		pyModule.addMethod(endTooltip, "Not Documented");
		pyModule.addMethod(getValue, "Not Documented");
		pyModule.addMethod(addCollapsingHeader, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}