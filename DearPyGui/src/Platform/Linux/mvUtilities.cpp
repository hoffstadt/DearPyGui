#include "Core/mvUtilities.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <vector>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel {

    // Simple helper function to load an image into a DX11 texture with common settings
    bool LoadTextureFromFile(const char* filename, mvTexture& storage)
    {

        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
        if (image_data == nullptr)
            return false;

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        storage.texture = reinterpret_cast<void *>(image_texture);
        storage.width = image_width;
        storage.height = image_height;

        return true;
    }

    bool UnloadTexture(const std::string& filename)
	{
		// TODO : decide if cleanup is necessary
		return true;
	}

    void FreeTexture(mvTexture& storage)
    {
        auto out_srv = (GLuint)(size_t)storage.texture;
        glDeleteTextures(1, &out_srv);
    }

}
