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

	// general
	void FreeTexture(void* texture);
	bool UnloadTexture(const std::string& filename);
	
	// static textures
	void* LoadTextureFromFile(const char* filename, int& width, int& height);
	void* LoadTextureFromArray(unsigned width, unsigned height, float* data);

	// dynamic textures
	void* LoadTextureFromArrayDynamic(unsigned width, unsigned height, float* data);
	void  UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<float>& data);

	// raw textures
	void* LoadTextureFromArrayRaw(unsigned width, unsigned height, float* data, int components);
	void  UpdateRawTexture(void* texture, unsigned width, unsigned height, float* data, int components);

}