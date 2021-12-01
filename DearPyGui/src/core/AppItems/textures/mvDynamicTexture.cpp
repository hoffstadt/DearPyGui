#include "mvDynamicTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvDynamicTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		args.push_back({ mvPyDataType::Integer, "width" });
		args.push_back({ mvPyDataType::Integer, "height" });
		args.push_back({ mvPyDataType::FloatList, "default_value" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_2", "Parent to add this item to. (runtime adding)" });
		
		mvPythonParserSetup setup;
		setup.about = "Adds a dynamic texture.";
		setup.category = { "Textures", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

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
		if(_source != 0) _value = titem->_value;
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
		_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
	}

	void mvDynamicTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_dirty)
		{

			_texture = LoadTextureFromArrayDynamic(_permWidth, _permHeight, _value->data());

			if (_texture == nullptr)
				_state.ok = false;

			_dirty = false;
			return;
		}

		UpdateTexture(_texture, _permWidth, _permHeight, *_value);

	}

	void mvDynamicTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_permWidth = ToInt(item);
				_width = _permWidth;
				break;

			case 1:
				_permHeight = ToInt(item);
				_height = _permHeight;
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