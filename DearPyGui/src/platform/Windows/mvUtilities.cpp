#include "mvUtilities.h"
#include "mvViewport.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <shlobj.h>
#include <shobjidl.h> 
#include <windows.h>
#include <atlbase.h> // Contains the declaration of CComPtr.
#include <array>
#include <codecvt>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <d3d11.h>
#include "mvWindowsSpecifics.h"
#include "mvBuffer.h"

namespace fs = std::filesystem;

mv_impl void
OutputFrameBufferArray(PymvBuffer* out)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;

    D3D11_TEXTURE2D_DESC description;
    graphicsData->backBuffer->GetDesc(&description);
    description.BindFlags = 0;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    description.Usage = D3D11_USAGE_STAGING;

    ID3D11Texture2D* stagingBuffer = nullptr;
    graphicsData->device->CreateTexture2D(&description, nullptr, &stagingBuffer);

    if (stagingBuffer)
    {
        graphicsData->deviceContext->CopyResource(stagingBuffer, graphicsData->backBuffer);
        D3D11_MAPPED_SUBRESOURCE resource;
        unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
        HRESULT hr = graphicsData->deviceContext->Map(stagingBuffer, subresource, D3D11_MAP_READ_WRITE, 0, &resource);
        out->arr.length = description.Width * description.Height * 4;
        u8* data = new u8[out->arr.length];
        f32* tdata = new f32[out->arr.length];
        for (int row = 0; row < description.Height; row++)
        {
            u8* src = &(((unsigned char*)resource.pData)[row * resource.RowPitch]);
            for (int j = 0; j < description.Width*4; j++)
                tdata[row * description.Width*4 + j] = src[j] / 255.0f;
        }
        out->arr.data = tdata;
        out->arr.width = description.Width;
        out->arr.height = description.Height;
        delete[] data; 
        stagingBuffer->Release();
        stagingBuffer = nullptr;
    }
}

mv_impl void
OutputFrameBuffer(const char* filepath)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;

    D3D11_TEXTURE2D_DESC description;
    graphicsData->backBuffer->GetDesc(&description);
    description.BindFlags = 0;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    description.Usage = D3D11_USAGE_STAGING;

    ID3D11Texture2D* stagingBuffer = nullptr;
    graphicsData->device->CreateTexture2D(&description, nullptr, &stagingBuffer);

    if (stagingBuffer)
    {
        graphicsData->deviceContext->CopyResource(stagingBuffer, graphicsData->backBuffer);
        D3D11_MAPPED_SUBRESOURCE resource;
        unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
        HRESULT hr = graphicsData->deviceContext->Map(stagingBuffer, subresource, D3D11_MAP_READ_WRITE, 0, &resource);
        stbi_write_png(filepath, description.Width, description.Height, 4, resource.pData, resource.RowPitch);
        stagingBuffer->Release();
        stagingBuffer = nullptr;
        return;
    }
    

}

mv_impl void*
LoadTextureFromFile(const char* filename, int& width, int& height)
{

    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;

    //auto out_srv = static_cast<ID3D11ShaderResourceView**>(storage.texture);
    ID3D11ShaderResourceView* out_srv = nullptr;

    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return nullptr;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    graphicsData->device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    graphicsData->device->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    width = image_width;
    height = image_height;
    stbi_image_free(image_data);

    return out_srv;
}

mv_impl void*
LoadTextureFromArray(unsigned width, unsigned height, float* data)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* out_srv = nullptr;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = data;
    subResource.SysMemPitch = desc.Width * 4 * sizeof(float);
    subResource.SysMemSlicePitch = 0;
    graphicsData->device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    graphicsData->device->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    return out_srv;
}

mv_impl void*
LoadTextureFromArray(unsigned width, unsigned height, int* data)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* out_srv = nullptr;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.Format = DXGI_FORMAT_R32G32B32_SINT;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = data;
    subResource.SysMemPitch = desc.Width * 4 * sizeof(int);
    subResource.SysMemSlicePitch = 0;
    graphicsData->device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    graphicsData->device->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    return out_srv;
}

