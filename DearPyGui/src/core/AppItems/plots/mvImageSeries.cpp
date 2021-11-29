#include <utility>
#include "mvImageSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvImageSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::UUID, "texture_tag" });
		args.push_back({ mvPyDataType::DoubleList, "bounds_min"});
		args.push_back({ mvPyDataType::DoubleList, "bounds_max"});
		args.push_back({ mvPyDataType::FloatList, "uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "normalized texture coordinates"});
		args.push_back({ mvPyDataType::FloatList, "uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "normalized texture coordinates"});
		args.push_back({ mvPyDataType::IntList, "tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });

		mvPythonParserSetup setup;
		setup.about = "Adds an image series to a plot.";
		setup.category = { "Plotting", "Containers", "Widgets", "Textures" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvImageSeries::mvImageSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvImageSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvImageSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvImageSeries::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
	}

	void mvImageSeries::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------
		if (!_show)
			return;

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
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

				ImPlot::PlotImage(_internalLabel.c_str(), texture, _bounds_min, _bounds_max, _uv_min, _uv_max, _tintColor);

				// Begin a popup for a legend entry.
				if (ImPlot::BeginLegendPopup(_internalLabel.c_str(), 1))
				{
					for (auto& childset : _children)
					{
						for (auto& item : childset)
						{
							// skip item if it's not shown
							if (!item->_show)
								continue;
							item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
							UpdateAppItemState(item->_state);
						}
					}
					ImPlot::EndLegendPopup();
				}
			}
		}

		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------


		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

	}

	void mvImageSeries::handleSpecificRequiredArgs(PyObject* dict)
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
			{
				auto result = ToPoint(item);
				_bounds_min.x = result.x;
				_bounds_min.y = result.y;
				break;
			}

			case 2:
			{
				auto result = ToPoint(item);
				_bounds_max.x = result.x;
				_bounds_max.y = result.y;
				break;
			}

			default:
				break;
			}
		}
	}

	void mvImageSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) _uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) _uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) _tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_min"))
		{
			auto result = ToPoint(item);
			_bounds_min.x = result.x;
			_bounds_min.y = result.y;
		}
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_max"))
		{
			auto result = ToPoint(item);
			_bounds_max.x = result.x;
			_bounds_max.y = result.y;
		}

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

	}

	void mvImageSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_texture_id = ToPyUUID(_textureUUID);
		mvPyObject py_uv_min = ToPyPair(_uv_min.x, _uv_min.y);
		mvPyObject py_uv_max = ToPyPair(_uv_max.x, _uv_max.y);
		mvPyObject py_tint_color = ToPyColor(_tintColor);
		mvPyObject py_bounds_min = ToPyPair(_bounds_min.x, _bounds_min.y);
		mvPyObject py_bounds_max = ToPyPair(_bounds_max.x, _bounds_max.y);

		PyDict_SetItemString(dict, "texture_tag", py_texture_id);
		PyDict_SetItemString(dict, "uv_min", py_uv_min);
		PyDict_SetItemString(dict, "uv_max", py_uv_max);
		PyDict_SetItemString(dict, "tint_color", py_tint_color);
		PyDict_SetItemString(dict, "bounds_min", py_bounds_min);
		PyDict_SetItemString(dict, "bounds_max", py_bounds_max);
	}

	void mvImageSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvImageSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_textureUUID = titem->_textureUUID;
		_bounds_min = titem->_bounds_min;
		_bounds_max = titem->_bounds_max;
		_uv_min = titem->_uv_min;
		_uv_max = titem->_uv_max;
		_tintColor = titem->_tintColor;
		_texture = titem->_texture;
		_internalTexture = titem->_internalTexture;
	}
}
