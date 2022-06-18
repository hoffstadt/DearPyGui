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

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

struct PymvBuffer;

// general
void FreeTexture(void* texture);
b8 UnloadTexture(const std::string& filename);
	
// static textures
void* LoadTextureFromFile(const char* filename, i32& width, i32& height);
void* LoadTextureFromArray(u32 width, u32 height, f32* data);

// dynamic textures
void* LoadTextureFromArrayDynamic(u32 width, u32 height, f32* data);
void  UpdateTexture(void* texture, u32 width, u32 height, std::vector<f32>& data);

// raw textures
void* LoadTextureFromArrayRaw(u32 width, u32 height, f32* data, i32 components);
void  UpdateRawTexture(void* texture, u32 width, u32 height, f32* data, i32 components);

// framebuffer output
void OutputFrameBuffer(const char* filepath);
void OutputFrameBufferArray(PymvBuffer* out);
