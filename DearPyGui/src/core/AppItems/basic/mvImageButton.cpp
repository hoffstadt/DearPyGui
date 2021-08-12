#include "mvImageButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvImageButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds an button with a texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::UUID>("texture_id");

		parser.addArg<mvPyDataType::Integer>("frame_padding", mvArgType::KEYWORD_ARG, "-1");
		
		parser.addArg<mvPyDataType::FloatList>("tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "Applies a color tint to the entire texture.");
		parser.addArg<mvPyDataType::FloatList>("background_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)", "Displays a border of the specified color around the texture.");
		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized texture coordinates min point.");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized texture coordinates max point.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvImageButton::mvImageButton(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvImageButton::draw(ImDrawList* drawlist, float x, float y)
	{

		if (_texture)
		{

			if (_internalTexture)
				_texture->draw(drawlist, x, y);

			if (!_texture->getState().isOk())
				return;

			// if width/height is not set by user, use texture dimensions
			if (_width == 0)
				_width = _texture->getWidth();

			if (_height == 0)
				_height = _texture->getHeight();

			void* texture = nullptr;

			if (_texture->getType() == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(_texture.get())->getRawTexture();
			else if (_texture->getType() == mvAppItemType::mvRawTexture)
				texture = static_cast<mvRawTexture*>(_texture.get())->getRawTexture();
			else
				texture = static_cast<mvDynamicTexture*>(_texture.get())->getRawTexture();

			ImGui::PushID(_uuid);
			if (ImGui::ImageButton(texture, ImVec2((float)_width, (float)_height),
				ImVec2(_uv_min.x, _uv_min.y), ImVec2(_uv_max.x, _uv_max.y), _framePadding,
				_backgroundColor, _tintColor))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, nullptr, _user_data);
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
				_textureUUID = mvAppItem::GetIDFromPyObject(item);
				_texture = mvApp::GetApp()->getItemRegistry().getRefItem(_textureUUID);
				if (_texture)
					break;
				else if (_textureUUID == MV_ATLAS_UUID)
				{
					_texture = std::make_shared<mvStaticTexture>(_textureUUID);
					_internalTexture = true;
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

	void mvImageButton::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) _uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) _uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) _tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "background_color")) _backgroundColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "frame_padding")) _framePadding = ToInt(item);
	}

	void mvImageButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", ToPyPair(_uv_min.x, _uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(_uv_max.x, _uv_max.y));
		PyDict_SetItemString(dict, "tint_color", ToPyColor(_tintColor));
		PyDict_SetItemString(dict, "background_color", ToPyColor(_backgroundColor));
		PyDict_SetItemString(dict, "frame_padding", ToPyInt(_framePadding));
		PyDict_SetItemString(dict, "texture_id", ToPyUUID(_textureUUID));
	}

}