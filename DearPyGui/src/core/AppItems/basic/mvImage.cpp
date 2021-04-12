#include "mvImage.h"
#include "mvTextureStorage.h"
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvImage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::String>("value", mvArgType::OPTIONAL_ARG, "''");
		
		parser.addArg<mvPyDataType::FloatList>("tint_color", mvArgType::KEYWORD, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::FloatList>("border_color", mvArgType::KEYWORD, "(0, 0, 0, 0)");
		parser.addArg<mvPyDataType::FloatList>("uv_min", mvArgType::KEYWORD, "(0.0, 0.0)", "normalized texture coordinates");
		parser.addArg<mvPyDataType::FloatList>("uv_max", mvArgType::KEYWORD, "(1.0, 1.0)", "normalized texture coordinates");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvImage::mvImage(const std::string& name)
		: mvAppItem(name)
	{
		mvEventBus::Subscribe(this, mvEVT_DELETE_TEXTURE);
	}

	mvImage::~mvImage()
	{
		mvEventBus::Publish(mvEVT_CATEGORY_TEXTURE, mvEVT_DEC_TEXTURE, { CreateEventArgument("NAME", m_value) });
		mvEventBus::UnSubscribe(this);
	}

	bool mvImage::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvImage::onTextureDeleted), mvEVT_DELETE_TEXTURE);

		return event.handled;
	}

	bool mvImage::onTextureDeleted(mvEvent& event)
	{
		std::string name = GetEString(event, "NAME");

		if (name == m_value)
		{
			m_texture = nullptr;
			return true;
		}

		return false;
	}

	void mvImage::draw(ImDrawList* drawlist, float x, float y)
	{

		if (m_texture == nullptr && !m_value.empty())
		{
			mvApp::GetApp()->getTextureStorage().addTexture(m_value);
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_value);
			if (texture == nullptr)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
					{
						PyErr_Format(PyExc_Exception,
							"Image %s could not be found for add_image. Check the path to the image "
							"you provided.", m_value.c_str());
						PyErr_Print();
						m_value = "";
					});
				return;
			}
			
			m_texture = texture->texture;

			if (m_width == 0) m_width = (int)((float)texture->width * (m_uv_max.x - m_uv_min.x));
			if (m_height == 0) m_height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));

		}

		if (m_dirty)
		{
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_value);
			if (texture)
			{
				m_texture = texture->texture;
				m_width = (int)((float)texture->width * (m_uv_max.x - m_uv_min.x));
				m_height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));
			}
			m_dirty = false;
		}

		if (m_texture)
		{
			ImGui::Image(m_texture, ImVec2((float)m_width, (float)m_height), ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y),
				ImVec4((float)m_tintColor.r, (float)m_tintColor.g, (float)m_tintColor.b, (float)m_tintColor.a),
				ImVec4((float)m_borderColor.r, (float)m_borderColor.g, (float)m_borderColor.b, (float)m_borderColor.a));

		}

	}

	void mvImage::setValue(const std::string& value) 
	{ 
		m_value = value; 
	}

	const std::string& mvImage::getValue() const 
	{ 
		return m_value; 
	}

	void mvImage::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "uv_min"))
		{
			m_uv_min = ToVec2(item);
			m_dirty = true;
		}
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max"))
		{
			m_uv_max = ToVec2(item);
			m_dirty = true;
		}
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) m_tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "border_color")) m_borderColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "value")) m_value = ToString(item);
	}

	void mvImage::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", ToPyPair(m_uv_min.x, m_uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(m_uv_max.x, m_uv_max.y));
		PyDict_SetItemString(dict, "tint_color", ToPyColor(m_tintColor));
		PyDict_SetItemString(dict, "border_color", ToPyColor(m_borderColor));
		PyDict_SetItemString(dict, "value", ToPyString(m_value));
	}

}