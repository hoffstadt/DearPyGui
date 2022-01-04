#include "mvViewport.h"
#include "mvFontManager.h"
#include <implot.h>
#include <imnodes.h>
#include <cstdlib>
#include <ctime>
#include "mvToolManager.h"
#include "mvItemRegistry.h"
#include "mvProfiler.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

ID3D11Device*                   gdevice = nullptr;
ID3D11DeviceContext*            gdeviceContext = nullptr;
static HWND                     ghandle = nullptr;
static WNDCLASSEX               gwc;
static MSG                      gmsg;
static DWORD                    gmodes;
static IDXGISwapChain*          gswapChain = nullptr;
static ID3D11RenderTargetView*  gtarget = nullptr;
static BYTE                     gprevious_ime_char;
static WORD                     glang_id;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Marvel {

	mv_internal std::vector <IDXGIAdapter*>
	EnumerateAdapters()
	{
		IDXGIAdapter* pAdapter;
		std::vector <IDXGIAdapter*> vAdapters;
		IDXGIFactory* pFactory = NULL;


		// Create a DXGIFactory object.
		if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
		{
			return vAdapters;
		}


		for (UINT i = 0;
			pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
			++i)
		{
			vAdapters.push_back(pAdapter);
		}


		if (pFactory)
		{
			pFactory->Release();
		}

		return vAdapters;

	}

	mv_internal void
	mvHandleModes()
	{
		gmodes = WS_OVERLAPPED;

		if (GContext->viewport->resizable && GContext->viewport->decorated) gmodes |= WS_THICKFRAME;
		if (GContext->viewport->decorated) gmodes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	}

	mv_internal bool
	mvCreateDeviceD3D()
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = ghandle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		auto adapters = EnumerateAdapters();

		UINT createDeviceFlags = 0;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

		// use default adapter
		if (GContext->IO.info_auto_device)
		{

			int index = 0;
			SIZE_T adapterMemory = 0;

			for (int i = 0; i < adapters.size(); i++)
			{
				DXGI_ADAPTER_DESC adpdesc;
				adapters[i]->GetDesc(&adpdesc);
				if (adpdesc.DedicatedVideoMemory > adapterMemory)
				{
					adapterMemory = adpdesc.DedicatedVideoMemory;
					index = i;
				}

			}

			if (D3D11CreateDeviceAndSwapChain(adapters[index], D3D_DRIVER_TYPE_UNKNOWN, nullptr,
				createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &gswapChain,
				&gdevice, &featureLevel, &gdeviceContext) != S_OK)
				return false;
		}
		else if (GContext->IO.info_device == -1)
		{
			if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
				createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &gswapChain,
				&gdevice, &featureLevel, &gdeviceContext) != S_OK)
				return false;
		}
		else
		{

			if (D3D11CreateDeviceAndSwapChain(adapters[GContext->IO.info_device], D3D_DRIVER_TYPE_UNKNOWN, nullptr,
				createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &gswapChain,
				&gdevice, &featureLevel, &gdeviceContext) != S_OK)
				return false;
		}

		// create render target
		ID3D11Texture2D* pBackBuffer;
		gswapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		gdevice->CreateRenderTargetView(pBackBuffer, nullptr, &gtarget);
		pBackBuffer->Release();

		//UINT support1;
		//s_pd3dDevice->CheckFormatSupport(DXGI_FORMAT_R8G8B8A8_SINT, &support1);
		//if (support1 & D3D11_FORMAT_SUPPORT_TEXTURE2D)
		//{
		//	int a = 5;
		//}

		return true;
	}

	mv_internal void
	mvCleanupDeviceD3D()
	{
		if (gtarget)
		{
			gtarget->Release();
			gtarget = nullptr;
		}

		if (gswapChain)
		{
			gswapChain->Release();
			gswapChain = nullptr;
		}

		if (gdeviceContext)
		{
			gdeviceContext->Release();
			gdeviceContext = nullptr;
		}

		if (gdevice)
		{
			gdevice->Release();
			gdevice = nullptr;
		}
	}

	mv_internal void
	mvPrerender()
	{
		MV_PROFILE_SCOPE("Viewport prerender")

			mvViewport* viewport = GContext->viewport;

		if (gmsg.message == WM_QUIT)
			viewport->running = false;

		if (viewport->posDirty)
		{
			SetWindowPos(ghandle, viewport->alwaysOnTop ? HWND_TOPMOST : HWND_TOP, viewport->xpos, viewport->ypos, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
			viewport->posDirty = false;
		}

		if (viewport->sizeDirty)
		{
			SetWindowPos(ghandle, viewport->alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, viewport->actualWidth, viewport->actualHeight, SWP_SHOWWINDOW | SWP_NOMOVE);
			viewport->sizeDirty = false;
		}

		if (viewport->modesDirty)
		{
			gmodes = WS_OVERLAPPED;

			if (viewport->resizable && viewport->decorated) gmodes |= WS_THICKFRAME;
			if (viewport->decorated) gmodes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

			SetWindowLongPtr(ghandle, GWL_STYLE, gmodes);
			SetWindowPos(ghandle, viewport->alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			viewport->modesDirty = false;
		}

		if (viewport->titleDirty)
		{
			SetWindowTextA(ghandle, viewport->title.c_str());
			viewport->titleDirty = false;
		}

		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

		if(GContext->IO.waitForInput)
			::WaitMessage();

		if (::PeekMessage(&gmsg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&gmsg);
			::DispatchMessage(&gmsg);
			//continue;
		}

		if (mvToolManager::GetFontManager().isInvalid())
		{
			mvToolManager::GetFontManager().rebuildAtlas();
			ImGui_ImplDX11_InvalidateDeviceObjects();
			mvToolManager::GetFontManager().updateAtlas();
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	}

	mv_internal void
	mvPostrender()
	{

		MV_PROFILE_SCOPE("Presentation")

		mvViewport* viewport = GContext->viewport;

		// Rendering
		ImGui::Render();
		gdeviceContext->OMSetRenderTargets(1, &gtarget, nullptr);
		gdeviceContext->ClearRenderTargetView(gtarget, viewport->clearColor);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		static UINT presentFlags = 0;
		if (gswapChain->Present(viewport->vsync ? 1 : 0, presentFlags) == DXGI_STATUS_OCCLUDED)
		{
			presentFlags = DXGI_PRESENT_TEST;
			Sleep(20);
		}
		else
			presentFlags = 0;
	}

	mv_internal LRESULT
	mvHandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		mvViewport* viewport = GContext->viewport;

		switch (msg)
		{
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = viewport->minwidth;
			lpMMI->ptMinTrackSize.y = viewport->minheight;
			lpMMI->ptMaxTrackSize.x = viewport->maxwidth;
			lpMMI->ptMaxTrackSize.y = viewport->maxheight;
			break;
		}

		case WM_MOVING:
			viewport->xpos = (int)(short)LOWORD(lParam);   // horizontal position 
			viewport->ypos = (int)(short)HIWORD(lParam);   // vertical position 
			break;

		case WM_SIZE:
			if (gdevice != nullptr && wParam != SIZE_MINIMIZED)
			{
				RECT rect;
				RECT crect;
				int awidth = 0;
				int aheight = 0;
				int cwidth = 0;
				int cheight = 0;
				if (GetWindowRect(hWnd, &rect))
				{
					awidth = rect.right - rect.left;
					aheight = rect.bottom - rect.top;
				}

				if (GetClientRect(hWnd, &crect))
				{
					cwidth = crect.right - crect.left;
					cheight = crect.bottom - crect.top;
				}

				viewport->actualWidth = awidth;
				viewport->actualHeight = aheight;


				if (viewport->decorated)
				{
					GContext->viewport->clientHeight = cheight;
					GContext->viewport->clientWidth = cwidth;
				}
				else
				{
					GContext->viewport->clientHeight = cheight + 39;
					GContext->viewport->clientWidth = cwidth + 16;
				}
				
				GContext->viewport->resized = true;
				//mvOnResize();

				// I believe this are only used for the error logger
				viewport->width = (UINT)LOWORD(lParam);
				viewport->height = (UINT)HIWORD(lParam);

				if (gtarget)
				{
					gtarget->Release();
					gtarget = nullptr;
				}

				if (viewport->decorated)
					gswapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				else
					gswapChain->ResizeBuffers(0, (UINT)awidth, (UINT)aheight, DXGI_FORMAT_UNKNOWN, 0);

				// recreate render target
				ID3D11Texture2D* pBackBuffer;
				gswapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
				gdevice->CreateRenderTargetView(pBackBuffer, nullptr, &gtarget);
				pBackBuffer->Release();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			GContext->started = false;
			::PostQuitMessage(0);
			return 0;
		case WM_INPUTLANGCHANGE:
			glang_id = LOWORD(lParam);
			break;
		case WM_IME_CHAR:
			if (glang_id == 0x0412)
				break;
			auto& io = ImGui::GetIO();
			DWORD wChar = (DWORD)wParam;
			if (wChar <= 127)
			{
				io.AddInputCharacter(wChar);
				return 0;
			}
			else if (wChar < 0xFF) {
				//GBK also supports Traditional Chinese, Japanese, English, Russian and (partially) Greek.
				//When input Japanese with MS Japanese IME under CP936, dual bytes of one Japanese char are send within one or two wParam in succession.
				if (gprevious_ime_char == 0) {
					gprevious_ime_char = (BYTE)(wChar & 0x00FF);
					return 0;
				}
				else {
					BYTE b2 = (BYTE)(wChar & 0x00FF);
					wChar = MAKEWORD(gprevious_ime_char, b2);
					gprevious_ime_char = 0;
				}
			}
			else
			{
				// swap lower and upper part.
				BYTE low = (BYTE)(wChar & 0x00FF);
				BYTE high = (BYTE)((wChar & 0xFF00) >> 8);
				if (gprevious_ime_char == 0)
					wChar = MAKEWORD(high, low);
				else
				{
					//wChar = MAKEWORD(previous_ime_char, high, low);
					wChar = MAKEWORD(gprevious_ime_char, high);
					gprevious_ime_char = low;
				}
			}
			wchar_t ch[6];
			MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR)&wChar, 4, ch, 3);
			io.AddInputCharacter(ch[0]);
			return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	mv_impl mvViewport*
	mvCreateViewport(unsigned width, unsigned height)
	{
		mvViewport* viewport = new mvViewport();
		viewport->width = width;
		viewport->height = height;
		return viewport;
	}

	mv_impl void
	mvShowViewport(bool minimized, bool maximized)
	{
		mvViewport* viewport = GContext->viewport;
		gwc = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			mvHandleMsg,
			0L,
			0L,
			GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
			_T(viewport->title.c_str()), nullptr
		};
		RegisterClassEx(&gwc);

		mvHandleModes();
		ghandle = CreateWindow(gwc.lpszClassName, _T(viewport->title.c_str()),
			gmodes,
			viewport->xpos, viewport->ypos, viewport->actualWidth, viewport->actualHeight, nullptr, nullptr, gwc.hInstance, nullptr);

		GContext->viewport->clientHeight = viewport->actualHeight;
		GContext->viewport->clientWidth = viewport->actualWidth;

		if (!viewport->small_icon.empty())
		{
			HANDLE hIcon = LoadImage(0, _T(viewport->small_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
			if (hIcon)
			{
				SendMessage(ghandle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				SendMessage(GetWindow(ghandle, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			}
		}

		if (!viewport->large_icon.empty())
		{
			HANDLE hIcon = LoadImage(0, _T(viewport->large_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
			if (hIcon)
			{
				SendMessage(ghandle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				SendMessage(GetWindow(ghandle, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			}
		}

		// Initialize Direct3D
		if (!mvCreateDeviceD3D())
		{
			mvCleanupDeviceD3D();
			::UnregisterClass(gwc.lpszClassName, gwc.hInstance);
		}
		gprevious_ime_char = 0;
		HKL hkl = GetKeyboardLayout(0);
		glang_id = LOWORD(hkl);

		// Show the window

		int cmdShow;

		if (minimized)
			cmdShow = SW_MINIMIZE;
		else if (maximized)
			cmdShow = SW_MAXIMIZE;
		else
			cmdShow = SW_SHOWDEFAULT;

		::ShowWindow(ghandle, cmdShow);
		::UpdateWindow(ghandle);

		if (viewport->alwaysOnTop)
			SetWindowPos(ghandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		if (GContext->IO.loadIniFile)
		{
			ImGui::LoadIniSettingsFromDisk(GContext->IO.iniFile.c_str());
			io.IniFilename = nullptr;
			if(GContext->IO.autoSaveIniFile)
				io.IniFilename = GContext->IO.iniFile.c_str();
		}
		else
		{
			if (GContext->IO.iniFile.empty())
				io.IniFilename = nullptr;
			else
				io.IniFilename = GContext->IO.iniFile.c_str();
		}

		if (GContext->IO.docking)
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		SetDefaultTheme();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(ghandle);
		ImGui_ImplDX11_Init(gdevice, gdeviceContext);
	}

	mv_impl void
	mvMaximizeViewport()
	{
		ShowWindow(ghandle, SW_MAXIMIZE);
	}

	mv_impl void
	mvMinimizeViewport()
	{
		ShowWindow(ghandle, SW_MINIMIZE);
	}

	mv_impl void
	mvCleanupViewport()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		mvCleanupDeviceD3D();
		::DestroyWindow(ghandle);
		::UnregisterClass(gwc.lpszClassName, gwc.hInstance);
	}

	mv_impl void
	mvRenderFrame()
	{
		mvPrerender();
		Render();
		mvPostrender();
	}

	mv_impl void
	mvToggleFullScreen()
    {

        mv_local_persist size_t storedWidth = 0;
        mv_local_persist size_t storedHeight = 0;
        mv_local_persist int    storedXPos = 0;
        mv_local_persist int    storedYPos = 0;
        
        size_t width = GetSystemMetrics(SM_CXSCREEN);
        size_t height = GetSystemMetrics(SM_CYSCREEN);

        if(GContext->viewport->fullScreen)
        {
			RECT rect;
			rect.left = storedXPos;
			rect.top = storedYPos;
			rect.right = storedXPos + storedWidth;
			rect.bottom = storedYPos + storedHeight;
            SetWindowLongPtr(ghandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow(ghandle, storedXPos, storedYPos, storedWidth, storedHeight, TRUE);
            GContext->viewport->fullScreen = false;
        }
        else
        {
            storedWidth = GContext->viewport->actualWidth;
            storedHeight = GContext->viewport->actualHeight;
            storedXPos = GContext->viewport->xpos;
            storedYPos = GContext->viewport->ypos;
            
            SetWindowLongPtr(ghandle, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(ghandle, 0, 0, width, height, TRUE);
            GContext->viewport->fullScreen = true;
        }
    }

}
