#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvAppInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();

	PyObject* isItemHovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemHovered"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemHovered(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemActive(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemActive"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemActive(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemFocused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemFocused"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemFocused(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemClicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemClicked"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemClicked(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemVisible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemVisible"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemVisible(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemEdited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemEdited"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemEdited(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemActivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemActivated"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemActivated(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemDeactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemDeactivated"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemDeactivated(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemDeactivatedAfterEdit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemDeactivatedAfterEdit"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemDeactivatedAfterEdit(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemToggledOpen(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["isItemToggledOpen"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		bool value = mvApp::GetApp()->isItemToogledOpen(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* getItemRectMin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["getItemRectMin"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		mvVec2 value = mvApp::GetApp()->getItemRectMin(std::string(item));

		PyObject* pvalue = PyTuple_New(2);
		PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
		PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));

		return pvalue;
	}

	PyObject* getItemRectMax(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["getItemRectMax"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		mvVec2 value = mvApp::GetApp()->getItemRectMax(std::string(item));

		PyObject* pvalue = PyTuple_New(2);
		PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
		PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));

		return pvalue;
	}

	PyObject* getItemRectSize(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["getItemRectSize"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		mvVec2 value = mvApp::GetApp()->getItemRectSize(std::string(item));

		PyObject* pvalue = PyTuple_New(2);
		PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
		PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));

		return pvalue;
	}

	PyObject* changeStyleItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		float x = 0.0f;
		float y = 0.0f;

		if (!Translators["changeStyleItem"].parse(args, kwargs,__FUNCTION__, &item, &x, &y))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeStyleItem(item, x, y);

		Py_RETURN_NONE;
	}

	PyObject* changeThemeItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		float r, g, b, a;

		if(!Translators["changeThemeItem"].parse(args, kwargs,__FUNCTION__, &item, &r, &g, &b, &a))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeThemeItem(item, { r, g, b, a });

		

		Py_RETURN_NONE;
	}

	PyObject* getValue(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Translators["getValue"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if(item == nullptr)
			Py_RETURN_NONE;

		return item->getPyValue();
	}

	PyObject* setValue(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if(!Translators["setValue"].parse(args, kwargs,__FUNCTION__, &name, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			Py_RETURN_NONE;

		item->setPyValue(value);
		Py_RETURN_NONE;
	}

	PyObject* showItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Translators["showItem"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		Py_RETURN_NONE;
	}

	PyObject* hideItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Translators["hideItem"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		Py_RETURN_NONE;
	}

	PyObject* setMainCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;


		if(!Translators["setMainCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMainCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setMouseDownCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["setMouseDownCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDownCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setMouseDoubleClickCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["setMouseDoubleClickCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDoubleClickCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setMouseClickCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["setMouseClickCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseClickCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setKeyDownCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["setKeyDownCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyDownCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setKeyPressCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["setKeyPressCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyPressCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setKeyReleaseCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["setKeyReleaseCallback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyReleaseCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* setItemCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback, * item;
		
		if(!Translators["setItemCallback"].parse(args, kwargs,__FUNCTION__, &item, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setItemCallback(item, callback);

		

		Py_RETURN_NONE;
	}

	PyObject* setItemPopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup, * item;

		if (!Translators["setItemPopup"].parse(args, kwargs,__FUNCTION__, &item, &popup))
			Py_RETURN_NONE;

		mvApp::GetApp()->setItemPopup(std::string(item), std::string(popup));

		

		Py_RETURN_NONE;
	}

	PyObject* setItemTip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if(!Translators["setItemTip"].parse(args, kwargs,__FUNCTION__, &item, &tip))
			Py_RETURN_NONE;

		mvApp::GetApp()->setItemTip(std::string(item), std::string(tip));

		

		Py_RETURN_NONE;
	}

	PyObject* setItemWidth(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		if(!Translators["setItemWidth"].parse(args, kwargs,__FUNCTION__, &item, &width))
			Py_RETURN_NONE;

		mvApp::GetApp()->setItemWidth(std::string(item), width);

		

		Py_RETURN_NONE;
	}

	PyObject* closePopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->closePopup();
		
		Py_RETURN_NONE;
	}

	PyObject* setTheme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		if(!Translators["setTheme"].parse(args, kwargs,__FUNCTION__, &theme))
			Py_RETURN_NONE;

		mvApp::GetApp()->setAppTheme(std::string(theme));

		

		Py_RETURN_NONE;
	}

	PyObject* showMetrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showMetrics();

		

		Py_RETURN_NONE;
	}

	PyObject* showAbout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showAbout();

		

		Py_RETURN_NONE;
	}

	void CreatePythonInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(showAbout, "Not Documented");
		pyModule.addMethod(showMetrics, "Not Documented");

		pyModule.addMethodD(setItemPopup);
		pyModule.addMethodD(closePopup);
		pyModule.addMethodD(isItemHovered);
		pyModule.addMethodD(isItemActive);
		pyModule.addMethodD(isItemFocused);
		pyModule.addMethodD(isItemClicked);
		pyModule.addMethodD(isItemVisible);
		pyModule.addMethodD(isItemEdited);
		pyModule.addMethodD(isItemActivated);
		pyModule.addMethodD(isItemDeactivated);
		pyModule.addMethodD(isItemDeactivatedAfterEdit);
		pyModule.addMethodD(isItemToggledOpen);
		pyModule.addMethodD(getItemRectMin);
		pyModule.addMethodD(getItemRectMax);
		pyModule.addMethodD(getItemRectSize);
		pyModule.addMethodD(setMouseClickCallback);
		pyModule.addMethodD(setMouseDownCallback);
		pyModule.addMethodD(setMouseDoubleClickCallback);
		pyModule.addMethodD(setKeyDownCallback);
		pyModule.addMethodD(setKeyPressCallback);
		pyModule.addMethodD(setKeyReleaseCallback);
		pyModule.addMethodD(changeStyleItem);
		pyModule.addMethodD(showItem);
		pyModule.addMethodD(hideItem);
		pyModule.addMethodD(changeThemeItem);
		pyModule.addMethodD(setTheme);
		pyModule.addMethodD(setMainCallback);
		pyModule.addMethodD(setItemCallback);
		pyModule.addMethodD(setItemTip);
		pyModule.addMethodD(setItemWidth);
		pyModule.addMethodD(getValue);
		pyModule.addMethodD(setValue);

		PyImport_AppendInittab(pyModule.getName() , initfunc);
	}
}