#include "mvUtilities.h"
#include "mvAppleViewport.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>
#include <Quartz/Quartz.h>
#include <simd/simd.h>
#include <vector>

// this is necessary to keep objective-c's reference counts
// from reaching 0.
static std::vector<std::pair<id<MTLTexture>, id<MTLTexture>>> g_textures;

namespace Marvel {
    
    void* LoadTextureFromArray(unsigned width, unsigned height, float* data)
    {

        MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA32Float width:width height:height mipmapped:NO];

        textureDescriptor.usage = MTLTextureUsageShaderRead;
        textureDescriptor.storageMode = MTLStorageModeManaged;

        id <MTLTexture> texture = [mvAppleViewport::GetDevice() newTextureWithDescriptor:textureDescriptor];
        [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * 4 * 4];

        g_textures.push_back({texture, texture});

        return (__bridge void*)g_textures.back().second;
    }

    void* LoadTextureFromArrayDynamic(unsigned width, unsigned height, float* data)
    {

        MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA32Float width:width height:height mipmapped:NO];

        textureDescriptor.usage = MTLTextureUsageShaderRead;
        textureDescriptor.storageMode = MTLStorageModeManaged;

        id <MTLTexture> texture = [mvAppleViewport::GetDevice() newTextureWithDescriptor:textureDescriptor];
        [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * 4 * 4];

        g_textures.push_back({texture, texture});

        return (__bridge void*)g_textures.back().second;
    }

    void* LoadTextureFromFile(const char* filename, int& width, int& height)
    {

        unsigned char* image_data = stbi_load(filename, &width, &height, nullptr, 4);
        if (image_data == nullptr)
            return nullptr;

        MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                     width:width
                                                                                                    height:height
                                                                                                 mipmapped:NO];
        textureDescriptor.usage = MTLTextureUsageShaderRead;
        textureDescriptor.storageMode = MTLStorageModeManaged;

        id <MTLTexture> texture = [mvAppleViewport::GetDevice() newTextureWithDescriptor:textureDescriptor];
        [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:image_data bytesPerRow:width * 4];

        g_textures.push_back({texture, texture});
	    
	stbi_image_free(image_data);

        return (__bridge void*)g_textures.back().second;
    }

    void* LoadTextureFromBytes(const char* data, int& width, int& height)
    {

        if (data == nullptr)
            return nullptr;

        MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                     width:width
                                                                                                    height:height
                                                                                                 mipmapped:NO];
        textureDescriptor.usage = MTLTextureUsageShaderRead;
        textureDescriptor.storageMode = MTLStorageModeManaged;

        id <MTLTexture> texture = [mvAppleViewport::GetDevice() newTextureWithDescriptor:textureDescriptor];
        [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data bytesPerRow:width * 4];

        g_textures.push_back({texture, texture});

        return (__bridge void*)g_textures.back().second;
    }


	bool UnloadTexture(const std::string& filename)
	{

		return true;
	}

    void FreeTexture(void* texture)
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

    void UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<float>& data)
    {
        id <MTLTexture> out_srv = (__bridge id <MTLTexture>)texture;
        [out_srv replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:data.data() bytesPerRow:width * 4 * 4];
    }

}
