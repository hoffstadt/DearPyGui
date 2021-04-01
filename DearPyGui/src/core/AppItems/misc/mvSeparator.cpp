#include "mvSeparator.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvSeparator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'separator'"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a horizontal line.", "None", "Adding Widgets") });
	}


	mvSeparator::mvSeparator(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvSeparator::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);

		ImGui::Separator();

	}

	PyObject* mvSeparator::add_separator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["add_separator"].parse(args, kwargs, __FUNCTION__, &name, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvSeparator>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}

}