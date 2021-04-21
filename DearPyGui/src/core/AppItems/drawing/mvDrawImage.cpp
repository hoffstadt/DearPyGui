#include "mvDrawImage.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvPythonExceptions.h"
#include "textures/mvTexture.h"

namespace Marvel {

	void mvDrawImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::String>("file");
		parser.addArg<mvPyDataType::FloatList>("pmin");
		parser.addArg<mvPyDataType::FloatList>("pmax");

		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "normalized texture coordinates");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawImage::mvDrawImage(const std::string& name)
		:
		mvAppItem(name)
	{
	}


	bool mvDrawImage::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawImage::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec2 start = { x, y };

		if (m_texture)
		{
			void* texture = static_cast<mvTexture*>(m_texture.get())->getRawTexture();
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
				m_file = ToString(item);
				m_texture = mvApp::GetApp()->getItemRegistry().getItem(m_file);
				break;
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

		if (PyObject* item = PyDict_GetItemString(dict, "file"))
		{
			m_file = ToString(item);

		}

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
		PyDict_SetItemString(dict, "file", ToPyString(m_file));
	}

}