#include "mvDrawImage.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0.0,0.0)->(1.0,1.0) texturecoordinates will generally display the entire texture.", { "Textures", "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::UUID>("texture_id");
		parser.addArg<mvPyDataType::FloatList>("pmin");
		parser.addArg<mvPyDataType::FloatList>("pmax");

		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "normalized texture coordinates");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawImage::mvDrawImage(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvDrawImage::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvDrawlist) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvPlot) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;
		if (type == mvAppItemType::mvViewportDrawlist) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: staging container, drawlist, layer, window, plot, viewport drawlist.", this);

		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawImage::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec2 start = { x, y };

		if (m_texture)
		{
			if (m_internalTexture)
				m_texture->draw(drawlist, x, y);

			if (!m_texture->getState().isOk())
				return;

			void* texture = nullptr;

			if (m_texture->getType() == mvAppItemType::mvStaticTexture)
				texture = static_cast<mvStaticTexture*>(m_texture.get())->getRawTexture();
			else
				texture = static_cast<mvDynamicTexture*>(m_texture.get())->getRawTexture();
			drawlist->AddImage(texture, m_pmin + start, m_pmax + start, m_uv_min, m_uv_max, m_color);
		}
	}

	void mvDrawImage::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				m_textureUUID = ToUUID(item);
				m_texture = mvApp::GetApp()->getItemRegistry().getRefItem(m_textureUUID);
				if (m_texture)
					break;
				else if (m_textureUUID == MV_ATLAS_UUID)
				{
					m_texture = std::make_shared<mvStaticTexture>(m_textureUUID);
					m_internalTexture = true;
					break;
				}
				else
				{
					mvThrowPythonError(mvErrorCode::mvTextureNotFound, s_command, "Texture not found.", this);
					break;
				}
			}

			case 1:
				m_pmin = ToVec2(item);
				break;

			case 2:
				m_pmax = ToVec2(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawImage::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) m_pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) m_pmin = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) m_uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) m_uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);

	}

	void mvDrawImage::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "pmax", ToPyPair(m_pmax.x, m_pmax.y));
		PyDict_SetItemString(dict, "pmin", ToPyPair(m_pmin.x, m_pmin.y));
		PyDict_SetItemString(dict, "uv_min", ToPyPair(m_uv_min.x, m_uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(m_uv_max.x, m_uv_max.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "texture_id", ToPyUUID(m_textureUUID));
	}

}