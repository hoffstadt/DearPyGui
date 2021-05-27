#include "mvImageButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvImageButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_CALLBACK_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::String>("default_value");

		parser.addArg<mvPyDataType::Integer>("frame_padding", mvArgType::KEYWORD_ARG, "-1");
		
		parser.addArg<mvPyDataType::FloatList>("tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("background_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "normalized texture coordinates");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvImageButton::mvImageButton(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvImageButton::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);

		if (m_texture)
		{

			if (!m_texture->getState().isOk())
				return;

			// if width/height is not set by user, use texture dimensions
			if (m_width == 0)
				m_width = m_texture->getWidth();

			if (m_height == 0)
				m_height = m_texture->getHeight();

			void* texture = nullptr;

			if (m_texture->getType() == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(m_texture.get())->getRawTexture();
			else
				texture = static_cast<mvDynamicTexture*>(m_texture.get())->getRawTexture();

			ImGui::PushID(m_name.c_str());
			if (ImGui::ImageButton(texture, ImVec2((float)m_width, (float)m_height),
				ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y), m_framePadding,
				m_backgroundColor, m_tintColor))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
			ImGui::PopID();
		}

	}

	void mvImageButton::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				m_value = ToString(item);
				m_texture = mvApp::GetApp()->getItemRegistry().getItem(m_value);
				if (m_texture)
					break;
				else
				{
					m_texture = std::make_shared<mvStaticTexture>(m_value);
					break;
				}
			}

			default:
				break;
			}
		}

		
	}

	void mvImageButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) m_uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) m_uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) m_tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "background_color")) m_backgroundColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "frame_padding")) m_framePadding = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "value")) m_value = ToString(item);
	}

	void mvImageButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", ToPyPair(m_uv_min.x, m_uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(m_uv_max.x, m_uv_max.y));
		PyDict_SetItemString(dict, "tint_color", ToPyColor(m_tintColor));
		PyDict_SetItemString(dict, "background_color", ToPyColor(m_backgroundColor));
		PyDict_SetItemString(dict, "frame_padding", ToPyInt(m_framePadding));
		PyDict_SetItemString(dict, "value", ToPyString(m_value));
	}

}