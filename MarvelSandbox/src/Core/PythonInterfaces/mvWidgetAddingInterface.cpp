#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"

#include "Core/AppItems/mvAppItems.h"

namespace Marvel {

	PyObject* addSimplePlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		float height = 80.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::FloatList, "value"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Bool, "autoscale"),
			mvPythonDataElement(mvPythonDataType::String, "overlay"),
			mvPythonDataElement(mvPythonDataType::Float, "minscale"),
			mvPythonDataElement(mvPythonDataType::Float, "maxscale"),
			mvPythonDataElement(mvPythonDataType::Float, "height"),
			mvPythonDataElement(mvPythonDataType::Bool, "histogram")
			});

		if (!pl.parse(__FUNCTION__, &name, &value, &autoscale, &overlay, &minscale, &maxscale, &height, &histogram))
			return Py_None;

		std::vector<float> values = pl.getFloatVec(value);

		if (autoscale)
		{
			maxscale = values[0];
			minscale = values[0];

			for (auto& item : values)
			{
				if (item > maxscale)
					maxscale = item;
				if (item < minscale)
					minscale = item;
			}
		}

		mvAppItem* item = new mvSimplePlot("", name, values, overlay, minscale, maxscale, height, histogram);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(117.0));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(1.0));

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "wrap"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Bool, "bullet")

			});

		if (!pl.parse(__FUNCTION__, &name, &wrap, &color, &bullet))
			return Py_None;

		auto mcolor = pl.getColor(color);

		if (mcolor.r > 100.0f)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", std::string(name), wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addLabelText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name, * value;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(117.0));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(1.0));

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::String, "value"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "wrap"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Bool, "bullet")

			});

		if (!pl.parse(__FUNCTION__, &name, &value, &wrap, &color))
			return Py_None;

		auto mcolor = pl.getColor(color);

		if (mcolor.r > 100.0f)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addListbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* items;
		int default_value = 0, height = -1;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::StringList, "items"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value"),
			mvPythonDataElement(mvPythonDataType::Integer, "height"),
			});

		if (!pl.parse(__FUNCTION__, &name, &items, &default_value, &height))
			return Py_None;

		mvAppItem* item = new mvListbox("", name, pl.getStringVec(items), default_value, height);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCombo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name, * default_value = "";
		PyObject* items;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::StringList, "items"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::String, "default_value")
			});

		if (!pl.parse(__FUNCTION__, &name, &items, &default_value))
			return Py_None;

		mvAppItem* item = new mvCombo("", name, pl.getStringVec(items), default_value);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if (!pl.parse(__FUNCTION__, &name))
			return Py_None;

		mvAppItem* item = new mvButton("", name);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* hint = "";
		int multiline = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::String, "hint"),
			mvPythonDataElement(mvPythonDataType::Bool, "multiline")
			});

		if (!pl.parse(__FUNCTION__, &name, &hint, &multiline))
			return Py_None;

		mvAppItem* item = new mvInputText("", name, hint, multiline);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputInt(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value")
			});

		if (!pl.parse(__FUNCTION__, &name, &default_value))
			return Py_None;

		mvAppItem* item = new mvInputInt("", name, default_value);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "default_value")
			});

		if (!pl.parse(__FUNCTION__, &name, &default_value))
			return Py_None;

		mvAppItem* item = new mvInputFloat("", name, default_value);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float offset = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "offset")
			});

		if (!pl.parse(__FUNCTION__, &offset))
			return Py_None;

		mvAppItem* item = new mvIndent("", offset);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float offset = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "offset")
			});

		if (!pl.parse(__FUNCTION__, &offset))
			return Py_None;

		mvAppItem* item = new mvUnindent("", offset);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &name);

		mvAppItem* item = new mvTabBar("", name);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &name);

		mvAppItem* item = new mvTab("", name);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndTab("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* endTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndTabBar("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addMenuBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			});

		pl.parse(__FUNCTION__, &name);

		mvAppItem* item = new mvMenuBar(name);
		mvApp::GetApp()->addFlag(ImGuiWindowFlags_MenuBar);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &name);

		mvAppItem* item = new mvMenu("", name);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndMenu("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* endMenuBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndMenuBar("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addMenuItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if (!pl.parse(__FUNCTION__, &name))
			return Py_None;

		mvAppItem* item = new mvMenuItem("", name);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSpacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int count = 1;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "count")
			});

		if (!pl.parse(__FUNCTION__, &count))
			return Py_None;

		mvAppItem* item = new mvSpacing("", count);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSameLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float xoffset = 0.0f;
		float spacing = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "xoffset"),
			mvPythonDataElement(mvPythonDataType::Float, "spacing")
			});

		if (!pl.parse(__FUNCTION__, &xoffset, &spacing))
			return Py_None;

		mvAppItem* item = new mvSameLine("", xoffset, spacing);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addRadioButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::StringList, "items"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value")
			});

		if (!pl.parse(__FUNCTION__, &name, &items, &default_value))
			return Py_None;

		mvAppItem* item = new mvRadioButton("", name, pl.getStringVec(items), default_value);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &name);

		mvAppItem* item = new mvGroup("", name);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndGroup("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addChild(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(__FUNCTION__, &name, &width, &height);

		mvAppItem* item = new mvChild("", name, width, height);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(__FUNCTION__, &name, &width, &height);

		mvAppItem* item = new mvWindowAppitem("", name, width, height);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndWindowAppitem("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* endChild(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndChild("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addTooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &parent, &name);

		mvAppItem* item = new mvTooltip(parent, name);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndTooltip("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addPopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		int mousebutton = 1;
		int modal = false;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "mousebutton"),
			mvPythonDataElement(mvPythonDataType::Integer, "modal")
			});

		pl.parse(__FUNCTION__, &parent, &name, &mousebutton, &modal);

		mvAppItem* item = new mvPopup(parent, name, mousebutton, modal);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endPopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndPopup("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addCollapsingHeader(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &name);

		mvAppItem* item = new mvCollapsingHeader("", name);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endCollapsingHeader(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndCollapsingHeader("");
		mvApp::GetApp()->addEndParentItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addSeperator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvSeparator("");
		mvApp::GetApp()->addItem(item);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addColorEdit4(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (!pl.parse(__FUNCTION__, &name, &r, &g, &b, &a))
			return Py_None;

		mvAppItem* item = new mvColorEdit4("", name, r, g, b, a);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCheckbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value")
			});

		if (!pl.parse(__FUNCTION__, &name, &default_value))
			return Py_None;

		mvAppItem* item = new mvCheckbox("", name, default_value);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreateWidgetAddingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addPopup, "Not Documented");
		pyModule.addMethod(endPopup, "Not Documented");
		pyModule.addMethod(addWindow, "Not Documented");
		pyModule.addMethod(endWindow, "Not Documented");
		pyModule.addMethod(indent, "Not Documented");
		pyModule.addMethod(unindent, "Not Documented");
		pyModule.addMethod(addSimplePlot, "Not Documented");
		pyModule.addMethod(addCombo, "Not Documented");
		pyModule.addMethod(addText, "Not Documented");
		pyModule.addMethod(addLabelText, "Not Documented");
		pyModule.addMethod(addListbox, "Not Documented");
		pyModule.addMethod(addColorEdit4, "Not Documented");
		pyModule.addMethod(addSeperator, "Not Documented");
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
		pyModule.addMethod(addCollapsingHeader, "Not Documented");
		pyModule.addMethod(endCollapsingHeader, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}