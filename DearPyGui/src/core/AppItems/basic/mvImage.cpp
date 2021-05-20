#include "mvImage.h"
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::String>("default_value");
		
		parser.addArg<mvPyDataType::FloatList>("tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::FloatList>("border_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)");
		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "normalized texture coordinates");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvImage::mvImage(const std::string& name)
		: mvAppItem(name)
	{
		m_width = 0;
		m_height = 0;
	}


	void mvImage::draw(ImDrawList* drawlist, float x, float y)
	{

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

			//ImGui::Image(texture, ImVec2((float)m_texture->getWidth(), (float)m_texture->getHeight()), ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y),
			ImGui::Image(texture, ImVec2(m_width, m_height), ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y),
				ImVec4((float)m_tintColor.r, (float)m_tintColor.g, (float)m_tintColor.b, (float)m_tintColor.a),
				ImVec4((float)m_borderColor.r, (float)m_borderColor.g, (float)m_borderColor.b, (float)m_borderColor.a));

		}

	}

	void mvImage::setValue(const std::string& value) 
	{ 
		m_value = value; 
	}

	const std::string& mvImage::getValue() const 
	{ 
		return m_value; 
	}

	void mvImage::handleSpecificRequiredArgs(PyObject* dict)
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
				if(m_texture)
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

	void mvImage::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) m_uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) m_uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) m_tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "border_color")) m_borderColor = ToColor(item);
	}

	void mvImage::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", ToPyPair(m_uv_min.x, m_uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(m_uv_max.x, m_uv_max.y));
		PyDict_SetItemString(dict, "tint_color", ToPyColor(m_tintColor));
		PyDict_SetItemString(dict, "border_color", ToPyColor(m_borderColor));
		PyDict_SetItemString(dict, "value", ToPyString(m_value));
	}

}