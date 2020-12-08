#include "mvTextureStorage.h"
#include "core/mvUtilities.h"
#include <imgui.h>

namespace Marvel {

	mvTextureStorage* mvTextureStorage::s_instance = nullptr;

	mvTextureStorage* mvTextureStorage::GetTextureStorage()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvTextureStorage();
		return s_instance;
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