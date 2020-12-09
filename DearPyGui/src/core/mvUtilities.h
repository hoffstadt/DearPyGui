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

	bool        UnloadTexture       (const std::string& filename);
	bool        LoadTextureFromArray(const char* name, float* data, unsigned width, unsigned height, mvTexture& storage, mvTextureFormat format);
	bool        LoadTextureFromFile (const char* filename, mvTexture& storage);
	void        FreeTexture         (mvTexture& storage);

}