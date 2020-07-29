#include "Core/mvUtilities.h"


namespace Marvel {


    // Simple helper function to load an image into a DX11 texture with common settings
    bool LoadTextureFromFile(const char* filename, void* vout_srv, int* out_width, int* out_height)
    {
        return true;
    }

	bool UnloadTexture(void* texture)
	{
		// TODO : decide if cleanup is necessary
		return true;
	}

	void RunFile(const std::string& file, const std::string& flags)
	{

	}

	std::string PickDirectory(const std::string& directory)
	{
		return "";
	}

	std::string SaveFile(const std::vector<std::pair<std::string, std::string >>& extensions)
	{

		return "";
	}

	std::string OpenFile(const std::vector<std::pair<std::string, std::string>>& extensions)
	{
		return "";
	}

}