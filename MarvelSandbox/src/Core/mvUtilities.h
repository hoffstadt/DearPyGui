#pragma once

#include <vector>
#include <string>

namespace Marvel {

	bool LoadTextureFromFile(const char* filename, void* vout_srv, int* out_width, int* out_height);

	std::string OpenFile(std::vector<std::pair<std::string, std::string>>& extensions);
	std::string SaveFile(std::vector<std::pair<std::string, std::string>>& extensions);

}