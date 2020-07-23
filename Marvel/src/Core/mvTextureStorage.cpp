#include "mvTextureStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include <thread>
#include "Core/mvUtilities.h"

namespace Marvel {

	std::map<std::string, mvTexture> mvTextureStorage::s_textures;

	void mvTextureStorage::DeleteAllTextures()
	{
		for (auto& texture : s_textures)
			UnloadTexture(texture.second.texture);

		s_textures.clear();
	}

	void mvTextureStorage::AddTexture(const std::string& name)
	{
		// check if texture already exists and if it does
		// just increment its reference count
		mvTexture* texture = GetTexture(name);
		if (texture)
		{
			IncrementTexture(name);
			return;
		}

		mvTexture newTexture = { 0, 0, nullptr, 1 };

		if (LoadTextureFromFile(name.c_str(), &newTexture.texture, &newTexture.width, &newTexture.height))
			s_textures.insert({ name, newTexture });

	}

	void mvTextureStorage::IncrementTexture(const std::string& name)
	{
		// see if texture exists
		if (s_textures.count(name) == 0)
			return;

		s_textures.at(name).count++;

	}

	void mvTextureStorage::DecrementTexture(const std::string& name)
	{
		// see if texture exists
		if (s_textures.count(name) == 0)
			return;

		s_textures.at(name).count--;

		// remove if count reaches 0
		if (s_textures.at(name).count == 0)
		{
			UnloadTexture(s_textures.at(name).texture);
			s_textures.erase(name);
		}
	}

	mvTexture* mvTextureStorage::GetTexture(const std::string& name)
	{

		if (s_textures.count(name) == 0)
			return nullptr;

		return &s_textures.at(name);
	}

	unsigned  mvTextureStorage::GetTextureCount()
	{
		return s_textures.size();
	}

}