#include "Core/mvUtilities.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <vector>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel {

    // Simple helper function to load an image into a DX11 texture with common settings
    bool LoadTextureFromFile(const char* filename, void* vout_srv, int* out_width, int* out_height)
    {
        auto out_srv = static_cast<GLuint*>(vout_srv);

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

        *out_srv = image_texture;
        *out_width = image_width;
        *out_height = image_height;

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