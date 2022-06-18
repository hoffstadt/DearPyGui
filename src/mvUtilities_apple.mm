#include "mvUtilities.h"
#include "mvViewport.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>
#include <Quartz/Quartz.h>
#include <simd/simd.h>
#include <vector>
#include "mvAppleSpecifics.h"
#include "mvPythonExceptions.h"

// this is necessary to keep objective-c's reference counts
// from reaching 0.
static std::vector<std::pair<id<MTLTexture>, id<MTLTexture>>> g_textures;

mv_impl void
OutputFrameBufferArray(PymvBuffer* out)
{
    mvSubmitCallback([]() {mvThrowPythonError(mvErrorCode::mvNone, "`output_frame_buffer(...)` has not been implemented for this platform yet."); });
}

mv_impl void
OutputFrameBuffer(const char* filepath)
{
    mvSubmitCallback([](){mvThrowPythonError(mvErrorCode::mvNone, "`output_frame_buffer(...)` has not been implemented for this platform yet.");});
}

mv_impl void*
LoadTextureFromArray(unsigned width, unsigned height, float* data)
{
    mvGraphics& graphics = GContext->graphics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA32Float width:width height:height mipmapped:NO];

    textureDescriptor.usage = MTLTextureUsageShaderRead;
    textureDescriptor.storageMode = MTLStorageModeManaged;

    id <MTLTexture> texture = [graphicsData->device newTextureWithDescriptor:textureDescriptor];
    [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * 4 * 4];

    g_textures.push_back({texture, texture});

    return (__bridge void*)g_textures.back().second;
}

mv_impl void*
LoadTextureFromArrayDynamic(unsigned width, unsigned height, float* data)
{
    mvGraphics& graphics = GContext->graphics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA32Float width:width height:height mipmapped:NO];

    textureDescriptor.usage = MTLTextureUsageShaderRead;
    textureDescriptor.storageMode = MTLStorageModeManaged;

    id <MTLTexture> texture = [graphicsData->device newTextureWithDescriptor:textureDescriptor];
    [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * 4 * 4];

    g_textures.push_back({texture, texture});

    return (__bridge void*)g_textures.back().second;
}

mv_impl void*
LoadTextureFromArrayRaw(unsigned width, unsigned height, float* data, int components)
{
    mvGraphics& graphics = GContext->graphics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA32Float width:width height:height mipmapped:NO];

    textureDescriptor.usage = MTLTextureUsageShaderRead;
    textureDescriptor.storageMode = MTLStorageModeManaged;

    id <MTLTexture> texture = [graphicsData->device newTextureWithDescriptor:textureDescriptor];
    [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * 4 * 4];

    g_textures.push_back({texture, texture});

    return (__bridge void*)g_textures.back().second;
}

mv_impl void*
LoadTextureFromFile(const char* filename, int& width, int& height)
{
    mvGraphics& graphics = GContext->graphics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    unsigned char* image_data = stbi_load(filename, &width, &height, nullptr, 4);
    if (image_data == nullptr)
        return nullptr;

    MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                    width:width
                                                                                                height:height
                                                                                                mipmapped:NO];
    textureDescriptor.usage = MTLTextureUsageShaderRead;
    textureDescriptor.storageMode = MTLStorageModeManaged;

    id <MTLTexture> texture = [graphicsData->device newTextureWithDescriptor:textureDescriptor];
    [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:image_data bytesPerRow:width * 4];

    g_textures.push_back({texture, texture});

    return (__bridge void*)g_textures.back().second;
}

mv_impl bool
UnloadTexture(const std::string& filename)
{

	return true;
}

mv_impl void
FreeTexture(void* texture)
{
    id <MTLTexture> out_srv = (__bridge id <MTLTexture>)texture;

    std::vector<std::pair<id<MTLTexture>, id<MTLTexture>>> oldtextures = g_textures;
    g_textures.clear();
    for(auto& texturepair : oldtextures)
    {
        if(texturepair.first != out_srv)
            g_textures.push_back(texturepair);
    }

}

mv_impl void
UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<float>& data)
{
    id <MTLTexture> out_srv = (__bridge id <MTLTexture>)texture;
    [out_srv replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data.data() bytesPerRow:width * 4 * 4];
}

    void UpdateRawTexture(void* texture, unsigned width, unsigned height, float* data, int components)
{
    id <MTLTexture> out_srv = (__bridge id <MTLTexture>)texture;
    [out_srv replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * components * 4];
}