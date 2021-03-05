#include "mvTextureStorage.h"
#include "mvUtilities.h"
#include <imgui.h>
#include "mvApp.h"

namespace Marvel {

	mvTextureStorage::mvTextureStorage()
	{
		mvEventBus::Subscribe(this, mvEVT_FRAME);
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_TEXTURE);
	}

	mvTextureStorage::~mvTextureStorage()
	{
		mvEventBus::UnSubscribe(this);
	}

	bool mvTextureStorage::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvTextureStorage::onDecrement), mvEVT_DEC_TEXTURE);
		dispatcher.dispatch(BIND_EVENT_METH(mvTextureStorage::onFirstFrame), mvEVT_FRAME);

		return event.handled;
	}

	bool mvTextureStorage::onFirstFrame(mvEvent& event)
	{
		if (GetEInt(event, "FRAME") != 1)
			return false;

		for (auto& item : m_delayedTextures)
		{
			if (item.width > 0u)
				addTexture(item.name, item.data.data(), item.width, item.height, item.format);
			else
				addTexture(item.name);
		}

		m_delayedTextures.clear();
		
		return false;
	}

	bool mvTextureStorage::onDecrement(mvEvent& event)
	{
		decrementTexture(GetEString(event, "NAME"));
		return true;
	}

	void mvTextureStorage::deleteAllTextures()
	{
		for (auto& texture : m_textures)
			UnloadTexture(texture.first);

		m_textures.clear();
	}

	void mvTextureStorage::addTexture(const std::string& name)
	{
		// check if texture already exists and if it does
		// just increment its reference count
		mvTexture* texture = getTexture(name);
		if (texture)
		{
			incrementTexture(name);
			return;
		}

		if (name == "INTERNAL_DPG_FONT_ATLAS")
		{
			m_textures.insert({ name, {ImGui::GetIO().Fonts->TexWidth, ImGui::GetIO().Fonts->TexHeight, ImGui::GetIO().Fonts->TexID, 1} });
		}

		mvTexture newTexture = { 0, 0, nullptr, 1 };

		if (LoadTextureFromFile(name.c_str(), newTexture))
            m_textures.insert({ name, newTexture });

	}

	void mvTextureStorage::addTexture(const std::string& name, float* data, unsigned width, unsigned height, mvTextureFormat format)
	{
		// check if texture already exists and if it does
		// just increment its reference count
		mvTexture* texture = getTexture(name);
		if (texture)
		{
			texture->count = 1;
			decrementTexture(name);
		}

		mvTexture newTexture = { 0, 0, nullptr, 1 };

		if (LoadTextureFromArray(name.c_str(), data, width, height, newTexture, format))
			m_textures[name] = newTexture;

	}

	void mvTextureStorage::addDelayedTexture(const std::string& name)
	{
		m_delayedTextures.push_back({ name, {}, 0u, 0u });
	}

	void mvTextureStorage::addDelayedTexture(const std::string& name, const std::vector<float>& data, unsigned width, unsigned height, mvTextureFormat format)
	{
		//m_textures.emplace_back(name, data, width, height, format);
		m_delayedTextures.push_back({ name, data, width, height, format });
	}

	void mvTextureStorage::incrementTexture(const std::string& name)
	{
		// see if texture exists
		if (m_textures.count(name) == 0)
			return;

		m_textures.at(name).count++;

	}

	void mvTextureStorage::decrementTexture(const std::string& name)
	{
		// see if texture exists
		if (m_textures.count(name) == 0)
			return;

		m_textures.at(name).count--;

		// remove if count reaches 0
		if (m_textures.at(name).count == 0 && name != "INTERNAL_DPG_FONT_ATLAS")
		{
			UnloadTexture(name);
			FreeTexture(m_textures.at(name));
			m_textures.erase(name);

			mvEventBus::Publish(mvEVT_CATEGORY_TEXTURE, mvEVT_DELETE_TEXTURE, { CreateEventArgument("NAME", name) });
		}
	}

	mvTexture* mvTextureStorage::getTexture(const std::string& name)
	{
		if (m_textures.count(name) == 0)
			return nullptr;

		return &m_textures.at(name);
	}

	unsigned  mvTextureStorage::getTextureCount()
	{
		return (unsigned)m_textures.size();
	}

