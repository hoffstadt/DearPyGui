#include "mvDrawImageQuad.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvDrawImageQuad::mvDrawImageQuad(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawImageQuad::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawImageQuad*>(item);
		_textureUUID = titem->_textureUUID;
		_p1 = titem->_p1;
		_p2 = titem->_p2;
		_p3 = titem->_p3;
		_p4 = titem->_p4;
		_uv1 = titem->_uv1;
		_uv2 = titem->_uv2;
		_uv3 = titem->_uv3;
		_uv4 = titem->_uv4;
		_color = titem->_color;
		_texture = titem->_texture;
		_internalTexture = titem->_internalTexture;
	}

	void mvDrawImageQuad::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_texture)
		{
			if (_internalTexture)
				_texture->draw(drawlist, x, y);

			if (!_texture->state.ok)
				return;

			void* texture = nullptr;

			if (_texture->type == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(_texture.get())->_texture;
			else if (_texture->type == mvAppItemType::mvRawTexture)
				texture = static_cast<mvRawTexture*>(_texture.get())->_texture;
			else
				texture = static_cast<mvDynamicTexture*>(_texture.get())->_texture;

			mvVec4  tp1 = drawInfo->transform * _p1;
			mvVec4  tp2 = drawInfo->transform * _p2;
			mvVec4  tp3 = drawInfo->transform * _p3;
			mvVec4  tp4 = drawInfo->transform * _p4;

			if (drawInfo->perspectiveDivide)
			{
				tp1.x = tp1.x / tp1.w;
				tp2.x = tp2.x / tp2.w;
				tp3.x = tp3.x / tp3.w;
				tp4.x = tp4.x / tp4.w;

				tp1.y = tp1.y / tp1.w;
				tp2.y = tp2.y / tp2.w;
				tp3.y = tp3.y / tp3.w;
				tp4.y = tp4.y / tp4.w;

				tp1.z = tp1.z / tp1.w;
				tp2.z = tp2.z / tp2.w;
				tp3.z = tp3.z / tp3.w;
				tp4.z = tp4.z / tp4.w;
			}

			if (drawInfo->depthClipping)
			{
				if (mvClipPoint(drawInfo->clipViewport, tp1)) return;
				if (mvClipPoint(drawInfo->clipViewport, tp2)) return;
				if (mvClipPoint(drawInfo->clipViewport, tp3)) return;
				if (mvClipPoint(drawInfo->clipViewport, tp4)) return;
			}

			if (ImPlot::GetCurrentContext()->CurrentPlot)
				drawlist->AddImageQuad(texture, ImPlot::PlotToPixels(tp1), 
					ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3), ImPlot::PlotToPixels(tp4), 
					_uv1, _uv2, _uv3, _uv4, _color);
			else
			{
				mvVec2 start = { x, y };
				drawlist->AddImageQuad(texture, tp1 + start, tp2 + start, tp3.xy(), tp4.xy(), _uv1, _uv2, _uv3, _uv4, _color);
			}
		}
	}

	void mvDrawImageQuad::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
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
					mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);
					break;
				}
			}

			case 1:
				_p1 = ToVec4(item);
				_p1.w = 1.0f;
				break;

			case 2:
				_p2 = ToVec4(item);
				_p2.w = 1.0f;
				break;

			case 3:
				_p3 = ToVec4(item);
				_p3.w = 1.0f;
				break;

			case 4:
				_p4 = ToVec4(item);
				_p4.w = 1.0f;
				break;

			default:
				break;
			}
		}
	}

	void mvDrawImageQuad::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p4")) _p4 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv1")) _uv1= ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv2")) _uv2= ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv3")) _uv3= ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv4")) _uv4= ToVec2(item);
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
                mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);
			}
        }

		_p1.w = 1.0f;
		_p2.w = 1.0f;
		_p3.w = 1.0f;
		_p4.w = 1.0f;
	}

	void mvDrawImageQuad::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
		PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
		PyDict_SetItemString(dict, "p3", mvPyObject(ToPyPair(_p3.x, _p3.y)));
		PyDict_SetItemString(dict, "p4", mvPyObject(ToPyPair(_p4.x, _p4.y)));
		PyDict_SetItemString(dict, "uv1", mvPyObject(ToPyPair(_uv1.x, _uv1.y)));
		PyDict_SetItemString(dict, "uv2", mvPyObject(ToPyPair(_uv2.x, _uv2.y)));
		PyDict_SetItemString(dict, "uv3", mvPyObject(ToPyPair(_uv3.x, _uv3.y)));
		PyDict_SetItemString(dict, "uv4", mvPyObject(ToPyPair(_uv4.x, _uv4.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "texture_tag", mvPyObject(ToPyUUID(_textureUUID)));
	}

}
