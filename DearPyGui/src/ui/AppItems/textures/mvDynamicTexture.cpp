#include "mvDynamicTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvDynamicTexture::mvDynamicTexture(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	mvDynamicTexture::~mvDynamicTexture()
	{
		FreeTexture(_texture);
	}

	void mvDynamicTexture::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDynamicTexture*>(item);
		if (config.source != 0) _value = titem->_value;
		_texture = titem->_texture;
		_permWidth = titem->_permWidth;
		_permHeight = titem->_permHeight;
		_dirty = titem->_dirty;
	}

	PyObject* mvDynamicTexture::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvDynamicTexture::setPyValue(PyObject* value)
	{
		*_value = ToFloatVect(value);
	}

	void mvDynamicTexture::setDataSource(mvUUID dataSource)
	{
		if (dataSource == config.source) return;
		config.source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->type) != GetEntityValueType(type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
	}

	void mvDynamicTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_dirty)
		{

			_texture = LoadTextureFromArrayDynamic(_permWidth, _permHeight, _value->data());

			if (_texture == nullptr)
				state.ok = false;

			_dirty = false;
			return;
		}

		UpdateTexture(_texture, _permWidth, _permHeight, *_value);

	}

	void mvDynamicTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_permWidth = ToInt(item);
				config.width = _permWidth;
				break;

			case 1:
				_permHeight = ToInt(item);
				config.height = _permHeight;
				break;

			case 2:
				*_value = ToFloatVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDynamicTexture::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

	}

	void mvDynamicTexture::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}