#ifdef MV_CPP
#else
	void AddTextureStorageCommands(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ "add_texture", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::IntList, "data", "RGBA format"},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "format", "mvTEX_XXXX_XXXXX constants", "0"},
		}, "Adds a texture. Incorrect format may yield unexpected results.") });

		parsers->insert({ "decrement_texture", mvPythonParser({
			{mvPythonDataType::String, "name"},
		}, "Decrements a texture.") });
	}

	PyObject* add_texture(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		// When using RGBA python data is texture data
		// When using RGB we must divide out the increment to stay in bouds of the shorter python data
		// When using BGRA python data must be reordered to match RGBA
		// When using BGR we must divide out the increment to stay in bouds of the shorter python data and also reorder

		const char* name;
		PyObject* data;
		int width;
		int height;
		int format = 0;

		if (!(*mvApp::GetApp()->getParsers())["add_texture"].parse(args, kwargs, __FUNCTION__,
			&name, &data, &width, &height, &format))
			return GetPyNone();

		mvTextureFormat tformat = (mvTextureFormat)format;

		if (PyList_Check(data)) {

			std::vector<float> fdata;
			fdata.resize(width * height * 4);
			switch (tformat)
			{
			case mvTextureFormat::RGBA_FLOAT:
				for (size_t i = 0; i < fdata.size(); ++i)
				{
					fdata[i] = PyFloat_AsDouble(PyList_GetItem(data, i));
				}
				break;

			case mvTextureFormat::RGBA_INT:
				for (size_t i = 0; i < fdata.size(); ++i)
				{
					fdata[i] = PyFloat_AsDouble(PyList_GetItem(data, i)) / 255.0f;
				}
				break;

			case mvTextureFormat::BGRA_FLOAT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = PyLong_AsLong(PyList_GetItem(data, i + 2));
					fdata[i + 1] = PyLong_AsLong(PyList_GetItem(data, i + 1));
					fdata[i + 2] = PyLong_AsLong(PyList_GetItem(data, i));
					fdata[i + 3] = PyLong_AsLong(PyList_GetItem(data, i + 3));
				}
				break;

			case mvTextureFormat::BGRA_INT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = PyLong_AsLong(PyList_GetItem(data, i + 2)) / 255.0f;
					fdata[i + 1] = PyLong_AsLong(PyList_GetItem(data, i + 1)) / 255.0f;
					fdata[i + 2] = PyLong_AsLong(PyList_GetItem(data, i)) / 255.0f;
					fdata[i + 3] = PyLong_AsLong(PyList_GetItem(data, i + 3)) / 255.0f;
				}
				break;

			case mvTextureFormat::RGB_FLOAT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = PyFloat_AsDouble(PyList_GetItem(data, (i / 4) * 3));
					fdata[i + 1] = PyFloat_AsDouble(PyList_GetItem(data, (i / 4) * 3 + 1));
					fdata[i + 2] = PyFloat_AsDouble(PyList_GetItem(data, (i / 4) * 3 + 2));
					fdata[i + 3] = 1.0f;
				}
				break;

			case mvTextureFormat::RGB_INT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = PyLong_AsLong(PyList_GetItem(data, (i / 4) * 3)) / 255.0f;
					fdata[i + 1] = PyLong_AsLong(PyList_GetItem(data, (i / 4) * 3 + 1)) / 255.0f;
					fdata[i + 2] = PyLong_AsLong(PyList_GetItem(data, (i / 4) * 3 + 2)) / 255.0f;
					fdata[i + 3] = 1.0f;
				}
				break;

			case mvTextureFormat::BGR_FLOAT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = PyFloat_AsDouble(PyList_GetItem(data, (i / 4) * 3 + 2));
					fdata[i + 1] = PyFloat_AsDouble(PyList_GetItem(data, (i / 4) * 3 + 1));
					fdata[i + 2] = PyFloat_AsDouble(PyList_GetItem(data, (i / 4) * 3));
					fdata[i + 3] = 1.0f;
				}
				break;

			case mvTextureFormat::BGR_INT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = PyLong_AsLong(PyList_GetItem(data, (i / 4) * 3 + 2)) / 255.0f;
					fdata[i + 1] = PyLong_AsLong(PyList_GetItem(data, (i / 4) * 3 + 1)) / 255.0f;
					fdata[i + 2] = PyLong_AsLong(PyList_GetItem(data, (i / 4) * 3)) / 255.0f;
					fdata[i + 3] = 1.0f;
				}
				break;

			default:
				return GetPyNone();
			}

			std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
			if (mvApp::IsAppStarted())
				mvApp::GetApp()->getTextureStorage().addTexture(name, fdata.data(), width, height, mvTextureFormat::BGRA_FLOAT);
			else
				mvApp::GetApp()->getTextureStorage().addDelayedTexture(name, fdata, width, height, mvTextureFormat::RGBA_FLOAT);

			return GetPyNone();
		}

		else if (PyObject_CheckBuffer(data))
		{
			Py_buffer buffer_info;

			if (PyObject_GetBuffer(data, &buffer_info,
				PyBUF_CONTIG_RO | PyBUF_FORMAT))
			{
				PyBuffer_Release(&buffer_info);
				return GetPyNone();
			}

			std::vector<float> fdata;
			fdata.resize(buffer_info.len / buffer_info.itemsize);

			//This will choose which function to use to interpretate the array. 
			//Each function interpretates it as a different type.

			auto BufferViewer = BufferViewFunctions(buffer_info);

			switch (tformat)
			{
			case mvTextureFormat::RGBA_FLOAT:
				for (size_t i = 0; i < fdata.size(); ++i)
					fdata[i] = BufferViewer(buffer_info, i);
				break;

			case mvTextureFormat::RGBA_INT:
				for (size_t i = 0; i < fdata.size(); ++i)
					fdata[i] = BufferViewer(buffer_info, i) / 255.0f;
				break;

			case mvTextureFormat::BGRA_FLOAT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = BufferViewer(buffer_info, i + 2);
					fdata[i + 1] = BufferViewer(buffer_info, i + 1);
					fdata[i + 2] = BufferViewer(buffer_info, i);
					fdata[i + 3] = BufferViewer(buffer_info, i + 3);
				}
				break;

			case mvTextureFormat::BGRA_INT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = BufferViewer(buffer_info, i + 2) / 255.0f;
					fdata[i + 1] = BufferViewer(buffer_info, i + 1) / 255.0f;
					fdata[i + 2] = BufferViewer(buffer_info, i) / 255.0f;
					fdata[i + 3] = BufferViewer(buffer_info, i + 3) / 255.0f;
				}
				break;

			case mvTextureFormat::RGB_FLOAT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = BufferViewer(buffer_info, (i / 4) * 3);
					fdata[i + 1] = BufferViewer(buffer_info, (i / 4) * 3 + 1);
					fdata[i + 2] = BufferViewer(buffer_info, (i / 4) * 3 + 2);
					fdata[i + 3] = 1.0f;
				}
				break;

			case mvTextureFormat::RGB_INT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = BufferViewer(buffer_info, (i / 4) * 3) / 255.0f;
					fdata[i + 1] = BufferViewer(buffer_info, (i / 4) * 3 + 1) / 255.0f;
					fdata[i + 2] = BufferViewer(buffer_info, (i / 4) * 3 + 2) / 255.0f;
					fdata[i + 3] = 1.0f;
				}
				break;

			case mvTextureFormat::BGR_FLOAT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = BufferViewer(buffer_info, (i / 4) * 3 + 2);
					fdata[i + 1] = BufferViewer(buffer_info, (i / 4) * 3 + 1);
					fdata[i + 2] = BufferViewer(buffer_info, (i / 4) * 3);
					fdata[i + 3] = 1.0f;
				}
				break;

			case mvTextureFormat::BGR_INT:
				for (size_t i = 0; i < fdata.size(); i += 4)
				{
					fdata[i] = BufferViewer(buffer_info, (i / 4) * 3 + 2) / 255.0f;
					fdata[i + 1] = BufferViewer(buffer_info, (i / 4) * 3 + 1) / 255.0f;
					fdata[i + 2] = BufferViewer(buffer_info, (i / 4) * 3) / 255.0f;
					fdata[i + 3] = 1.0f;
				}
				break;

			default:
				PyBuffer_Release(&buffer_info);
				return GetPyNone();
			}

			std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
			if (mvApp::IsAppStarted())
				mvApp::GetApp()->getTextureStorage().addTexture(name, fdata.data(), width, height, tformat);
			else
				mvApp::GetApp()->getTextureStorage().addDelayedTexture(name, fdata, width, height, tformat);

			PyBuffer_Release(&buffer_info);
			return GetPyNone();
		}

		else
		{
			return GetPyNone();
		}

	}

	PyObject* decrement_texture(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["decrement_texture"].parse(args, kwargs, __FUNCTION__,
			&name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvEventBus::PublishEndFrame(mvEVT_CATEGORY_TEXTURE, mvEVT_DEC_TEXTURE, { CreateEventArgument("NAME", std::string(name)) });

		return GetPyNone();
	}
#endif
}