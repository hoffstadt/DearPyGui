#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"

#include "Core/AppItems/mvAppItems.h"

namespace Marvel {


	PyObject* isItemHovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemHovered(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemActive(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemActive(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemFocused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemFocused(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemClicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemClicked(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemVisible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemVisible(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemEdited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemEdited(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemActivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemActivated(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemDeactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemDeactivated(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemDeactivatedAfterEdit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemDeactivatedAfterEdit(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* isItemToogledOpen(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		bool value = mvApp::GetApp()->isItemToogledOpen(std::string(item));

		PyObject* pvalue = Py_BuildValue("i", value);

		return pvalue;
	}

	PyObject* getItemRectMin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		mvVec2 value = mvApp::GetApp()->getItemRectMin(std::string(item));

		PyObject* pvalue = PyTuple_New(2);
		PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
		PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));

		return pvalue;
	}

	PyObject* getItemRectMax(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		mvVec2 value = mvApp::GetApp()->getItemRectMax(std::string(item));

		PyObject* pvalue = PyTuple_New(2);
		PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
		PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));

		return pvalue;
	}

	PyObject* getItemRectSize(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item")
			});

		if (!pl.parse(__FUNCTION__, &item))
			return Py_None;

		mvVec2 value = mvApp::GetApp()->getItemRectSize(std::string(item));

		PyObject* pvalue = PyTuple_New(2);
		PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
		PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));

		return pvalue;
	}

	PyObject* setStyleItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		float x = 0.0f;
		float y = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item"),
			mvPythonDataElement(mvPythonDataType::Float, "x"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "x"),

			});

		if (!pl.parse(__FUNCTION__, &item, &x, &y))
			return Py_False;

		bool check = mvApp::GetApp()->setStyleItem(std::string(item), x, y);

		if (check)
			return Py_True;

		return Py_False;
	}

	PyObject* updateStyle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->updateStyle();
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* changeThemeItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float r, g, b, a;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Float, "r"),
			mvPythonDataElement(mvPythonDataType::Float, "g"),
			mvPythonDataElement(mvPythonDataType::Float, "b"),
			mvPythonDataElement(mvPythonDataType::Float, "a")
			});

		if(!pl.parse(__FUNCTION__, &name, &r, &g, &b, &a))
			return Py_None;

		mvApp::GetApp()->changeThemeItem(name, { r, g, b, a });

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* updateTheme(PyObject* self, PyObject* args)
	{

		mvApp::GetApp()->updateTheme();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* getValue(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if(!pl.parse(__FUNCTION__, &name))
			return Py_None;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if(item == nullptr)
			return Py_None;

		return item->getPyValue();
	}

	PyObject* setValue(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Object, "value")
			});

		if(!pl.parse(__FUNCTION__, &name, &value))
			return Py_None;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return Py_None;

		item->setPyValue(value);
		return Py_None;
	}

	PyObject* showItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if(!pl.parse(__FUNCTION__, &name))
			return Py_None;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		return Py_None;
	}

	PyObject* hideItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if(!pl.parse(__FUNCTION__, &name))
			return Py_None;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		return Py_None;
	}

	PyObject* setMainCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if(!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setMainCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setMouseDownCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if (!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setMouseDownCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setMouseDoubleClickCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if (!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setMouseDoubleClickCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setMouseClickCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if (!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setMouseClickCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setKeyDownCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if (!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setKeyDownCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setKeyPressCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if (!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setKeyPressCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setKeyReleaseCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});

		if (!pl.parse(__FUNCTION__, &callback))
			return Py_None;

		mvApp::GetApp()->setKeyReleaseCallback(std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setItemCallback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback, * item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item"),
			mvPythonDataElement(mvPythonDataType::String, "callback")
			});
		
		if(!pl.parse(__FUNCTION__, &item, &callback))
			return Py_None;

		mvApp::GetApp()->setItemCallback(std::string(item), std::string(callback));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setItemPopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup, * item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item"),
			mvPythonDataElement(mvPythonDataType::String, "popup")
			});

		if (!pl.parse(__FUNCTION__, &item, &popup))
			return Py_None;

		mvApp::GetApp()->setItemPopup(std::string(item), std::string(popup));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setItemTip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item"),
			mvPythonDataElement(mvPythonDataType::String, "tip")
			});

		if(!pl.parse(__FUNCTION__, &item, &tip))
			return Py_None;

		mvApp::GetApp()->setItemTip(std::string(item), std::string(tip));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setItemWidth(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "item"),
			mvPythonDataElement(mvPythonDataType::Integer, "width")
			});

		if(!pl.parse(__FUNCTION__, &item, &width))
			return Py_None;

		mvApp::GetApp()->setItemWidth(std::string(item), width);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* closePopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->closePopup();
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* setTheme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "theme")
			});

		if(!pl.parse(__FUNCTION__, &theme))
			return Py_None;

		mvApp::GetApp()->setAppTheme(std::string(theme));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* showMetrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showMetrics();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* showAbout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showAbout();

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreatePythonInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(showAbout, "Not Documented");
		pyModule.addMethod(showMetrics, "Not Documented");
		pyModule.addMethod(setItemPopup, "Not Documented");
		pyModule.addMethod(closePopup, "Not Documented");
		pyModule.addMethod(isItemHovered, "Not Documented");
		pyModule.addMethod(isItemActive, "Not Documented");
		pyModule.addMethod(isItemFocused, "Not Documented");
		pyModule.addMethod(isItemClicked, "Not Documented");
		pyModule.addMethod(isItemVisible, "Not Documented");
		pyModule.addMethod(isItemEdited, "Not Documented");
		pyModule.addMethod(isItemActivated, "Not Documented");
		pyModule.addMethod(isItemDeactivated, "Not Documented");
		pyModule.addMethod(isItemDeactivatedAfterEdit, "Not Documented");
		pyModule.addMethod(isItemToogledOpen, "Not Documented");
		pyModule.addMethod(getItemRectMin, "Not Documented");
		pyModule.addMethod(getItemRectMax, "Not Documented");
		pyModule.addMethod(getItemRectSize, "Not Documented");
		pyModule.addMethod(setMouseClickCallback, "Not Documented");
		pyModule.addMethod(setMouseDownCallback, "Not Documented");
		pyModule.addMethod(setMouseDoubleClickCallback, "Not Documented");
		pyModule.addMethod(setKeyDownCallback, "Not Documented");
		pyModule.addMethod(setKeyPressCallback, "Not Documented");
		pyModule.addMethod(setKeyReleaseCallback, "Not Documented");
		pyModule.addMethod(setStyleItem, "Not Documented");
		pyModule.addMethod(updateStyle, "Not Documented");
		pyModule.addMethod(showItem, "Not Documented");
		pyModule.addMethod(hideItem, "Not Documented");
		pyModule.addMethod(changeThemeItem, "Not Documented");
		pyModule.addMethod(updateTheme, "Not Documented");
		pyModule.addMethod(setTheme, "Not Documented");
		pyModule.addMethod(setMainCallback, "Not Documented");
		pyModule.addMethod(setItemCallback, "Not Documented");
		pyModule.addMethod(setItemTip, "Not Documented");
		pyModule.addMethod(setItemWidth, "Not Documented");
		pyModule.addMethod(getValue, "Not Documented");
		pyModule.addMethod(setValue, "Not Documented");

		PyImport_AppendInittab(pyModule.getName() , initfunc);
	}
}