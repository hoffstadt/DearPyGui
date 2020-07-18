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
#include <map>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTexture
	//-----------------------------------------------------------------------------
	struct mvTexture
	{
		int   width;
		int   height;
		void* texture;
		int   count = 0;
	};

	//-----------------------------------------------------------------------------
	// mvTextureStorage
	//-----------------------------------------------------------------------------
	class mvTextureStorage
	{

	public:

		static void       AddTexture       (const std::string& name);
		static void       IncrementTexture (const std::string& name);
		static void       DecrementTexture (const std::string& name);
		static mvTexture* GetTexture       (const std::string& name);
		static unsigned   GetTextureCount  ();
		static void       DeleteAllTextures();

	private:

		mvTextureStorage() = default;
		
		static std::map<std::string, mvTexture> s_textures;

	};

}