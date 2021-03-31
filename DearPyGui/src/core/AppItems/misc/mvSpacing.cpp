#include "mvSpacing.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvSpacing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_spacing", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'spacing'"},
			{mvPythonDataType::Integer, "count", "", "1"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds vertical spacing.", "None", "Adding Widgets") });
	}


	mvSpacing::mvSpacing(const std::string& name, int count)
		: mvIntPtrBase(name, count)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvSpacing::draw()
	{
		for (int i = 0; i < *m_value; i++)
			ImGui::Spacing();
	}

	PyObject* mvSpacing::add_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int count = 1;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_spacing"].parse(args, kwargs, __FUNCTION__, &name, &count,
			&parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvSpacing>(name, count);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}

}