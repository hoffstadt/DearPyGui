#include "mvDynamicTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvDynamicTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.addArg<mvPyDataType::Integer>("width");
		parser.addArg<mvPyDataType::Integer>("height");
		parser.addArg<mvPyDataType::FloatList>("default_value");
		parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_2", "Parent to add this item to. (runtime adding)");
		parser.finalize();

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

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = std::get<std::shared_ptr<std::vector<float>>>(item->getValue());
	}

	void mvDynamicTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_dirty)
		{

			_texture = LoadTextureFromArrayDynamic(_permWidth, _permHeight, _value->data());

			if (_texture == nullptr)
				_state.setOk(false);

			_dirty = false;
			return;
		}

		UpdateTexture(_texture, _permWidth, _permHeight, *_value);

	}

	void mvDynamicTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
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