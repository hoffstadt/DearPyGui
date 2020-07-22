#include "Core/mvUtilities.h"
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

namespace fs = std::filesystem;

namespace Marvel {

	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{

		if (uMsg == BFFM_INITIALIZED)
		{
			std::string tmp = (const char*)lpData;
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		}

		return 0;
	}

    // Simple helper function to load an image into a DX11 texture with common settings
    bool LoadTextureFromFile(const char* filename, void* vout_srv, int* out_width, int* out_height)
    {

        auto out_srv = static_cast<ID3D11ShaderResourceView**>(vout_srv);

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
        mvWindowsWindow::getDevice()->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
        pTexture->Release();

        *out_width = image_width;
        *out_height = image_height;
        stbi_image_free(image_data);

        return true;
    }

	bool UnloadTexture(void* texture)
	{
		// TODO : decide if cleanup is necessary
		return true;
	}

	void RunFile(const std::string& name, const std::string& file, const std::string& flags)
	{
		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		fs::path p = fs::path(file);
		p.replace_extension(" ");
		auto filename = p.filename().string();
		auto pathname = p.parent_path().string();

		size_t lastindex = filename.find_last_of(".");
		std::string rawname = filename.substr(0, lastindex);

		//std::string cmd = std::string("\"") + std::string(Marvel::Application::getArgv0()) + std::string("\" --app ") + rawname + " --path \"" + pathname + "\" " + flags;
		std::string cmd = "";
		std::string pname = name + ".exe";

		// start the program up
		CreateProcess(const_cast<char*>(pname.c_str()),   // the path
			const_cast<char*>(cmd.c_str()),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		);
		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	std::string PickDirectory(const std::string& directory)
	{
		TCHAR path[MAX_PATH];

		const char* path_param = directory.c_str();

		BROWSEINFO bi = { 0 };
		bi.lpszTitle = ("Browse for folder...");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = (LPARAM)path_param;

		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

		if (pidl != 0)
		{
			//get the name of the folder and put it in path
			SHGetPathFromIDList(pidl, path);

			//free memory used
			IMalloc* imalloc = 0;
			if (SUCCEEDED(SHGetMalloc(&imalloc)))
			{
				imalloc->Free(pidl);
				imalloc->Release();
			}

			return path;
		}

		return "";
	}

	std::string SaveFile(const std::vector<std::pair<std::string, std::string >>& extensions)
	{

		std::string  result = "";

		std::vector<std::pair<std::wstring, std::wstring >> wextensions;

		for (const auto& item : extensions)
		{
			wextensions.emplace_back(std::wstring(L""), std::wstring(L""));
			wextensions.back().first = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(item.first);
			wextensions.back().second = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(item.second);
		}

		COMDLG_FILTERSPEC* fspecArray = new COMDLG_FILTERSPEC[wextensions.size() + 1];
		for (int i = 0; i < wextensions.size(); i++)
		{
			fspecArray[i].pszName = wextensions[i].first.c_str();
			fspecArray[i].pszSpec = wextensions[i].second.c_str();
		}

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			CComPtr<IFileSaveDialog> pFileOpen;

			// Create the FileOpenDialog object.
			hr = pFileOpen.CoCreateInstance(__uuidof(FileSaveDialog));

			hr = pFileOpen->SetFileTypes(extensions.size(), fspecArray);
			hr = pFileOpen->SetDefaultExtension(fspecArray[0].pszSpec);
			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					CComPtr<IShellItem> pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						if (SUCCEEDED(hr))
							result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(pszFilePath);


					}

					// pItem goes out of scope.
				}

				// pFileOpen goes out of scope.
			}
			CoUninitialize();
		}

		delete[] fspecArray;
		return result;
	}

	std::string OpenFile(const std::vector<std::pair<std::string, std::string>>& extensions)
	{
		std::string result = "";

		std::vector<std::pair<std::wstring, std::wstring >> wextensions;

		for (const auto& item : extensions)
		{
			wextensions.emplace_back(std::wstring(L""), std::wstring(L""));
			wextensions.back().first = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(item.first);
			wextensions.back().second = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(item.second);
		}

		COMDLG_FILTERSPEC* fspecArray = new COMDLG_FILTERSPEC[wextensions.size() + 1];
		for (int i = 0; i < wextensions.size(); i++)
		{
			fspecArray[i].pszName = wextensions[i].first.c_str();
			fspecArray[i].pszSpec = wextensions[i].second.c_str();
		}

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			CComPtr<IFileOpenDialog> pFileOpen;

			// Create the FileOpenDialog object.
			hr = pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog));
			hr = pFileOpen->SetFileTypes(extensions.size(), fspecArray);
			hr = pFileOpen->SetDefaultExtension(fspecArray[0].pszSpec);
			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					CComPtr<IShellItem> pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						if (SUCCEEDED(hr))
							result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(pszFilePath);

					}



					// pItem goes out of scope.
				}

				// pFileOpen goes out of scope.
			}
			CoUninitialize();
		}

		delete[] fspecArray;
		return result;
	}

}