mv_impl void*
LoadTextureFromArrayDynamic(unsigned width, unsigned height, float* data)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* out_srv = nullptr;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = data;
    subResource.SysMemPitch = desc.Width * 4 * 4;
    subResource.SysMemSlicePitch = 0;
    graphicsData->device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    graphicsData->device->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    return out_srv;
}

mv_impl void*
LoadTextureFromArrayDynamic(unsigned width, unsigned height, int* data)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* out_srv = nullptr;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Format = DXGI_FORMAT_R32G32B32_SINT;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = data;
    subResource.SysMemPitch = desc.Width * 4 * 4;
    subResource.SysMemSlicePitch = 0;
    graphicsData->device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    graphicsData->device->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    return out_srv;
}

mv_impl void
UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<float>& data)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* view = (ID3D11ShaderResourceView*)texture;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    //  Disable GPU access to the vertex buffer data.
    ID3D11Resource* resource;
    view->GetResource(&resource);
    graphicsData->deviceContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
    BYTE* buffer = reinterpret_cast<BYTE*>(data.data());
    for (UINT i = 0; i < height; ++i)
    {
        memcpy(mappedData, buffer, width * 4 * sizeof(float));
        mappedData += mappedResource.RowPitch;
        buffer += width * 4 * sizeof(float);
    }
    //  Update the vertex buffer here.
    //memcpy(mappedResource.pData, data.data(), data.size()*sizeof(float));
    //  Reenable GPU access to the vertex buffer data.
    graphicsData->deviceContext->Unmap(resource, 0);

    resource->Release();
}

mv_impl void
UpdateTexture(void* texture, unsigned width, unsigned height, std::vector<int>& data)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* view = (ID3D11ShaderResourceView*)texture;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    //  Disable GPU access to the vertex buffer data.
    ID3D11Resource* resource;
    view->GetResource(&resource);
    graphicsData->deviceContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
    BYTE* buffer = reinterpret_cast<BYTE*>(data.data());
    for (UINT i = 0; i < height; ++i)
    {
        memcpy(mappedData, buffer, width * 4 * sizeof(int));
        mappedData += mappedResource.RowPitch;
        buffer += width * 4 * sizeof(int);
    }
    //  Update the vertex buffer here.
    //memcpy(mappedResource.pData, data.data(), data.size()*sizeof(float));
    //  Reenable GPU access to the vertex buffer data.
    graphicsData->deviceContext->Unmap(resource, 0);

    resource->Release();
}

mv_impl void
UpdateRawTexture(void* texture, unsigned width, unsigned height, float* data, int components)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* view = (ID3D11ShaderResourceView*)texture;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    //  Disable GPU access to the vertex buffer data.
    ID3D11Resource* resource;
    view->GetResource(&resource);
    graphicsData->deviceContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
    BYTE* buffer = reinterpret_cast<BYTE*>(data);
    for (UINT i = 0; i < height; ++i)
    {
        memcpy(mappedData, buffer, width * components * sizeof(float));
        mappedData += mappedResource.RowPitch;
        buffer += width * components * sizeof(float);
    }
    //  Update the vertex buffer here.
    //memcpy(mappedResource.pData, data.data(), data.size()*sizeof(float));
    //  Reenable GPU access to the vertex buffer data.
    graphicsData->deviceContext->Unmap(resource, 0);

    resource->Release();
}

mv_impl void*
LoadTextureFromArrayRaw(unsigned width, unsigned height, float* data, int components)
{
    mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)GContext->graphics.backendSpecifics;
    ID3D11ShaderResourceView* out_srv = nullptr;

    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;

    switch (components)
    {
    case(3):
        format = DXGI_FORMAT_R32G32B32_FLOAT;
        break;
    case(4):
        format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    default:
        format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    }

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Format = format;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = data;
    subResource.SysMemPitch = desc.Width * components * sizeof(float);
    subResource.SysMemSlicePitch = 0;
    graphicsData->device->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    graphicsData->device->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    return out_srv;
}

mv_impl void
FreeTexture(void* texture)
{
    ID3D11ShaderResourceView* out_srv = static_cast<ID3D11ShaderResourceView*>(texture);
    if (out_srv)
        auto count = out_srv->Release();

    texture = nullptr;
}

mv_impl bool
UnloadTexture(const std::string& filename)
{
    // TODO : decide if cleanup is necessary
    return true;
}