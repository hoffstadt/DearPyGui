#include "mvUtilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "mvContext.h"
#include "mvLinuxSpecifics.h"
#include "mvViewport.h"
#include "mvPythonExceptions.h"
#include "mvBuffer.h"

static std::unordered_map<GLuint, GLuint> PBO_ids;

mv_internal void
UpdatePixels(GLubyte* dst, const float* data, int size)
{

    if(!dst)
        return;

    auto ptr = (float*)dst;

    for(int i = 0; i < size; ++i)
    {
        ptr[i] = data[i];
    }
}

mv_impl void
OutputFrameBufferArray(PymvBuffer* out)
{
    mvViewport* viewport = GContext->viewport;
    auto viewportData = (mvViewportData*)viewport->platformSpecifics;

    int display_w, display_h;
    glfwGetFramebufferSize(viewportData->handle, &display_w, &display_h);

    stbi_flip_vertically_on_write(true);
    GLint ReadType = GL_UNSIGNED_BYTE;
    GLint ReadFormat = GL_RGBA;
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &ReadType);
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &ReadFormat);
    auto data = (GLubyte*)malloc(4 * display_w * display_h);
    glReadPixels(0, 0, display_w, display_h, ReadFormat,  ReadType, data);
    out->arr.length = display_w*display_h*4;
    f32* tdata = new f32[out->arr.length];
    out->arr.width = display_w;
    out->arr.height = display_h;
    for(int row = 0; row < out->arr.height; row++)
    {
        for(int col = 0; col < out->arr.width*4; col++)
        {
            tdata[row*out->arr.width*4+ col] = (f32) data[(out->arr.height-1-row)*out->arr.width*4 + col] / 255.0f;
        }
    }
    out->arr.data = tdata;
    free(data);
}

mv_impl void
OutputFrameBuffer(const char* filepath)
{
    mvViewport* viewport = GContext->viewport;
    auto viewportData = (mvViewportData*)viewport->platformSpecifics;

    int display_w, display_h;
    glfwGetFramebufferSize(viewportData->handle, &display_w, &display_h);

    stbi_flip_vertically_on_write(true);
    GLint ReadType = GL_UNSIGNED_BYTE;
    GLint ReadFormat = GL_RGBA;
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &ReadType);
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &ReadFormat);
    auto data = (GLubyte*)malloc(4 * display_w * display_h);
    glReadPixels(0, 0, display_w, display_h, ReadFormat,  ReadType, data);
    stbi_write_png(filepath, display_w, display_h, 4, data, display_w*4);
    free(data);
}

mv_impl void*
LoadTextureFromArray(unsigned width, unsigned height, float* data)
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

mv_impl void*
LoadTextureFromArrayDynamic(unsigned width, unsigned height, float* data)
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

mv_impl void*
LoadTextureFromArrayRaw(unsigned width, unsigned height, float* data, int components)
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
    if(components == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);

    GLuint pboid;
    glGenBuffers(1, &pboid);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboid);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * components * sizeof(float), 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    PBO_ids[image_texture] = pboid;

    return reinterpret_cast<void*>(image_texture);
}

mv_impl void*
LoadTextureFromFile(const char* filename, int& width, int& height)
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

mv_impl bool
UnloadTexture(const std::string& filename)
{
    // TODO : decide if cleanup is necessary
    return true;
}

mv_impl void
FreeTexture(void* texture)
{
    auto out_srv = (GLuint)(size_t)texture;

    if(PBO_ids.count(out_srv) != 0)
        PBO_ids.erase(out_srv);

    glDeleteTextures(1, &out_srv);
}

mv_impl void
UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<float>& data)
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

mv_impl void
UpdateRawTexture(void* texture, unsigned width, unsigned height, float* data, int components)
{
    auto textureId = (GLuint)(size_t)texture;

    // start to copy from PBO to texture object ///////

    // bind the texture and PBO
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO_ids[textureId]);

    // copy pixels from PBO to texture object
    // Use offset instead of ponter.
    if(components == 4)
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, 0);
    else
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, 0);

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
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * components * sizeof(float), 0, GL_STREAM_DRAW);
    GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    if (ptr)
    {
        // update data directly on the mapped buffer
        UpdatePixels(ptr, data, width*height*components);

        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);  // release pointer to mapping buffer
    }

    ///////////////////////////////////////////////////

    // it is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}