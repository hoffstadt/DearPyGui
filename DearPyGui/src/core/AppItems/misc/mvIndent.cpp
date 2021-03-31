#include "mvIndent.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {


	void mvIndent::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_indent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'indent'"},
			{mvPythonDataType::Float, "offset", "", "0.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds an indent to following items. Must be closed with the unindent command.", "None", "Adding Widgets") });
	}


	mvIndent::mvIndent(const std::string& name, float default_value)
		: mvFloatPtrBase(name, default_value)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvIndent::draw()
	{
		ImGui::Indent(*m_value);
	}

	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("indent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_indent"].parse(args, kwargs, __FUNCTION__, &name, &offset,
			&parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvIndent>(name, offset);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

}