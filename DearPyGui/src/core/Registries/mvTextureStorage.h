#pragma once

//-----------------------------------------------------------------------------
// mvTextureStorage
//
//     - This class acts as a manager for texture storage. The 
//       texture storage system serves 2 purposes:
//
//         * Allows some image based widgets to share the same textures.
//         * Allows some textures to be automatically cleaned up using
//           a reference counting system.
//
//     - AddTexture will increment an existing texture if it already exists
//     
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <unordered_map>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTexture
	//-----------------------------------------------------------------------------
	struct mvTexture
	{
		int   width = 0;
		int   height = 0;
		void* texture = nullptr;
		int   count = 0;
	};

	enum class mvTextureFormat
	{
		RGBA_INT = 0u,
		RGBA_FLOAT,
		RGB_FLOAT,
		RGB_INT
	};

	//-----------------------------------------------------------------------------
	// mvTextureStorage
	//-----------------------------------------------------------------------------
	class mvTextureStorage
	{

	public:

		static void       AddTexture       (const std::string& name);
		static void       AddTexture       (const std::string& name, float* data, unsigned width, unsigned height, mvTextureFormat format);
		static void       IncrementTexture (const std::string& name);
		static void       DecrementTexture (const std::string& name);
		static mvTexture* GetTexture       (const std::string& name);
		static unsigned   GetTextureCount  ();
		static void       DeleteAllTextures();

	private:

		mvTextureStorage() = default;
		
		static std::unordered_map<std::string, mvTexture> s_textures;

	};

}