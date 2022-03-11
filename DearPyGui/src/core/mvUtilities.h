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
#include "mvTypes.h"

enum Filtering{
    LINEAR = 4,
    NEAREST = 3,
};

// general
void FreeTexture(void* texture);
b8 UnloadTexture(const std::string& filename);
	
// static textures
void* LoadTextureFromFile(const char* filename, i32& width, i32& height, i32 filtering);
void* LoadTextureFromArray(u32 width, u32 height, f32* data, i32 filtering);

// dynamic textures
void* LoadTextureFromArrayDynamic(u32 width, u32 height, f32* data, i32 filtering);
void  UpdateTexture(void* texture, u32 width, u32 height, std::vector<f32>& data, i32 filtering);

// raw textures
void* LoadTextureFromArrayRaw(u32 width, u32 height, f32* data, i32 components, i32 filtering);
void  UpdateRawTexture(void* texture, u32 width, u32 height, f32* data, i32 components, i32 filtering);

// framebuffer output
void OutputFrameBuffer(const char* filepath);
