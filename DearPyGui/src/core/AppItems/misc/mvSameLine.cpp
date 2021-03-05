#include "mvSameLine.h"
#include "mvItemRegistry.h"
#include "mvThemeManager.h"

namespace Marvel {

	void mvSameLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_same_line", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'sameline'"},
			{mvPythonDataType::Float, "xoffset", "offset from containing window", "0.0"},
			{mvPythonDataType::Float, "spacing", "offset from previous widget", "-1.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.",
		"None", "Adding Widgets") });
	}


	mvSameLine::mvSameLine(const std::string& name)
		: mvAppItem(name)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvSameLine::draw()
	{
		ImGui::SameLine(m_xoffset, m_spacing);
	}

#ifndef MV_CPP



#endif


#ifdef MV_CPP
#else

		void mvSameLine::setExtraConfigDict(PyObject* dict)
		{
			if (dict == nullptr)
				return;

			if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) m_xoffset = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "spacing")) m_spacing = ToFloat(item);

		}

		void mvSameLine::getExtraConfigDict(PyObject* dict)
		{
			if (dict == nullptr)
				return;

			PyDict_SetItemString(dict, "xoffset", ToPyFloat(m_xoffset));
			PyDict_SetItemString(dict, "spacing", ToPyFloat(m_spacing));
		}

	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("sameline" + std::to_string(i));
		const char* name = sname.c_str();
		float xoffset = 0.0f;
		float spacing = -1.0f;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_same_line"].parse(args, kwargs, __FUNCTION__, &name,
			&xoffset, &spacing, &parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvSameLine>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}
#endif

}