#include "mvTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::None);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("before");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("show");
		parser.removeArg("enabled");
		parser.removeArg("width");
		parser.removeArg("height");

		parser.addArg<mvPyDataType::Integer>("width");
		parser.addArg<mvPyDataType::Integer>("height");
		parser.addArg<mvPyDataType::FloatList>("default_value");

		parser.addArg<mvPyDataType::Bool>("dynamic", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTexture::mvTexture(const std::string& name)
		:
		mvFloatVectPtrBase(name)
	{

	}

	mvTexture::~mvTexture()
	{
		//UnloadTexture(m_name);
		FreeTexture(m_texture);
	}


	bool mvTexture::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTextureContainer) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!m_dirty && !m_dynamic)
			return;

		if (m_name == "INTERNAL_DPG_FONT_ATLAS")
		{
			m_texture = ImGui::GetIO().Fonts->TexID;
			m_width = ImGui::GetIO().Fonts->TexWidth;
			m_height = ImGui::GetIO().Fonts->TexHeight;
		}
		else if(m_dirty && !m_dynamic)
			m_texture = LoadTextureFromArray(m_width, m_height, m_value->data());
		else if (m_dirty)
			m_texture = LoadTextureFromArrayDynamic(m_width, m_height, m_value->data());
		else if (m_dynamic)
		{
			UpdateTexture(m_texture, m_width, m_height, *m_value);
		}

		m_dirty = false;
	}

	void mvTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

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
				*m_value = ToFloatVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvTexture::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "dynamic")) m_dynamic = ToBool(item);

	}

	void mvTexture::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}


}