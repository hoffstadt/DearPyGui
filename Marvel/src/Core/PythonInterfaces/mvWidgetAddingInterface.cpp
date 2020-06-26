#include "Core/PythonUtilities/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/PythonUtilities/mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbWidgets");

	std::map<std::string, mvPythonTranslator> BuildWidgetsInterface() {

		std::map<std::string, mvPythonTranslator> translators = {

			{"endTreeNode", mvPythonTranslator({
			}, false, "not documented")},

			{"endPopup", mvPythonTranslator({
			}, false, "not documented")},

			{"endWindow", mvPythonTranslator({
			}, false, "not documented")},

			{"endMainWindow", mvPythonTranslator({
			}, false, "not documented")},

			{"endGroup", mvPythonTranslator({
			}, false, "not documented")},

			{"endChild", mvPythonTranslator({
			}, false, "not documented")},

			{"endTab", mvPythonTranslator({
			}, false, "not documented")},

			{"endTabBar", mvPythonTranslator({
			}, false, "not documented")},

			{"endMenu", mvPythonTranslator({
			}, false, "not documented")},

			{"endMenuBar", mvPythonTranslator({
			}, false, "not documented")},

			{"endTooltip", mvPythonTranslator({
			}, false, "not documented")},

			{"endCollapsingHeader", mvPythonTranslator({
			}, false, "not documented")},

			{"addSimplePlot", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "value", "Tuple of float values"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Bool, "autoscale", "autoscales range based on data (default is True)"},
				{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)"},
				{mvPythonDataType::Float, "minscale", "used if autoscale is false"},
				{mvPythonDataType::Float, "maxscale", "used if autoscale is false"},
				{mvPythonDataType::Bool, "histogram", "create a histogram"}
			}, true, "A simple plot for visualization of a set of values")},

			{"addProgressBar", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "value"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::String, "overlay"}
			}, true, "A progress bar.")},

			{"addImage", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::String, "value"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "tint_color"},
				{mvPythonDataType::FloatList, "border_color"}
			}, true, "An image.")},

			{"addSliderFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Float, "default_value"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"},
				{mvPythonDataType::Bool, "vertical"}
			}, true, "A slider for a single float value")},

			{"addSliderFloat2", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A slider for a 2 float values.")},

			{"addSliderFloat3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A slider for a 3 float values.")},

			{"addSliderFloat4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A slider for a 4 float values.")},

			{"addSliderInt", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "default_value"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Bool, "vertical"}
			}, true, "A slider for a single int value")},

			{"addSliderInt2", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A slider for a 2 int values.")},

			{"addSliderInt3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A slider for a 3 int values.")},

			{"addSliderInt4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A slider for a 4 int values.")},

			{"addDragFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Float, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A drag for a single float value")},

			{ "addDragFloat2", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A drag for a 2 float values.") },

			{ "addDragFloat3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A drag for a 3 float values.") },

			{"addDragFloat4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A drag for a 4 float values.")},

			{"addDragInt", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A drag for a single int value")},

			{ "addDragInt2", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A drag for a 2 int values.") },

			{ "addDragInt3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A drag for a 3 int values.") },

			{"addDragInt4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Float, "speed"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A drag for a 4 int values.")},

			{"addText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
				{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
				{mvPythonDataType::Bool, "bullet", "makes the text bulleted"}
			}, true, "Adds text")},

			{"addLabelText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::String, "value"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "color"},
			}, true, "A text with a label. Useful for output values.")},

			{"addListbox", mvPythonTranslator({
				{mvPythonDataType::String, "name", "Name of the listbox"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"},
			}, true, "A Listbox is a listbox")},

			{"addCombo", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addSelectable", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Bool, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addButton", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
				{mvPythonDataType::Bool, "arrow", "Arrow button."},
				{mvPythonDataType::Integer, "direction", "A cardinal direction"},
			}, true, "A button.")},

			{"addInputText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::String, "hint"},
				{mvPythonDataType::Bool, "multiline"},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A input for text values.")},

			{"addInputInt", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"},
			}, true, "A Listbox is a listbox")},

			{ "addInputInt2", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value"},
			}, true, "A Listbox is a listbox") },

			{ "addInputInt3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value"},
			}, true, "A Listbox is a listbox") },

			{ "addInputInt4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value"},
			}, true, "A Listbox is a listbox") },

			{"addInputFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "default_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A Listbox is a listbox")},

			{ "addInputFloat2", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A Listbox is a listbox") },

			{ "addInputFloat3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A Listbox is a listbox") },

			{ "addInputFloat4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A Listbox is a listbox") },

			{"indent", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "offset"}
			}, false, "A Listbox is a listbox")},

			{"unindent", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "offset"}
			}, false, "A Listbox is a listbox")},

			{"addTabBar", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A tab bar.")},

			{"addTab", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addMenuBar", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "A Listbox is a listbox")},

			{"addMenu", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "A Listbox is a listbox")},

			{"addMenuItem", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addSpacing", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "count"}
			}, false, "A Listbox is a listbox")},

			{"addSameLine", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "xoffset"},
				{mvPythonDataType::Float, "spacing"}
			}, false, "A Listbox is a listbox")},

			{"addRadioButton", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox") },

			{"addGroup", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox") },

			{"addChild", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "A Listbox is a listbox") },

			{"addWindow", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "A Listbox is a listbox") },

			{"addTooltip", mvPythonTranslator({
				{mvPythonDataType::String, "parent"},
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox") },

			{"addPopup", mvPythonTranslator({
				{mvPythonDataType::String, "parent"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "mousebutton"},
				{mvPythonDataType::Integer, "modal"}
			}, true, "A Listbox is a listbox") },

			{"addCollapsingHeader", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"}
			}, false, "A Listbox is a listbox") },

			{"addTreeNode", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A Listbox is a listbox") },

			{"addColorEdit3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
			}, true, "A Listbox is a listbox") },


			{ "addColorEdit4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
			}, true, "A Listbox is a listbox") },

			{ "addColorPicker3", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
			}, true, "A Listbox is a listbox") },


			{ "addColorPicker4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
			}, true, "A Listbox is a listbox") },

			{"addCheckbox", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox") },

			{"addDragFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "default_value"}
			}, true, "This is useful for easy selection of a float withing a set range") }

		};

		return translators;
	}

	PyObject* addSimplePlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;

		if (!Translators["addSimplePlot"].parse(args, kwargs,__FUNCTION__, &name, &value, &autoscale, &overlay, 
			&minscale, &maxscale, &histogram, MV_STANDARD_CALLBACK_PARSE))
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

	PyObject* addProgressBar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		float default_value;
		const char* overlay = "";

		if (!Translators["addProgressBar"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &overlay, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvProgressBar("", name, default_value, overlay);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addImage(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* value;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(1.0));
		PyObject* bordercolor = PyTuple_New(4);
		PyTuple_SetItem(bordercolor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 3, PyFloat_FromDouble(0.0));

		if (!Translators["addImage"].parse(args, kwargs, __FUNCTION__, &name, &value, &tintcolor, &bordercolor, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto mtintcolor = mvPythonTranslator::getColor(tintcolor);
		auto mbordercolor = mvPythonTranslator::getColor(bordercolor);

		mvAppItem* item = new mvImage("", name, value, mtintcolor, mbordercolor);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		float default_value = 0.0f;
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addDragFloat"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragFloat("", name, default_value, speed, min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragFloat2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addDragFloat2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat2("", name, vec.data(), speed, min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragFloat3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addDragFloat3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat3("", name, vec.data(), speed, min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragFloat4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addDragFloat4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat4("", name, vec.data(), speed, min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragInt(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int default_value = 0;
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addDragInt"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed, 
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragInt("", name, default_value, speed, min_value, max_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragInt2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addDragInt2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt2("", name, vec.data(), speed, min_value, max_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragInt3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addDragInt3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt3("", name, vec.data(), speed, min_value, max_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addDragInt4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addDragInt4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt4("", name, vec.data(), speed, min_value, max_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		float default_value = 0.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		int vertical = false;

		if (!Translators["addSliderFloat"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&min_value, &max_value, &format, &power, &vertical, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderFloat("", name, default_value, min_value, max_value, format, power, vertical);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderFloat2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addSliderFloat2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat2("", name, vec.data(), min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderFloat3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addSliderFloat3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat3("", name, vec.data(), min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderFloat4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["addSliderFloat4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat4("", name, vec.data(), min_value, max_value, format, power);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderInt(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int default_value = 0;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		int vertical = false;

		if (!Translators["addSliderInt"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderInt("", name, default_value, min_value, max_value, format, vertical);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderInt2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addSliderInt2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt2("", name, vec.data(), min_value, max_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderInt3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addSliderInt3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt3("", name, vec.data(), min_value, max_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addSliderInt4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["addSliderInt4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt4("", name, vec.data(), min_value, max_value, format);
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
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));

		if (!Translators["addText"].parse(args, kwargs,__FUNCTION__, &name, &wrap, &color, &bullet, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", name, wrap, mcolor, bullet);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addLabelText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* value;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));

		if (!Translators["addLabelText"].parse(args, kwargs,__FUNCTION__, &name, &value, &color, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);
		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, mcolor);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addListbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0;

		if (!Translators["addListbox"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::getStringVec(items), default_value, height);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addCombo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		const char* default_value = "";
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
		int small = false;
		int arrow = false;
		int direction = -1;

		if (!Translators["addButton"].parse(args, kwargs,__FUNCTION__, &name, &small,
			&arrow, &direction, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvButton("", name, small, arrow, direction);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		const char* hint = "";
		int multiline = 0;
		int flags = 0;

		if (!Translators["addInputText"].parse(args, kwargs,__FUNCTION__, &name, &hint, &multiline, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputText("", name, hint, multiline, flags);
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

	PyObject* addInputInt2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));

		if (!Translators["addInputInt2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt2("", name, vec.data());
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputInt3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));

		if (!Translators["addInputInt3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt3("", name, vec.data());
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputInt4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));

		if (!Translators["addInputInt4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt4("", name, vec.data());
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputFloat(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		float default_value = 0.0f;
		const char* format = "%.3f";

		if (!Translators["addInputFloat"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputFloat("", name, default_value, format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputFloat2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* format = "%.3f";

		if (!Translators["addInputFloat2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat2("", name, vec.data(), format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputFloat3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* format = "%.3f";

		if (!Translators["addInputFloat3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat3("", name, vec.data(), format);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addInputFloat4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* format = "%.3f";

		if (!Translators["addInputFloat4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat4("", name, vec.data(), format);
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
		int flags = 0;

		if (!Translators["addTabBar"].parse(args, kwargs, __FUNCTION__, &name, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTabBar("", name, flags);
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

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
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

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
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

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
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

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
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

		Py_RETURN_NONE;
	}

	PyObject* addWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		if (!Translators["addWindow"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvWindowAppitem("", name, width, height);
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

		Py_RETURN_NONE;
	}

	PyObject* endWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* endMainWindow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppItem* item = new mvEndWindowAppitem("");
		mvApp::GetApp()->addItem(item);

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
		mvApp::GetApp()->popParent();
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
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;
	}

	PyObject* addCollapsingHeader(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int flags = 0;

		if (!Translators["addCollapsingHeader"].parse(args, kwargs, __FUNCTION__, &name, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCollapsingHeader("", name, flags);
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

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
		int flags = 0;

		if (!Translators["addTreeNode"].parse(args, kwargs, __FUNCTION__, &name, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTreeNode("", name, flags);
		mvApp::GetApp()->addItem(item);
		mvApp::GetApp()->pushParent(item);

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
	
	PyObject* addColorEdit3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));

		if (!Translators["addColorEdit3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit3("", name, color);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addColorEdit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));

		if (!Translators["addColorEdit4"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit4("", name, color);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();
		
		Py_RETURN_NONE;
	}

	PyObject* addColorPicker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));

		if (!Translators["addColorPicker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker3("", name, color);
		mvApp::GetApp()->addItem(item);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* addColorPicker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(1));

		if (!Translators["addColorPicker4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker4("", name, color);
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

	mvPythonModule* CreateWidgetAddingInterface()
	{

		auto pyModule = new mvPythonModule("sbWidgets", {});

		pyModule->addMethodD(endTreeNode);
		pyModule->addMethodD(endPopup);
		pyModule->addMethodD(endWindow);
		pyModule->addMethodD(endMainWindow);
		pyModule->addMethodD(endGroup);
		pyModule->addMethodD(endChild);
		pyModule->addMethodD(endTab);
		pyModule->addMethodD(endTabBar);
		pyModule->addMethodD(endMenu);
		pyModule->addMethodD(endMenuBar);
		pyModule->addMethodD(endTooltip);
		pyModule->addMethodD(endCollapsingHeader);
		pyModule->addMethodD(addImage);
		pyModule->addMethodD(addProgressBar);
		pyModule->addMethodD(addDragFloat);
		pyModule->addMethodD(addDragInt);
		pyModule->addMethodD(addDragFloat2);
		pyModule->addMethodD(addDragFloat3);
		pyModule->addMethodD(addDragFloat4);
		pyModule->addMethodD(addDragInt2);
		pyModule->addMethodD(addDragInt3);
		pyModule->addMethodD(addDragInt4);
		pyModule->addMethodD(addSliderFloat);
		pyModule->addMethodD(addSliderInt);
		pyModule->addMethodD(addSliderFloat2);
		pyModule->addMethodD(addSliderFloat3);
		pyModule->addMethodD(addSliderFloat4);
		pyModule->addMethodD(addSliderInt2);
		pyModule->addMethodD(addSliderInt3);
		pyModule->addMethodD(addSliderInt4);
		pyModule->addMethodD(addTreeNode);
		pyModule->addMethodD(addSelectable);
		pyModule->addMethodD(addPopup);
		pyModule->addMethodD(addWindow);
		pyModule->addMethodD(indent);
		pyModule->addMethodD(unindent);
		pyModule->addMethodD(addSimplePlot);
		pyModule->addMethodD(addCombo);
		pyModule->addMethodD(addText);
		pyModule->addMethodD(addLabelText);
		pyModule->addMethodD(addListbox);
		pyModule->addMethodD(addColorEdit3);
		pyModule->addMethodD(addColorEdit4);
		pyModule->addMethodD(addColorPicker3);
		pyModule->addMethodD(addColorPicker4);
		pyModule->addMethodD(addSeperator);
		pyModule->addMethodD(addButton);
		pyModule->addMethodD(addInputText);
		pyModule->addMethodD(addInputInt);
		pyModule->addMethodD(addInputFloat);
		pyModule->addMethodD(addRadioButton);
		pyModule->addMethodD(addCheckbox);
		pyModule->addMethodD(addGroup);
		pyModule->addMethodD(addChild);
		pyModule->addMethodD(addTabBar);
		pyModule->addMethodD(addTab);
		pyModule->addMethodD(addMenuBar);
		pyModule->addMethodD(addMenu);
		pyModule->addMethodD(addMenuItem);
		pyModule->addMethodD(addSpacing);
		pyModule->addMethodD(addSameLine);
		pyModule->addMethodD(addTooltip);
		pyModule->addMethodD(addCollapsingHeader);
		
		return pyModule;
	}
}