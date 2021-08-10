#include "mvDrawImage.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0.0,0.0)->(1.0,1.0) texturecoordinates will generally display the entire texture.", { "Textures", "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::UUID>("texture_id");
		parser.addArg<mvPyDataType::FloatList>("pmin", mvArgType::REQUIRED_ARG, "...", "Point of to start drawing texture.");
		parser.addArg<mvPyDataType::FloatList>("pmax", mvArgType::REQUIRED_ARG, "...", "Point to complete drawing texture.");

		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized coordinates on texture that will be drawn.");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized coordinates on texture that will be drawn.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawImage::mvDrawImage(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawImage::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec2 start = { x, y };

		if (_texture)
		{
			if (_internalTexture)
				_texture->draw(drawlist, x, y);

			if (!_texture->getState().isOk())
				return;

			void* texture = nullptr;

			if (_texture->getType() == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(_texture.get())->getRawTexture();
			else if (_texture->getType() == mvAppItemType::mvRawTexture)
				texture = static_cast<mvRawTexture*>(_texture.get())->getRawTexture();
			else
				texture = static_cast<mvDynamicTexture*>(_texture.get())->getRawTexture();

			if (ImPlot::GetCurrentContext()->CurrentPlot)
				drawlist->AddImage(texture, ImPlot::PlotToPixels(_pmin), ImPlot::PlotToPixels(_pmax), _uv_min, _uv_max, _color);
			else
				drawlist->AddImage(texture, _pmin + start, _pmax + start, _uv_min, _uv_max, _color);
		}
	}

	void mvDrawImage::handleSpecificRequiredArgs(PyObject* dict)
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
				_textureUUID = ToUUID(item);
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

			case 1:
				_pmin = ToVec2(item);
				break;

			case 2:
				_pmax = ToVec2(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawImage::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) _pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) _pmin = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) _uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) _uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);

	}

	void mvDrawImage::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "pmax", ToPyPair(_pmax.x, _pmax.y));
		PyDict_SetItemString(dict, "pmin", ToPyPair(_pmin.x, _pmin.y));
		PyDict_SetItemString(dict, "uv_min", ToPyPair(_uv_min.x, _uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(_uv_max.x, _uv_max.y));
		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "texture_id", ToPyUUID(_textureUUID));
	}

}