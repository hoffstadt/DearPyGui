#include "mvDrawImage.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvTextureStorage.h"
#include "mvApp.h"
#include "mvPythonExceptions.h"

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
		mvEventBus::Subscribe(this, mvEVT_DELETE_TEXTURE);
	}

	mvDrawImage::~mvDrawImage()
	{
		mvEventBus::Publish(mvEVT_CATEGORY_TEXTURE, mvEVT_DEC_TEXTURE, { CreateEventArgument("NAME", m_file) });
		mvEventBus::UnSubscribe(this);
	}

	bool mvDrawImage::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvDrawImage::onTextureDeleted), mvEVT_DELETE_TEXTURE);

		return event.handled;
	}

	bool mvDrawImage::onTextureDeleted(mvEvent& event)
	{
		std::string name = GetEString(event, "NAME");

		if (name == m_file)
		{
			m_texture = nullptr;
			return false;
		}

		return false;
	}

	bool mvDrawImage::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawImage::draw(ImDrawList* drawlist, float x, float y)
	{

		if (m_texture == nullptr && !m_file.empty())
		{

			mvApp::GetApp()->getTextureStorage().addTexture(m_file);
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_file);
			if (texture == nullptr)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
					{
						PyErr_Format(PyExc_Exception,
							"Image %s could not be found for draw_image. Check the path to the image "
							"you provided.", m_file.c_str());
						PyErr_Print();
						m_file = "";
					});
				return;
			}

			m_texture = texture->texture;

			if (m_width == 0) m_width = (int)((float)texture->width * (m_uv_max.x - m_uv_min.x));
			if (m_height == 0) m_height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));

		}

		mvVec2 start = { x, y };

		if (m_texture)
			drawlist->AddImage(m_texture, m_pmin + start, m_pmax + start, m_uv_min, m_uv_max, m_color);
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
			if (m_file != ToString(item))
			{
				mvApp::GetApp()->getTextureStorage().decrementTexture(m_file);
				m_texture = nullptr;
			}
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