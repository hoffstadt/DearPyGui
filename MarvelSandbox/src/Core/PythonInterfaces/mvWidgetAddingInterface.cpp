#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvWidgetAddingInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();

	PyObject* addSimplePlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		float height = 80.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;

		if (!Translators["addSimplePlot"].parse(args, kwargs,__FUNCTION__, &name, &value, &autoscale, &overlay, &minscale, &maxscale, &height, &histogram, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		std::vector<float> values = mvPythonTranslator::getFloatVec(value);

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

		MV_STANDARD_CALLBACK_EVAL();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(117.0));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(1.0));

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "wrap"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Bool, "bullet")

			}, true);

		if (!Translators["addText"].parse(args, kwargs,__FUNCTION__, &name, &wrap, &color, &bullet, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 100.0f)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", std::string(name), wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addLabelText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name, * value;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(117.0));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(1.0));

		if (!Translators["addLabelText"].parse(args, kwargs,__FUNCTION__, &name, &value, &wrap, &color, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 100.0f)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addListbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0, height = -1;

		if (!Translators["addListbox"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, &height, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::getStringVec(items), default_value, height);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCombo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name, * default_value = "";
		PyObject* items;

		if (!Translators["addCombo"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvCombo("", name, mvPythonTranslator::getStringVec(items), default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSelectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = false;

		if (!Translators["addSelectable"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvSelectable("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;

		if (!Translators["addButton"].parse(args, kwargs,__FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvButton("", name);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		MV_STANDARD_CALLBACK_EVAL();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		const char* hint = "";
		int multiline = 0;

		if (!Translators["addInputText"].parse(args, kwargs,__FUNCTION__, &name, &hint, &multiline, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvInputText("", name, hint, multiline);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputInt(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = 0;

		if (!Translators["addInputInt"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvInputInt("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		float default_value = 0.0f;

		if (!Translators["addInputFloat"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvInputFloat("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float offset = 0.0f;

		if (!Translators["indent"].parse(args, kwargs,__FUNCTION__, &offset))
			return Py_None;

		mvAppItem* item = new mvIndent("", offset);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float offset = 0.0f;

		if (!Translators["unindent"].parse(args, kwargs,__FUNCTION__, &offset))
			return Py_None;

		mvAppItem* item = new mvUnindent("", offset);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addTabBar"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvTabBar("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addTab"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvTab("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
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
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addMenuBar"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvMenuBar(name);
		mvApp::GetApp()->addFlag(ImGuiWindowFlags_MenuBar);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addMenu"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvMenu("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
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
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addMenuItem"].parse(args, kwargs,__FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvMenuItem("", name);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSpacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int count = 1;

		if (!Translators["addSpacing"].parse(args, kwargs,__FUNCTION__, &count))
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

		if (!Translators["addSameLine"].parse(args, kwargs,__FUNCTION__, &xoffset, &spacing))
			return Py_None;

		mvAppItem* item = new mvSameLine("", xoffset, spacing);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addRadioButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0;

		if (!Translators["addRadioButton"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvRadioButton("", name, mvPythonTranslator::getStringVec(items), default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addGroup"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvGroup("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
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

		if (!Translators["addChild"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			return Py_None;

		mvAppItem* item = new mvChild("", name, width, height);
		mvApp::GetApp()->addParentItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		if (!Translators["addWindow"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			return Py_None;

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
		MV_STANDARD_CALLBACK_INIT();
		const char* parent, * name;

		if (!Translators["addTooltip"].parse(args, kwargs, __FUNCTION__, &parent, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvTooltip(parent, name);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		MV_STANDARD_CALLBACK_EVAL();
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
		MV_STANDARD_CALLBACK_INIT();
		const char* parent, * name;
		int mousebutton = 1;
		int modal = false;

		if (!Translators["addPopup"].parse(args, kwargs, __FUNCTION__, &parent, &name, &mousebutton, &modal, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvPopup(parent, name, mousebutton, modal);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		MV_STANDARD_CALLBACK_EVAL();
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
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addCollapsingHeader"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvCollapsingHeader("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
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

	PyObject* addTreeNode(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addTreeNode"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvTreeNode("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTreeNode(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndTreeNode("");
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
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		float r, g, b, a;

		if (!Translators["addColorEdit4"].parse(args, kwargs,__FUNCTION__, &name, &r, &g, &b, &a, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvColorEdit4("", name, r, g, b, a);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCheckbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = 0;

		if (!Translators["addCheckbox"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			return Py_None;

		mvAppItem* item = new mvCheckbox("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreateWidgetAddingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(endTreeNode, "Not Documented");
		pyModule.addMethod(endPopup, "Not Documented");
		pyModule.addMethod(endWindow, "Not Documented");
		pyModule.addMethod(endGroup, "Not Documented");
		pyModule.addMethod(endChild, "Not Documented");
		pyModule.addMethod(endTab, "Not Documented");
		pyModule.addMethod(endTabBar, "Not Documented");
		pyModule.addMethod(endMenu, "Not Documented");
		pyModule.addMethod(endMenuBar, "Not Documented");
		pyModule.addMethod(endTooltip, "Not Documented");
		pyModule.addMethod(endCollapsingHeader, "Not Documented");

		pyModule.addMethodD(addTreeNode);
		pyModule.addMethodD(addSelectable);
		pyModule.addMethodD(addPopup);
		pyModule.addMethodD(addWindow);
		pyModule.addMethodD(indent);
		pyModule.addMethodD(unindent);
		pyModule.addMethodD(addSimplePlot);
		pyModule.addMethodD(addCombo);
		pyModule.addMethodD(addText);
		pyModule.addMethodD(addLabelText);
		pyModule.addMethodD(addListbox);
		pyModule.addMethodD(addColorEdit4);
		pyModule.addMethodD(addSeperator);
		pyModule.addMethodD(addButton);
		pyModule.addMethodD(addInputText);
		pyModule.addMethodD(addInputInt);
		pyModule.addMethodD(addInputFloat);
		pyModule.addMethodD(addRadioButton);
		pyModule.addMethodD(addCheckbox);
		pyModule.addMethodD(addGroup);
		pyModule.addMethodD(addChild);
		pyModule.addMethodD(addTabBar);
		pyModule.addMethodD(addTab);
		pyModule.addMethodD(addMenuBar);
		pyModule.addMethodD(addMenu);
		pyModule.addMethodD(addMenuItem);
		pyModule.addMethodD(addSpacing);
		pyModule.addMethodD(addSameLine);
		pyModule.addMethodD(addTooltip);
		pyModule.addMethodD(addCollapsingHeader);
		

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}