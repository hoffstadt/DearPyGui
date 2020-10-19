#include "mvTextureStorage.h"
#include "Core/mvUtilities.h"
#include <imgui.h>

namespace Marvel {

	std::map<std::string, mvTexture> mvTextureStorage::s_textures;

	void mvTextureStorage::DeleteAllTextures()
	{
		for (auto& texture : s_textures)
			UnloadTexture(texture.first);

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

		if (name == "INTERNAL_DPG_FONT_ATLAS")
		{
			s_textures.insert({ name, {ImGui::GetIO().Fonts->TexWidth, ImGui::GetIO().Fonts->TexHeight, ImGui::GetIO().Fonts->TexID, 1} });
		}

		mvTexture newTexture = { 0, 0, nullptr, 1 };

		if (LoadTextureFromFile(name.c_str(), newTexture))
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
		if (s_textures.at(name).count == 0 && name != "INTERNAL_DPG_FONT_ATLAS")
		{
			UnloadTexture(name);
			FreeTexture(s_textures.at(name));
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
		return (unsigned)s_textures.size();
	}

}