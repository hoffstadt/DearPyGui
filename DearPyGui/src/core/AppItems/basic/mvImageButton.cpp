#include "mvImageButton.h"
#include "mvTextureStorage.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvImageButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_image_button", mvPythonParser({
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::FloatList, "tint_color", "", "(255, 255, 255, 255)"},
			{mvPythonDataType::FloatList, "background_color", "", "(0, 0, 0, 0)"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::Integer, "frame_padding","", "-1"},
			{mvPythonDataType::FloatList, "uv_min", "normalized texture coordinates", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "uv_max", "normalized texture coordinates", "(1.0, 1.0)"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds an image button."
		"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown."
		"Using(0,0)->(1,1) texture coordinates will generally display the entire texture", "None", "Adding Widgets") });
	}

	mvImageButton::mvImageButton(const std::string& name, std::string  default_value)
		: mvAppItem(name), m_value(std::move(default_value))
	{
		m_description.ignoreSizeUpdate = true;
		m_description.disableAllowed = true;
		mvEventBus::Subscribe(this, mvEVT_DELETE_TEXTURE);
	}

	mvImageButton::~mvImageButton()
	{
		mvEventBus::Publish(mvEVT_CATEGORY_TEXTURE, mvEVT_DEC_TEXTURE, { CreateEventArgument("NAME", m_value) });
		mvEventBus::UnSubscribe(this);
	}

	bool mvImageButton::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvImageButton::onTextureDeleted), mvEVT_DELETE_TEXTURE);

		return event.handled;
	}

	bool mvImageButton::onTextureDeleted(mvEvent& event)
	{
		std::string name = GetEString(event, "NAME");
		
		if (name == m_value)
		{
			m_texture = nullptr;
			return true;
		}

		return false;
	}

	void mvImageButton::draw()
	{
		mvImGuiThemeScope scope(this);

		if (m_texture == nullptr && !m_value.empty())
		{
			mvApp::GetApp()->getTextureStorage().addTexture(m_value);
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_value);
			if (texture == nullptr)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
					{
						PyErr_Format(PyExc_Exception,
							"Image %s could not be found for add_image_button. Check the path to the image "
							"you provided.", m_value.c_str());
						PyErr_Print();
						m_value = "";
					});
				return;
			}
			if (m_core_config.width == 0) m_core_config.width = (int)((float)texture->width * (m_uv_max.x - m_uv_min.x));
			if (m_core_config.height == 0) m_core_config.height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));

			m_texture = texture->texture;
		}

		if (m_dirty)
		{
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_value);
			if (texture)
			{
				m_core_config.width = (int)((float)texture->width * (m_uv_max.x - m_uv_min.x));
				m_core_config.height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));
			}
			m_dirty = false;
		}

		if (m_texture)
		{
			ImGui::PushID(m_core_config.name.c_str());
			if (ImGui::ImageButton(m_texture, ImVec2((float)m_core_config.width, (float)m_core_config.height),
				ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y), m_framePadding,
				m_backgroundColor, m_tintColor))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);
			ImGui::PopID();
		}

	}

	void mvImageButton::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvImageButtonConfig*)config;

		m_core_config.width = config->width;
		m_core_config.height = config->height;
		m_core_config.show = config->show;
		m_core_config.callback = config->callback;
		m_core_config.callback_data = config->callback_data;

		m_config.source = aconfig->source;

		if (config != &m_config)
			m_config = *aconfig;
	}

	mvAppItemConfig* mvImageButton::getConfig()
	{
		return &m_config;
	}

	void mvImageButton::setExtraConfigDict(PyObject* dict)
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
		if (PyObject* item = PyDict_GetItemString(dict, "background_color")) m_backgroundColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "frame_padding")) m_framePadding = ToInt(item);
	}

	void mvImageButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "uv_min", ToPyPair(m_uv_min.x, m_uv_min.y));
		PyDict_SetItemString(dict, "uv_max", ToPyPair(m_uv_max.x, m_uv_max.y));
		PyDict_SetItemString(dict, "tint_color", ToPyColor(m_tintColor));
		PyDict_SetItemString(dict, "background_color", ToPyColor(m_backgroundColor));
		PyDict_SetItemString(dict, "frame_padding", ToPyInt(m_framePadding));
	}

	PyObject* mvImageButton::add_image_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* value;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(255.0));
		PyObject* backgroundColor = PyTuple_New(4);
		PyTuple_SetItem(backgroundColor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 3, PyFloat_FromDouble(0.0));
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int frame_padding = -1;
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));
		int show = true;
		int enabled = true;

		if (!(mvApp::GetApp()->getParsers())["add_image_button"].parse(args, kwargs, __FUNCTION__,
			&value, &name, &callback, &callback_data, &tintcolor, &backgroundColor, &parent,
			&before, &width, &height, &frame_padding, &uv_min, &uv_max, &show, &enabled))
			return ToPyBool(false);

		auto item = CreateRef<mvImageButton>(name, value);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}

}