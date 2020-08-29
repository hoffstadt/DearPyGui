#include "Core/mvUtilities.h"
#include "mvAppleWindow.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>
#include <Quartz/Quartz.h>
#include <simd/simd.h>
#include <vector>

// this is necessary to keep objective-c's reference counts
// from reaching 0.
static std::vector<std::pair<std::string, id<MTLTexture>>> g_textures;

namespace Marvel {

    bool LoadTextureFromFile(const char* filename, mvTexture& storage)
    {
        int width, height;

        unsigned char* image_data = stbi_load(filename, &width, &height, nullptr, 4);
        if (image_data == nullptr)
            return false;

        MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                     width:width
                                                                                                    height:height
                                                                                                 mipmapped:NO];
        textureDescriptor.usage = MTLTextureUsageShaderRead;
        textureDescriptor.storageMode = MTLStorageModeManaged;

        id <MTLTexture> texture = [mvAppleWindow::GetDevice() newTextureWithDescriptor:textureDescriptor];
        [texture replaceRegion:MTLRegionMake2D(0, 0, width, height) mipmapLevel:0 withBytes:image_data bytesPerRow:width * 4];

        g_textures.push_back({filename, texture});

        storage.texture = (__bridge void*)g_textures.back().second;
        storage.width = width;
        storage.height = height;

        return true;
    }

	bool UnloadTexture(const std::string& filename)
	{
        std::vector<std::pair<std::string, id<MTLTexture>>> oldtextures = g_textures;
        g_textures.clear();
        for(auto& texture : oldtextures)
        {
            if(texture.first != filename)
                g_textures.push_back(texture);
        }
		return true;
	}

    void FreeTexture(mvTexture& storage)
    {
        auto out_srv = (GLuint)(size_t)storage.texture;
        glDeleteTextures(1, &out_srv);
    }

}
