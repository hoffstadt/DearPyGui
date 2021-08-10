#include "mvRawTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	void mvRawTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.addArg<mvPyDataType::Integer>("width");
		parser.addArg<mvPyDataType::Integer>("height");
		parser.addArg<mvPyDataType::FloatList>("default_value");
		parser.addArg<mvPyDataType::Integer>("format", mvArgType::KEYWORD_ARG, "internal_dpg.mvFormat_Float_rgba", "Data format.");
		parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_2", "Parent to add this item to. (runtime adding)");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvRawTexture::mvRawTexture(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}


	PyObject* mvRawTexture::getPyValue()
	{
		return GetPyNone();
	}

	void mvRawTexture::setPyValue(PyObject* value)
	{

		if (value == nullptr)
			return;

		if (PyObject_CheckBuffer(value))
		{
			Py_buffer buffer_info;

			if (!PyObject_GetBuffer(value, &buffer_info,
				PyBUF_CONTIG_RO | PyBUF_FORMAT))
			{
				_value = buffer_info.buf;
				if (_value == nullptr)
				{
					mvThrowPythonError(mvErrorCode::mvTextureNotFound, s_command, "Texture data not valid", this);
				}
			}
			PyBuffer_Release(&buffer_info);
			if (_buffer)
				Py_XDECREF(_buffer);
			Py_XINCREF(value);
			_buffer = value;
		}
	}

	mvRawTexture::~mvRawTexture()
	{
		FreeTexture(_texture);
		
		mvGlobalIntepreterLock gil;
		Py_XDECREF(_buffer);
	}

	void mvRawTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_dirty)
		{

			if (_value == nullptr)
				return;

			if(_componentType == ComponentType::MV_FLOAT_COMPONENT)
				_texture = LoadTextureFromArrayRaw(_permWidth, _permHeight, (float*)_value, _components);

			if (_texture == nullptr)
				_state.setOk(false);

			_dirty = false;
			return;
		}

		if (_componentType == ComponentType::MV_FLOAT_COMPONENT)
			UpdateRawTexture(_texture, _permWidth, _permHeight, (float*)_value, _components);

	}

	void mvRawTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
		{
			mvThrowPythonError(mvErrorCode::mvTextureNotFound, s_command, "Texture data not valid", this);
			return;
		}

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
				setPyValue(item);
				break;

			default:
				break;
			}
		}
	}

	void mvRawTexture::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "format"))
		{
			int format = ToInt(item);
			if (format == 0)
			{
				_components = 4;
				_componentType = mvRawTexture::ComponentType::MV_FLOAT_COMPONENT;
			}
			else if (format == 1)
			{
				_components = 3;
				_componentType = mvRawTexture::ComponentType::MV_FLOAT_COMPONENT;
			}
		}
	}

	void mvRawTexture::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}