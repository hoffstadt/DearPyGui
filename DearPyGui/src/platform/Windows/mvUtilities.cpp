#include "core/mvUtilities.h"
#include "mvWindowsWindow.h"

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

    bool LoadTextureFromArray(const char* name, float* data, unsigned width, unsigned height, mvTexture& storage)
    {

        //auto out_srv = static_cast<ID3D11ShaderResourceView**>(storage.texture);
        ID3D11ShaderResourceView* out_srv = nullptr;

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        mvWindowsWindow::getDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        mvWindowsWindow::getDevice()->CreateShaderResourceView(pTexture, nullptr, &out_srv);
        pTexture->Release();

        storage.texture = out_srv;
        storage.width = width;
        storage.height = height;

        return true;
    }

    // Simple helper function to load an image into a DX11 texture with common settings
    bool LoadTextureFromFile(const char* filename, mvTexture& storage)
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
        mvWindowsWindow::getDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        mvWindowsWindow::getDevice()->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
        pTexture->Release();

        storage.texture = out_srv;
        storage.width = image_width;
        storage.height = image_height;
        stbi_image_free(image_data);

        return true;
    }

    bool UnloadTexture(const std::string& filename)
	{
		// TODO : decide if cleanup is necessary
		return true;
	}

    void FreeTexture(mvTexture& storage)
    {
        ID3D11ShaderResourceView* out_srv = static_cast<ID3D11ShaderResourceView*>(storage.texture);
        out_srv->Release();
        out_srv = nullptr;
    }

}