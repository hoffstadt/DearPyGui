#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"

namespace Marvel {

	PyObject* addSimplePlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		float height = 80.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
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

		if(!pl.parse(__FUNCTION__, &parent, &name, &value, &autoscale, &overlay, &minscale, &maxscale, &height, &histogram))
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

		mvApp::GetApp()->addSimplePlot(std::string(parent), std::string(name), values, std::string(overlay),
			minscale, maxscale, height, histogram);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(117.0));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(1.0));

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "wrap"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Bool, "bullet")

			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &wrap, &color, &bullet))
			return Py_None;

		auto ncolor = pl.getFloatVec(color);

		bool specified = true;
		if (ncolor[0] > 100.0f)
			specified = false;

		mvApp::GetApp()->addText(std::string(parent), std::string(name), wrap, { ncolor[0], ncolor[1], ncolor[2], ncolor[3], specified }, bullet);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addLabelText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name, * value;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(117.0));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(1.0));

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::String, "value"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "wrap"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Bool, "bullet")

			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &value, &wrap, &color))
			return Py_None;

		auto ncolor = pl.getFloatVec(color);

		bool specified = true;
		if (ncolor[0] > 100.0f)
			specified = false;

		mvApp::GetApp()->addLabelText(std::string(parent), std::string(name), value, wrap, { ncolor[0], ncolor[1], ncolor[2], ncolor[3], specified }, bullet);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addListbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* parent, * name;
		PyObject* items;
		int default_value = 0, height = -1;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::StringList, "items"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value"),
			mvPythonDataElement(mvPythonDataType::Integer, "height"),
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &items, &default_value, &height))
			return Py_None;

		mvApp::GetApp()->addListbox(std::string(parent), std::string(name), pl.getStringVec(items), default_value, height);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCombo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name, *default_value = "";
		PyObject* items;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::StringList, "items"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::String, "default_value")
			});

		if (!pl.parse(__FUNCTION__, &parent, &name, &items, &default_value))
			return Py_None;

		mvApp::GetApp()->addCombo(std::string(parent), std::string(name), pl.getStringVec(items), default_value);

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

	PyObject* addButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name))
			return Py_None;

		mvApp::GetApp()->addButton(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, *name;
		const char* hint = "";
		int multiline = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::String, "hint"),
			mvPythonDataElement(mvPythonDataType::Bool, "multiline")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &hint, &multiline))
			return Py_None;

		mvApp::GetApp()->addInputText(std::string(parent), std::string(name), std::string(hint), multiline);
		
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputInt(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		int default_value = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &default_value))
			return Py_None;

		mvApp::GetApp()->addInputInt(std::string(parent), std::string(name), default_value);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addInputFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		float default_value = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "default_value")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &default_value))
			return Py_None;

		mvApp::GetApp()->addInputFloat(std::string(parent), std::string(name), default_value);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		float offset = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "offset")
			});

		if (!pl.parse(__FUNCTION__, &parent, &offset))
			return Py_None;

		mvApp::GetApp()->indent(std::string(parent), offset);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		float offset = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "offset")
			});

		if (!pl.parse(__FUNCTION__, &parent, &offset))
			return Py_None;

		mvApp::GetApp()->unindent(std::string(parent), offset);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &parent, &name);

		mvApp::GetApp()->addTabBar(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &parent, &name);

		mvApp::GetApp()->addTab(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endTab(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTabBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endTabBar(std::string(parent));

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

		mvApp::GetApp()->addMenuBar(std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &parent, &name);

		mvApp::GetApp()->addMenu(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endMenu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endMenu(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endMenuBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endMenuBar(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenuItem(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name))
			return Py_None;

		mvApp::GetApp()->addMenuItem(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSpacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		int count = 1;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "count")
			});

		if(!pl.parse(__FUNCTION__, &parent, &count))
			return Py_None;


		mvApp::GetApp()->addSpacing(std::string(parent), count);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSameLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		float xoffset = 0.0f;
		float spacing = 0.0f;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "xoffset"),
			mvPythonDataElement(mvPythonDataType::Float, "spacing")
			});

		if(!pl.parse(__FUNCTION__, &parent, &xoffset, &spacing))
			return Py_None;

		mvApp::GetApp()->addSameLine(std::string(parent), xoffset, spacing);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addRadioButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		const char* name;
		PyObject* items;
		int default_value = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::StringList, "items"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &items, &default_value))
			return Py_None;

		mvApp::GetApp()->addRadioButtons(std::string(parent), std::string(name), pl.getStringVec(items), default_value);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &parent, &name);

		mvApp::GetApp()->addGroup(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endGroup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent")
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endGroup(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addChild(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		int width, height;

		PyArg_ParseTuple(args, "ssii", &parent, &name, &width, &height);

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(__FUNCTION__, &parent, &name, &width, &height);

		mvApp::GetApp()->addChild(std::string(parent), std::string(name), width, height);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endChild(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent")
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endChild(std::string(parent));

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

		mvApp::GetApp()->addTooltip(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent")
			});

		pl.parse(__FUNCTION__, &parent);

		mvApp::GetApp()->endTooltip(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCollapsingHeader(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name")
			});

		pl.parse(__FUNCTION__, &parent, &name);

		mvApp::GetApp()->addCollapsingHeader(std::string(parent), std::string(name));

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

	PyObject* addSeperator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent")
			});

		if(!pl.parse(__FUNCTION__, &parent))
			return Py_None;

		mvApp::GetApp()->addSeperator(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addColorEdit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		float r, g, b, a;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Float, "r"),
			mvPythonDataElement(mvPythonDataType::Float, "g"),
			mvPythonDataElement(mvPythonDataType::Float, "b"),
			mvPythonDataElement(mvPythonDataType::Float, "a")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &r, &g, &b, &a))
			return Py_None;

		mvApp::GetApp()->addColorEdit4(std::string(parent), std::string(name), { r, g, b, a });

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addCheckbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		int default_value = 0;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "default_value")
			});

		if(!pl.parse(__FUNCTION__, &parent, &name, &default_value))
			return Py_None;

		mvApp::GetApp()->addCheckbox(std::string(parent), std::string(name), default_value);

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreatePythonInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(indent, "Not Documented");
		pyModule.addMethod(unindent, "Not Documented");
		pyModule.addMethod(addSimplePlot, "Not Documented");
		pyModule.addMethod(showItem, "Not Documented");
		pyModule.addMethod(hideItem, "Not Documented");
		pyModule.addMethod(addCombo, "Not Documented");
		pyModule.addMethod(addText, "Not Documented");
		pyModule.addMethod(addLabelText, "Not Documented");
		pyModule.addMethod(addListbox, "Not Documented");
		pyModule.addMethod(changeThemeItem, "Not Documented");
		pyModule.addMethod(updateTheme, "Not Documented");
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
		pyModule.addMethod(setValue, "Not Documented");
		pyModule.addMethod(addCollapsingHeader, "Not Documented");

		PyImport_AppendInittab(pyModule.getName() , initfunc);
	}
}