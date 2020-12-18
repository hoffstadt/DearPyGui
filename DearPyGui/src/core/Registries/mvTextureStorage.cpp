#include "mvTextureStorage.h"
#include "mvUtilities.h"
#include <imgui.h>

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
			UnloadTexture(name);
			FreeTexture(m_textures.at(name));
			m_textures.erase(name);
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

}