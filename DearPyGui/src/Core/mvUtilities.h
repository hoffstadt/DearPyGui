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

namespace Marvel {

	bool        UnloadTexture      (void* texture);
	bool        LoadTextureFromFile(const char* filename, void* vout_srv, int* out_width, int* out_height);
	void        RunFile            (const std::string& file, const std::string& flags);
	std::string OpenFile           (const std::vector<std::pair<std::string, std::string>>& extensions);
	std::string SaveFile           (const std::vector<std::pair<std::string, std::string>>& extensions);
	std::string PickDirectory      (const std::string& directory);

}