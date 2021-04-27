#include "mvStaticTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvStaticTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Textures", "Widgets" });
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

		parser.addArg<mvPyDataType::String>("file", mvArgType::KEYWORD_ARG, "''");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvStaticTexture::mvStaticTexture(const std::string& name)
		:
		mvFloatVectPtrBase(name)
	{

	}

	mvStaticTexture::~mvStaticTexture()
	{
		if (m_name == "INTERNAL_DPG_FONT_ATLAS")
			return;
		UnloadTexture(m_name);
		FreeTexture(m_texture);
	}


	bool mvStaticTexture::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTextureContainer) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvStaticTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!m_dirty)
			return;

		if (m_name == "INTERNAL_DPG_FONT_ATLAS")
		{
			m_texture = ImGui::GetIO().Fonts->TexID;
			m_width = ImGui::GetIO().Fonts->TexWidth;
			m_height = ImGui::GetIO().Fonts->TexHeight;
		}
		else if (!m_file.empty())
			m_texture = LoadTextureFromFile(m_file.c_str(), m_width, m_height);
		else
			m_texture = LoadTextureFromArray(m_width, m_height, m_value->data());

		if (m_texture == nullptr)
			m_state.setOk(false);


		m_dirty = false;
	}

	void mvStaticTexture::handleSpecificRequiredArgs(PyObject* dict)
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

	void mvStaticTexture::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "file")) m_file = ToString(item);
	}

	void mvStaticTexture::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}