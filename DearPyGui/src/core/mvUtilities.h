#pragma once

//-----------------------------------------------------------------------------
// mvUtilities
//
//     - This file contains typically platform specific functions. May need
//       to rename to a more appropriate name.
//     
//-----------------------------------------------------------------------------

#include <vector>
#include <string>
#include "mvTextureStorage.h"

namespace Marvel {

	bool        UnloadTexture      (const std::string& filename);
	bool        LoadTextureFromFile(const char* filename, mvTexture& storage);
	void        FreeTexture        (mvTexture& storage);

}