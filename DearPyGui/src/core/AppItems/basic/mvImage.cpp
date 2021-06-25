#include "mvImage.h"
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds an image from a specified texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture.", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::UUID>("texture_id");
		
		parser.addArg<mvPyDataType::FloatList>("tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "Applies a color tint to the entire texture.");
		parser.addArg<mvPyDataType::FloatList>("border_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)", "Displays a border of the specified color around the texture.");
		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized texture coordinates min point.");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized texture coordinates max point.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvImage::mvImage(mvUUID uuid)
		: mvAppItem(uuid)
	{
		m_width = 0;
		m_height = 0;
	}


	void mvImage::draw(ImDrawList* drawlist, float x, float y)
	{

		if (m_texture)
		{
			if (m_internalTexture)
				m_texture->draw(drawlist, x, y);

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
			else if (m_texture->getType() == mvAppItemType::mvRawTexture)
				texture = static_cast<mvRawTexture*>(m_texture.get())->getRawTexture();
			else
				texture = static_cast<mvDynamicTexture*>(m_texture.get())->getRawTexture();

			ImGui::Image(texture, ImVec2(m_width, m_height), ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y),
				ImVec4((float)m_tintColor.r, (float)m_tintColor.g, (float)m_tintColor.b, (float)m_tintColor.a),
				ImVec4((float)m_borderColor.r, (float)m_borderColor.g, (float)m_borderColor.b, (float)m_borderColor.a));

		}

	}

	void mvImage::setValue(mvUUID value)
	{ 
		m_textureUUID = value;
	}

	mvUUID mvImage::getValue() const
	{ 
		return m_textureUUID;
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
				m_textureUUID = ToUUID(item);
				m_texture = mvApp::GetApp()->getItemRegistry().getRefItem(m_textureUUID);
				if (m_texture)
					break;
				else if (m_textureUUID == MV_ATLAS_UUID)
				{
					m_texture = std::make_shared<mvStaticTexture>(m_textureUUID);
					m_internalTexture = true;
					break;
				}
				else
				{
					mvThrowPythonError(mvErrorCode::mvTextureNotFound, s_command, "Texture not found.", this);
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
		PyDict_SetItemString(dict, "texture_id", ToPyUUID(m_textureUUID));
	}

}