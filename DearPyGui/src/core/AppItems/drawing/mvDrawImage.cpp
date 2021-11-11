#include "mvDrawImage.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::UUID, "texture_tag" });
		args.push_back({ mvPyDataType::FloatList, "pmin", mvArgType::REQUIRED_ARG, "...", "Point of to start drawing texture." });
		args.push_back({ mvPyDataType::FloatList, "pmax", mvArgType::REQUIRED_ARG, "...", "Point to complete drawing texture." });
		args.push_back({ mvPyDataType::FloatList, "uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized coordinates on texture that will be drawn." });
		args.push_back({ mvPyDataType::FloatList, "uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized coordinates on texture that will be drawn." });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });

		mvPythonParserSetup setup;
		setup.about = "Adds an image (for a drawing).";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDrawImage::mvDrawImage(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawImage::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawImage*>(item);
		_textureUUID = titem->_textureUUID;
		_pmax = titem->_pmax;
		_pmin = titem->_pmin;
		_uv_min = titem->_uv_min;
		_uv_max = titem->_uv_max;
		_color = titem->_color;
		_texture = titem->_texture;
		_internalTexture = titem->_internalTexture;
	}

	void mvDrawImage::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_texture)
		{
			if (_internalTexture)
				_texture->draw(drawlist, x, y);

			if (!_texture->_state.ok)
				return;

			void* texture = nullptr;

			if (_texture->getType() == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(_texture.get())->getRawTexture();
			else if (_texture->getType() == mvAppItemType::mvRawTexture)
				texture = static_cast<mvRawTexture*>(_texture.get())->getRawTexture();
			else
				texture = static_cast<mvDynamicTexture*>(_texture.get())->getRawTexture();

			mvVec4  tpmin = _transform * _pmin;
			mvVec4  tpmax = _transform * _pmax;

			if (_perspectiveDivide)
			{
				tpmin.x = tpmin.x / tpmin.w;
				tpmax.x = tpmax.x / tpmax.w;

				tpmin.y = tpmin.y / tpmin.w;
				tpmax.y = tpmax.y / tpmax.w;

				tpmin.z = tpmin.z / tpmin.w;
				tpmax.z = tpmax.z / tpmax.w;
			}

			if (_depthClipping)
			{
				if (mvClipPoint(_clipViewport, tpmin)) return;
				if (mvClipPoint(_clipViewport, tpmax)) return;
			}

			if (ImPlot::GetCurrentContext()->CurrentPlot)
				drawlist->AddImage(texture, ImPlot::PlotToPixels(tpmin), ImPlot::PlotToPixels(tpmax), _uv_min, _uv_max, _color);
			else
			{
				mvVec2 start = { x, y };
				drawlist->AddImage(texture, tpmin + start, tpmax + start, _uv_min, _uv_max, _color);
			}
		}
	}

	void mvDrawImage::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				_textureUUID = GetIDFromPyObject(item);
				_texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
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
				_pmin = ToVec4(item);
				_pmin.w = 1.0f;
				break;

			case 2:
				_pmax = ToVec4(item);
				_pmax.w = 1.0f;
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

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) _pmax = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) _pmin = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) _uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) _uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "texture_tag"))
        {
            _textureUUID = GetIDFromPyObject(item);
            _texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
            if (_textureUUID == MV_ATLAS_UUID)
            {
                _texture = std::make_shared<mvStaticTexture>(_textureUUID);
                _internalTexture = true;
            }
            else if(_texture)
            {
                _internalTexture = false;
            }
            else
            {
                mvThrowPythonError(mvErrorCode::mvTextureNotFound, s_command, "Texture not found.", this);
			}
        }

		_pmin.w = 1.0f;
		_pmax.w = 1.0f;
	}

	void mvDrawImage::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "pmax", mvPyObject(ToPyPair(_pmax.x, _pmax.y)));
		PyDict_SetItemString(dict, "pmin", mvPyObject(ToPyPair(_pmin.x, _pmin.y)));
		PyDict_SetItemString(dict, "uv_min", mvPyObject(ToPyPair(_uv_min.x, _uv_min.y)));
		PyDict_SetItemString(dict, "uv_max", mvPyObject(ToPyPair(_uv_max.x, _uv_max.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "texture_tag", mvPyObject(ToPyUUID(_textureUUID)));
	}

}
