#include "mvUtilities.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

static std::unordered_map<GLuint, GLuint> PBO_ids;

namespace Marvel {

    static void UpdatePixels(GLubyte* dst, const float* data, int size)
    {

        if(!dst)
            return;

        auto ptr = (float*)dst;

        for(int i = 0; i < size; ++i)
        {
            ptr[i] = data[i];
        }
    }

    void* LoadTextureFromArray(unsigned width, unsigned height, float* data)
    {

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);

        return reinterpret_cast<void *>(image_texture);
    }

    void* LoadTextureFromArrayDynamic(unsigned width, unsigned height, float* data)
    {

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);

        GLuint pboid;
        glGenBuffers(1, &pboid);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboid);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width*height*4*sizeof(float), 0, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        PBO_ids[image_texture] = pboid;

        return reinterpret_cast<void *>(image_texture);
    }

    void* LoadTextureFromFile(const char* filename, int& width, int& height)
    {

        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
        if (image_data == nullptr)
            return nullptr;

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

        width = image_width;
        height = image_height;

        return reinterpret_cast<void *>(image_texture);;
    }

    void* LoadTextureFromBytes(const char* data, int len, int& width, int& height)
    {
	
	// Use STB to covert encoded buffer to a gl interpretable buffer
        unsigned char* image_data = stbi_load(data, len, &image_width, &image_height, NULL, 4);
        int image_width = 0;
        int image_height = 0;
        if (image_data == nullptr)
            return nullptr;

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

        width = image_width;
        height = image_height;

        return reinterpret_cast<void *>(image_texture);;
    }
	

    bool UnloadTexture(const std::string& filename)
	{
		// TODO : decide if cleanup is necessary
		return true;
	}

    void FreeTexture(void* texture)
    {
        auto out_srv = (GLuint)(size_t)texture;

        if(PBO_ids.count(out_srv) != 0)
            PBO_ids.erase(out_srv);

        glDeleteTextures(1, &out_srv);
    }

    void UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<float>& data)
    {
        auto textureId = (GLuint)(size_t)texture;

        // start to copy from PBO to texture object ///////

        // bind the texture and PBO
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO_ids[textureId]);

        // copy pixels from PBO to texture object
        // Use offset instead of ponter.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, 0);

        ///////////////////////////////////////////////////

        // start to modify pixel values ///////////////////

        // bind PBO to update pixel values
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO_ids[textureId]);

        // map the buffer object into client's memory
        // Note that glMapBuffer() causes sync issue.
        // If GPU is working with this buffer, glMapBuffer() will wait(stall)
        // for GPU to finish its job. To avoid waiting (stall), you can call
        // first glBufferData() with NULL pointer before glMapBuffer().
        // If you do that, the previous data in PBO will be discarded and
        // glMapBuffer() returns a new allocated pointer immediately
        // even if GPU is still working with the previous data.
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width*height*4*sizeof(float), 0, GL_STREAM_DRAW);
        GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        if(ptr)
        {
            // update data directly on the mapped buffer
            UpdatePixels(ptr, data.data(), data.size());

            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);  // release pointer to mapping buffer
        }

        ///////////////////////////////////////////////////

        // it is good idea to release PBOs with ID 0 after use.
        // Once bound with 0, all pixel operations behave normal ways.
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

}
