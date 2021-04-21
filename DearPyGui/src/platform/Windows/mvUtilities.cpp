#include "mvUtilities.h"
#include "mvWindowsViewport.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shlobj.h> 
#include <shobjidl.h> 
#include <windows.h>
#include <atlbase.h> // Contains the declaration of CComPtr.
#include <array>
#include <codecvt>
#include <sstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace Marvel {

    void* LoadTextureFromArray(unsigned width, unsigned height, float* data)
    {
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
        subResource.SysMemPitch = desc.Width * 4 * 4;
        subResource.SysMemSlicePitch = 0;
        mvWindowsViewport::getDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
                // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        mvWindowsViewport::getDevice()->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
        pTexture->Release();

        return out_srv;
    }

    void FreeTexture(void* texture)
    {
        ID3D11ShaderResourceView* out_srv = static_cast<ID3D11ShaderResourceView*>(texture);
        if (out_srv)
            auto count = out_srv->Release();

        texture = nullptr;
    }

    // Simple helper function to load an image into a DX11 texture with common settings
    void* LoadTextureFromFile(const char* filename, int& width, int& height)
    {

        //auto out_srv = static_cast<ID3D11ShaderResourceView**>(storage.texture);
        ID3D11ShaderResourceView* out_srv = nullptr;

        // Load from disk into a raw RGBA buffer
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

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
        mvWindowsViewport::getDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        mvWindowsViewport::getDevice()->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
        pTexture->Release();

        width = image_width;
        height = image_height;
        stbi_image_free(image_data);

        return out_srv;
    }

}