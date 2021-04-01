#include "mvDummy.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {



	void mvDummy::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "name", "", "'dummy'"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"}
		}, "Adds a spacer or 'dummy' object.", "None", "Adding Widgets") });
	}


	mvDummy::mvDummy(const std::string& name)
		: mvAppItem(name)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvDummy::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::Dummy({ (float)m_core_config.width, (float)m_core_config.height });
	}

	PyObject* mvDummy::add_dummy(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int width;
		int height;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_dummy"].parse(args, kwargs, __FUNCTION__, &width, &height, &name,
			&parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvDummy>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}

}