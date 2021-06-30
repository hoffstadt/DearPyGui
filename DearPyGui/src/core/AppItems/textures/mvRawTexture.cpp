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
				m_value = buffer_info.buf;
				if (m_value == nullptr)
				{
					mvThrowPythonError(mvErrorCode::mvTextureNotFound, s_command, "Texture data not valid", this);
				}
			}
			PyBuffer_Release(&buffer_info);

			Py_XINCREF(value);
			m_buffer = value;
		}
	}

	mvRawTexture::~mvRawTexture()
	{
		FreeTexture(m_texture);
		
		mvGlobalIntepreterLock gil;
		Py_XDECREF(m_buffer);
	}


	bool mvRawTexture::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvTextureRegistry) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: mvTextureRegistry, mvStagingContainer.", this);

		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvRawTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_dirty)
		{

			if (m_value == nullptr)
				return;

			if(m_componentType == ComponentType::MV_FLOAT_COMPONENT)
				m_texture = LoadTextureFromArrayRaw(m_width, m_height, (float*)m_value, m_components);

			if (m_texture == nullptr)
				m_state.setOk(false);

			m_dirty = false;
			return;
		}

		if (m_componentType == ComponentType::MV_FLOAT_COMPONENT)
			UpdateRawTexture(m_texture, m_width, m_height, (float*)m_value, m_components);

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
				m_width = ToInt(item);
				break;

			case 1:
				m_height = ToInt(item);
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
				m_components = 4;
				m_componentType = mvRawTexture::ComponentType::MV_FLOAT_COMPONENT;
			}
			else if (format == 1)
			{
				m_components = 3;
				m_componentType = mvRawTexture::ComponentType::MV_FLOAT_COMPONENT;
			}
		}
	}

	void mvRawTexture::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}