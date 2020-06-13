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
			Py_RETURN_NONE;

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

		

		Py_RETURN_NONE;
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
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 100.0f)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", std::string(name), wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		

		Py_RETURN_NONE;
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
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 100.0f)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		

		Py_RETURN_NONE;
	}

	PyObject* addListbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0, height = -1;

		if (!Translators["addListbox"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, &height, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::getStringVec(items), default_value, height);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		

		Py_RETURN_NONE;
	}

	PyObject* addCombo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name, * default_value = "";
		PyObject* items;

		if (!Translators["addCombo"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCombo("", name, mvPythonTranslator::getStringVec(items), default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addSelectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = false;

		if (!Translators["addSelectable"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSelectable("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;

		if (!Translators["addButton"].parse(args, kwargs,__FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvButton("", name);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		MV_STANDARD_CALLBACK_EVAL();

		

		Py_RETURN_NONE;
	}

	PyObject* addInputText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		const char* hint = "";
		int multiline = 0;

		if (!Translators["addInputText"].parse(args, kwargs,__FUNCTION__, &name, &hint, &multiline, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputText("", name, hint, multiline);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addInputInt(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = 0;

		if (!Translators["addInputInt"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputInt("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addInputFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		float default_value = 0.0f;

		if (!Translators["addInputFloat"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputFloat("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float offset = 0.0f;

		if (!Translators["indent"].parse(args, kwargs,__FUNCTION__, &offset))
			Py_RETURN_NONE;

		mvAppItem* item = new mvIndent("", offset);
		mvApp::GetApp()->addItem(item);

		

		Py_RETURN_NONE;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float offset = 0.0f;

		if (!Translators["unindent"].parse(args, kwargs,__FUNCTION__, &offset))
			Py_RETURN_NONE;

		mvAppItem* item = new mvUnindent("", offset);
		mvApp::GetApp()->addItem(item);

		

		Py_RETURN_NONE;
	}

	PyObject* addTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addTabBar"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTabBar("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addTab"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTab("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* endTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* addMenuBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addMenuBar"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvMenuBar(name);
		mvApp::GetApp()->addFlag(ImGuiWindowFlags_MenuBar);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addMenu"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvMenu("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* endMenuBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* addMenuItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addMenuItem"].parse(args, kwargs,__FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvMenuItem("", name);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addSpacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int count = 1;

		if (!Translators["addSpacing"].parse(args, kwargs,__FUNCTION__, &count))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSpacing("", count);
		mvApp::GetApp()->addItem(item);

		

		Py_RETURN_NONE;
	}

	PyObject* addSameLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float xoffset = 0.0f;
		float spacing = 0.0f;

		if (!Translators["addSameLine"].parse(args, kwargs,__FUNCTION__, &xoffset, &spacing))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSameLine("", xoffset, spacing);
		mvApp::GetApp()->addItem(item);

		

		Py_RETURN_NONE;
	}

	PyObject* addRadioButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0;

		if (!Translators["addRadioButton"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvRadioButton("", name, mvPythonTranslator::getStringVec(items), default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		
		Py_RETURN_NONE;
	}

	PyObject* addGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addGroup"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvGroup("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* addChild(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		if (!Translators["addChild"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvChild("", name, width, height);
		mvApp::GetApp()->addParentItem(item);

		

		Py_RETURN_NONE;
	}

	PyObject* addWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		if (!Translators["addWindow"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvWindowAppitem("", name, width, height);
		mvApp::GetApp()->addParentItem(item);

		

		Py_RETURN_NONE;
	}

	PyObject* endWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndWindowAppitem("");
		mvApp::GetApp()->addEndParentItem(item);
		
		Py_RETURN_NONE;
	}

	PyObject* endChild(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parent = mvApp::GetApp()->popParent();		
		Py_RETURN_NONE;
	}

	PyObject* addTooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* parent, * name;

		if (!Translators["addTooltip"].parse(args, kwargs, __FUNCTION__, &parent, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTooltip(parent, name);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endTooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndTooltip("");
		mvApp::GetApp()->addEndParentItem(item);
		
		Py_RETURN_NONE;
	}

	PyObject* addPopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* parent, * name;
		int mousebutton = 1;
		int modal = false;

		if (!Translators["addPopup"].parse(args, kwargs, __FUNCTION__, &parent, &name, &mousebutton, &modal, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvPopup(parent, name, mousebutton, modal);
		mvApp::GetApp()->pushParent(item);
		mvApp::GetApp()->addItemManual(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endPopup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndPopup("");
		mvApp::GetApp()->addEndParentItem(item);
		
		Py_RETURN_NONE;
	}

	PyObject* addCollapsingHeader(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addCollapsingHeader"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCollapsingHeader("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endCollapsingHeader(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();	
		Py_RETURN_NONE;
	}

	PyObject* addTreeNode(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["addTreeNode"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTreeNode("", name);
		mvApp::GetApp()->addParentItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* endTreeNode(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* addSeperator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvSeparator("");
		mvApp::GetApp()->addItem(item);
		
		Py_RETURN_NONE;
	}

	PyObject* addColorEdit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		float r, g, b, a;

		if (!Translators["addColorEdit4"].parse(args, kwargs,__FUNCTION__, &name, &r, &g, &b, &a, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvColorEdit4("", name, r, g, b, a);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* addCheckbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = 0;

		if (!Translators["addCheckbox"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCheckbox("", name, default_value);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
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