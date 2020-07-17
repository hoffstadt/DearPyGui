#pragma once

//-----------------------------------------------------------------------------
// mvDataStorage
//
//     - This class acts as a manager for data storage by the users. The 
//       data storage system serves 3 purposes:
//
//         * Allows some widgets to share the same underlying data.
//         * Allows some widgets secondary data be modified (i.e. listbox list items)
//         * Allows the user to store data for any other purpose where
//           the data can be retrieved at a later time.
//
//     - AddData is also used to update existing data. We probably need to add
//       an UpdataData method to make this explicit and come up with another
//       name for the current UpdateData method. The current UpdateData just
//       updates all items in mvApp (which should not be this class's
//       responsibility).
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
		int width;
		int height;
		void* texture;
		int count = 0;
	};

	//-----------------------------------------------------------------------------
	// mvTextureStorage
	//-----------------------------------------------------------------------------
	class mvTextureStorage
	{

	public:

		static void       AddTexture     (const std::string& name);
		static void       IncrementTexture(const std::string& name);
		static void       DecrementTexture(const std::string& name);
		static mvTexture* GetTexture  (const std::string& name);
		static unsigned   GetTextureCount();

	private:
		
		static std::map<std::string, mvTexture> s_textures;

	};

}