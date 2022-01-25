#include "mvImageItems.h"
#include "mvItemRegistry.h"
#include "mvContext.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

	mvImage::mvImage(mvUUID uuid)
		: mvAppItem(uuid)
	{
		config.width = 0;
		config.height = 0;
	}

	void mvImage::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvImage*>(item);
		_textureUUID = titem->_textureUUID;
		_uv_min = titem->_uv_min;
		_uv_max = titem->_uv_max;
		_tintColor = titem->_tintColor;
		_borderColor = titem->_borderColor;
		_texture = titem->_texture;
		_internalTexture = titem->_internalTexture;
	}

	void mvImage::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!config.show)
			return;

		// focusing
		if (info.focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			info.focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (info.dirtyPos)
			ImGui::SetCursorPos(state.pos);

		// update widget's position state
		state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (config.width != 0)
			ImGui::SetNextItemWidth((float)config.width);

		// set indent
		if (config.indent > 0.0f)
			ImGui::Indent(config.indent);

		// push font if a font object is attached
		if (font)
		{
			ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		apply_local_theming(this);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			if (_texture)
			{
				if (_internalTexture)
					_texture->draw(drawlist, x, y);

				if (!_texture->state.ok)
					return;

				// if width/height is not set by user, use texture dimensions
				if (config.width == 0)
					config.width = _texture->config.width;

				if (config.height == 0)
					config.height = _texture->config.height;

				void* texture = nullptr;

				if (_texture->type == mvAppItemType::mvStaticTexture)
					texture = static_cast<mvStaticTexture*>(_texture.get())->_texture;
				else if (_texture->type == mvAppItemType::mvRawTexture)
					texture = static_cast<mvRawTexture*>(_texture.get())->_texture;
				else
					texture = static_cast<mvDynamicTexture*>(_texture.get())->_texture;

				ImGui::Image(texture, ImVec2((float)config.width, (float)config.height), ImVec2(_uv_min.x, _uv_min.y), ImVec2(_uv_max.x, _uv_max.y),
					ImVec4((float)_tintColor.r, (float)_tintColor.g, (float)_tintColor.b, (float)_tintColor.a),
					ImVec4((float)_borderColor.r, (float)_borderColor.g, (float)_borderColor.b, (float)_borderColor.a));

			}
		}


		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		UpdateAppItemState(state);

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (info.dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (config.indent > 0.0f)
			ImGui::Unindent(config.indent);

		// pop font off stack
		if (font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

		// handle drag & drop if used
		apply_drag_drop(this);

	}

	void mvImage::setValue(mvUUID value)
	{ 
		_textureUUID = value;
	}

	mvUUID mvImage::get1Value() const
	{ 
		return _textureUUID;
	}

	void mvImage::handleSpecificRequiredArgs(PyObject* dict)
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

			default:
				break;
			}
		}
	}

	void mvImage::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) _uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) _uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) _tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "border_color")) _borderColor = ToColor(item);
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
	}

	void mvImage::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", mvPyObject(ToPyPair(_uv_min.x, _uv_min.y)));
		PyDict_SetItemString(dict, "uv_max", mvPyObject(ToPyPair(_uv_max.x, _uv_max.y)));
		PyDict_SetItemString(dict, "tint_color", mvPyObject(ToPyColor(_tintColor)));
		PyDict_SetItemString(dict, "border_color", mvPyObject(ToPyColor(_borderColor)));
		PyDict_SetItemString(dict, "texture_tag", mvPyObject(ToPyUUID(_textureUUID)));
	}

	mvImageButton::mvImageButton(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvImageButton::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvImageButton*>(item);
		_textureUUID = titem->_textureUUID;
		_uv_min = titem->_uv_min;
		_uv_max = titem->_uv_max;
		_tintColor = titem->_tintColor;
		_backgroundColor = titem->_backgroundColor;
		_texture = titem->_texture;
		_internalTexture = titem->_internalTexture;
		_framePadding = titem->_framePadding;
	}

	void mvImageButton::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!config.show)
			return;

		// focusing
		if (info.focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			info.focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (info.dirtyPos)
			ImGui::SetCursorPos(state.pos);

		// update widget's position state
		state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (config.width != 0)
			ImGui::SetNextItemWidth((float)config.width);

		// set indent
		if (config.indent > 0.0f)
			ImGui::Indent(config.indent);

		// push font if a font object is attached
		if (font)
		{
			ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		apply_local_theming(this);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{

			if (_texture)
			{

				if (_internalTexture)
					_texture->draw(drawlist, x, y);

				if (!_texture->state.ok)
					return;

				// if width/height is not set by user, use texture dimensions
				if (config.width == 0)
					config.width = _texture->config.width;

				if (config.height == 0)
					config.height = _texture->config.height;

				void* texture = nullptr;

				if (_texture->type == mvAppItemType::mvStaticTexture)
					texture = static_cast<mvStaticTexture*>(_texture.get())->_texture;
				else if (_texture->type == mvAppItemType::mvRawTexture)
					texture = static_cast<mvRawTexture*>(_texture.get())->_texture;
				else
					texture = static_cast<mvDynamicTexture*>(_texture.get())->_texture;

				ImGui::PushID(uuid);
				if (ImGui::ImageButton(texture, ImVec2((float)config.width, (float)config.height),
					ImVec2(_uv_min.x, _uv_min.y), ImVec2(_uv_max.x, _uv_max.y), _framePadding,
					_backgroundColor, _tintColor))
				{
					if (config.alias.empty())
						mvAddCallback(getCallback(false), uuid, nullptr, config.user_data);
					else
						mvAddCallback(getCallback(false), config.alias, nullptr, config.user_data);
				}
				ImGui::PopID();
			}
		}

		//-----------------------------------------------------------------------------
		// update state
		//-----------------------------------------------------------------------------
		UpdateAppItemState(state);

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (info.dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (config.indent > 0.0f)
			ImGui::Unindent(config.indent);

		// pop font off stack
		if (font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);

		if (handlerRegistry)
			handlerRegistry->checkEvents(&state);

		// handle drag & drop if used
		apply_drag_drop(this);
	}

	void mvImageButton::handleSpecificRequiredArgs(PyObject* dict)
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
		if (PyObject* item = PyDict_GetItemString(dict, "texture_tag"))
		{
			_textureUUID = GetIDFromPyObject(item);
			_texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
			if (_textureUUID == MV_ATLAS_UUID)
			{
				_texture = std::make_shared<mvStaticTexture>(_textureUUID);
				_internalTexture = true;
			}
			else if (_texture)
			{
				_internalTexture = false;
			}
			else
			{
				mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);
			}
		}
	}

	void mvImageButton::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", mvPyObject(ToPyPair(_uv_min.x, _uv_min.y)));
		PyDict_SetItemString(dict, "uv_max", mvPyObject(ToPyPair(_uv_max.x, _uv_max.y)));
		PyDict_SetItemString(dict, "tint_color", mvPyObject(ToPyColor(_tintColor)));
		PyDict_SetItemString(dict, "background_color", mvPyObject(ToPyColor(_backgroundColor)));
		PyDict_SetItemString(dict, "frame_padding", mvPyObject(ToPyInt(_framePadding)));
		PyDict_SetItemString(dict, "texture_tag", mvPyObject(ToPyUUID(_textureUUID)));
	}
}
