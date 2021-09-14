#include "mvStaticTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include <fstream>

namespace Marvel {

	void mvStaticTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
		setup.about = "Adds a static texture.";
		setup.category = { "Textures", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvStaticTexture::mvStaticTexture(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	mvStaticTexture::~mvStaticTexture()
	{
		if (_uuid == MV_ATLAS_UUID)
			return;
		//UnloadTexture(_name);
		FreeTexture(_texture);
	}

	void mvStaticTexture::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvStaticTexture*>(item);
		if(_source != 0) _value = titem->_value;
		_texture = titem->_texture;
		_dirty = titem->_dirty;
		_permWidth = titem->_permWidth;
		_permHeight = titem->_permHeight;
	}

	void mvStaticTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!_dirty)
			return;

		if (!_state.isOk())
			return;

		if (_uuid == MV_ATLAS_UUID)
		{
			_texture = ImGui::GetIO().Fonts->TexID;
			_width = ImGui::GetIO().Fonts->TexWidth;
			_height = ImGui::GetIO().Fonts->TexHeight;
		}
		else
			_texture = LoadTextureFromArray(_permWidth, _permHeight, _value->data());

		if (_texture == nullptr)
		{
			_state.setOk(false);
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "add_static_texture",
				"Texture data can not be found.", this);
		}


		_dirty = false;
	}

	void mvStaticTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(mvApp::GetApp()->getParsers()[s_command], dict))
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

	PyObject* mvStaticTexture::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvStaticTexture::setPyValue(PyObject* value)
	{
		*_value = ToFloatVect(value);
	}

	void mvStaticTexture::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
	}

}