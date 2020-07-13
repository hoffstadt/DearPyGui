#pragma once

#include <vector>
#include <string>

namespace Marvel {

	bool        LoadTextureFromFile(const char* filename, void* vout_srv, int* out_width, int* out_height);
	void        RunFile            (const std::string& name, const std::string& file, const std::string& flags = "");
	std::string OpenFile           (const std::vector<std::pair<std::string, std::string>>& extensions);
	std::string SaveFile           (const std::vector<std::pair<std::string, std::string>>& extensions);

}