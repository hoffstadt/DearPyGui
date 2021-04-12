#include <utility>
#include "mvCheckbox.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvCheckbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");

		parser.addArg<mvPyDataType::Bool>("default_value", mvArgType::KEYWORD, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCheckbox::mvCheckbox(const std::string& name)
		: 
		mvBoolPtrBase(name)
	{
	}

	void mvCheckbox::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled) m_disabled_value = *m_value;

		if (ImGui::Checkbox(m_label.c_str(), m_enabled ? m_value.get() : &m_disabled_value))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

	}

}