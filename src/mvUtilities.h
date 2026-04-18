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
#include <imgui.h>      // Need this for ImTextureID
#include "mvTypes.h"

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

struct PymvBuffer;
struct ImDrawList;

// general
void FreeTexture(ImTextureID texture);
b8 UnloadTexture(const std::string& filename);

// Bracket ImDrawList::AddImage calls to render with nearest-neighbor sampling.
void EnterNearestFilterScope(ImDrawList* drawlist);
void LeaveNearestFilterScope(ImDrawList* drawlist);

// static textures
ImTextureID LoadTextureFromFile(const char* filename, i32& width, i32& height);
ImTextureID LoadTextureFromArray(u32 width, u32 height, f32* data, i32 filter = 0);

// dynamic textures
ImTextureID LoadTextureFromArrayDynamic(u32 width, u32 height, f32* data, i32 filter = 0);
void  UpdateTexture(ImTextureID texture, u32 width, u32 height, std::vector<f32>& data);

// raw textures
ImTextureID LoadTextureFromArrayRaw(u32 width, u32 height, f32* data, i32 components, i32 filter = 0);
void  UpdateRawTexture(ImTextureID texture, u32 width, u32 height, f32* data, i32 components);

// framebuffer output
void OutputFrameBuffer(const char* filepath);
void OutputFrameBufferArray(PymvBuffer* out);
