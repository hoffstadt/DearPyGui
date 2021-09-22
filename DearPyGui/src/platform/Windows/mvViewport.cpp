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

static HWND                     ghandle = nullptr;
static WNDCLASSEX               gwc;
static MSG                      gmsg;
static DWORD                    gmodes;
static ID3D11Device*            gdevice = nullptr;
static ID3D11DeviceContext*     gdeviceContext = nullptr;
static IDXGISwapChain*          gswapChain = nullptr;
static ID3D11RenderTargetView*  gtarget = nullptr;
static BYTE                     gprevious_ime_char;
static WORD                     glang_id;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Marvel {

	static std::vector <IDXGIAdapter*> EnumerateAdapters()
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

	static void mvHandleModes()
	{
		GContext->viewport->modes = WS_OVERLAPPED;

		if (GContext->viewport->resizable && GContext->viewport->decorated) GContext->viewport->modes |= WS_THICKFRAME;
		if (GContext->viewport->decorated) GContext->viewport->modes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	}

	static bool mvCreateDeviceD3D()
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
		sd.OutputWindow = GContext->viewport->handle;
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
			int adapterMemory = 0;

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
				createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &GContext->viewport->swapChain,
				&GContext->viewport->device, &featureLevel, &GContext->viewport->deviceContext) != S_OK)
				return false;
		}
		else if (GContext->IO.info_device == -1)
		{
			if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
				createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &GContext->viewport->swapChain,
				&GContext->viewport->device, &featureLevel, &GContext->viewport->deviceContext) != S_OK)
				return false;
		}
		else
		{

			if (D3D11CreateDeviceAndSwapChain(adapters[GContext->IO.info_device], D3D_DRIVER_TYPE_UNKNOWN, nullptr,
				createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &GContext->viewport->swapChain,
				&GContext->viewport->device, &featureLevel, &GContext->viewport->deviceContext) != S_OK)
				return false;
		}

		// create render target
		ID3D11Texture2D* pBackBuffer;
		GContext->viewport->swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		GContext->viewport->device->CreateRenderTargetView(pBackBuffer, nullptr, &GContext->viewport->target);
		pBackBuffer->Release();

		//UINT support1;
		//s_pd3dDevice->CheckFormatSupport(DXGI_FORMAT_R8G8B8A8_SINT, &support1);
		//if (support1 & D3D11_FORMAT_SUPPORT_TEXTURE2D)
		//{
		//	int a = 5;
		//}

		return true;
	}

	static void mvCleanupDeviceD3D()
	{
		if (GContext->viewport->target)
		{
			GContext->viewport->target->Release();
			GContext->viewport->target = nullptr;
		}

		if (GContext->viewport->swapChain)
		{
			GContext->viewport->swapChain->Release();
			GContext->viewport->swapChain = nullptr;
		}

		if (GContext->viewport->deviceContext)
		{
			GContext->viewport->deviceContext->Release();
			GContext->viewport->deviceContext = nullptr;
		}

		if (GContext->viewport->device)
		{
			GContext->viewport->device->Release();
			GContext->viewport->device = nullptr;
		}
	}

	static void mvPrerender()
	{
		MV_PROFILE_SCOPE("Viewport prerender")

			mvViewport* viewport = GContext->viewport;

		if (viewport->msg.message == WM_QUIT)
			viewport->running = false;

		if (viewport->posDirty)
		{
			SetWindowPos(viewport->handle, viewport->alwaysOnTop ? HWND_TOPMOST : HWND_TOP, viewport->xpos, viewport->ypos, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
			viewport->posDirty = false;
		}

		if (viewport->sizeDirty)
		{
			SetWindowPos(viewport->handle, viewport->alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, viewport->actualWidth, viewport->actualHeight, SWP_SHOWWINDOW | SWP_NOMOVE);
			viewport->sizeDirty = false;
		}

		if (viewport->modesDirty)
		{
			viewport->modes = WS_OVERLAPPED;

			if (viewport->resizable && viewport->decorated) viewport->modes |= WS_THICKFRAME;
			if (viewport->decorated) viewport->modes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

			SetWindowLongPtr(viewport->handle, GWL_STYLE, viewport->modes);
			SetWindowPos(viewport->handle, viewport->alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			viewport->modesDirty = false;
		}

		if (viewport->titleDirty)
		{
			SetWindowTextA(viewport->handle, viewport->title.c_str());
			viewport->titleDirty = false;
		}

		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&viewport->msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&viewport->msg);
			::DispatchMessage(&viewport->msg);
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

	static void mvPostrender()
	{

		MV_PROFILE_SCOPE("Presentation")

		mvViewport* viewport = GContext->viewport;

		// Rendering
		ImGui::Render();
		viewport->deviceContext->OMSetRenderTargets(1, &viewport->target, nullptr);
		viewport->deviceContext->ClearRenderTargetView(viewport->target, viewport->clearColor);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		static UINT presentFlags = 0;
		if (viewport->swapChain->Present(viewport->vsync ? 1 : 0, presentFlags) == DXGI_STATUS_OCCLUDED)
		{
			presentFlags = DXGI_PRESENT_TEST;
			Sleep(20);
		}
		else
			presentFlags = 0;
	}

	static LRESULT mvHandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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

		case WM_MOVE:
			viewport->xpos = (int)(short)LOWORD(lParam);   // horizontal position 
			viewport->ypos = (int)(short)HIWORD(lParam);   // vertical position 
			break;

		case WM_SIZE:
			if (viewport->device != nullptr && wParam != SIZE_MINIMIZED)
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
				viewport->clientWidth = cwidth;
				viewport->clientHeight = cheight;

				mvOnResize();

				// I believe this are only used for the error logger
				viewport->width = (UINT)LOWORD(lParam);
				viewport->height = (UINT)HIWORD(lParam);

				if (viewport->target)
				{
					viewport->target->Release();
					viewport->target = nullptr;
				}

				if (viewport->decorated)
					viewport->swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				else
					viewport->swapChain->ResizeBuffers(0, (UINT)awidth, (UINT)aheight, DXGI_FORMAT_UNKNOWN, 0);

				// recreate render target
				ID3D11Texture2D* pBackBuffer;
				viewport->swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
				viewport->device->CreateRenderTargetView(pBackBuffer, nullptr, &viewport->target);
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
			viewport->lang_id = LOWORD(lParam);
			break;
		case WM_IME_CHAR:
			if (viewport->lang_id == 0x0412)
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
				if (viewport->previous_ime_char == 0) {
					viewport->previous_ime_char = (BYTE)(wChar & 0x00FF);
					return 0;
				}
				else {
					BYTE b2 = (BYTE)(wChar & 0x00FF);
					wChar = MAKEWORD(viewport->previous_ime_char, b2);
					viewport->previous_ime_char = 0;
				}
			}
			else
			{
				// swap lower and upper part.
				BYTE low = (BYTE)(wChar & 0x00FF);
				BYTE high = (BYTE)((wChar & 0xFF00) >> 8);
				if (viewport->previous_ime_char == 0)
					wChar = MAKEWORD(high, low);
				else
				{
					//wChar = MAKEWORD(previous_ime_char, high, low);
					wChar = MAKEWORD(viewport->previous_ime_char, high);
					viewport->previous_ime_char = low;
				}
			}
			wchar_t ch[6];
			MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR)&wChar, 4, ch, 3);
			io.AddInputCharacter(ch[0]);
			return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	mvViewport* mvCreateViewport(unsigned width, unsigned height)
	{
		mvViewport* viewport = new mvViewport();
		viewport->width = width;
		viewport->height = height;
		return viewport;
	}

	void mvShowViewport(bool minimized, bool maximized)
	{
		mvViewport* viewport = GContext->viewport;
		viewport->wc = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			mvHandleMsg,
			0L,
			0L,
			GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
			_T(viewport->title.c_str()), nullptr
		};
		RegisterClassEx(&viewport->wc);

		mvHandleModes();
		viewport->handle = CreateWindow(viewport->wc.lpszClassName, _T(viewport->title.c_str()),
			viewport->modes,
			viewport->xpos, viewport->ypos, viewport->actualWidth, viewport->actualHeight, nullptr, nullptr, viewport->wc.hInstance, nullptr);

		if (!viewport->small_icon.empty())
		{
			HANDLE hIcon = LoadImage(0, _T(viewport->small_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
			if (hIcon)
			{
				SendMessage(viewport->handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				SendMessage(GetWindow(viewport->handle, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			}
		}

		if (!viewport->large_icon.empty())
		{
			HANDLE hIcon = LoadImage(0, _T(viewport->large_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
			if (hIcon)
			{
				SendMessage(viewport->handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				SendMessage(GetWindow(viewport->handle, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			}
		}

		// Initialize Direct3D
		if (!mvCreateDeviceD3D())
		{
			mvCleanupDeviceD3D();
			::UnregisterClass(viewport->wc.lpszClassName, viewport->wc.hInstance);
		}
		viewport->previous_ime_char = 0;
		HKL hkl = GetKeyboardLayout(0);
		viewport->lang_id = LOWORD(hkl);

		// Show the window

		int cmdShow;

		if (minimized)
			cmdShow = SW_MINIMIZE;
		else if (maximized)
			cmdShow = SW_MAXIMIZE;
		else
			cmdShow = SW_SHOWDEFAULT;

		::ShowWindow(viewport->handle, cmdShow);
		::UpdateWindow(viewport->handle);

		if (viewport->alwaysOnTop)
			SetWindowPos(viewport->handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		imnodes::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		if (GContext->IO.loadIniFile)
		{
			ImGui::LoadIniSettingsFromDisk(GContext->IO.iniFile.c_str());
			io.IniFilename = nullptr;
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
		ImGui_ImplWin32_Init(viewport->handle);
		ImGui_ImplDX11_Init(viewport->device, viewport->deviceContext);
	}

	void mvMaximizeViewport()
	{
		ShowWindow(GContext->viewport->handle, SW_MAXIMIZE);
	}

	void mvMinimizeViewport()
	{
		ShowWindow(GContext->viewport->handle, SW_MINIMIZE);
	}

	void mvCleanupViewport()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		imnodes::DestroyContext();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		mvCleanupDeviceD3D();
		::DestroyWindow(GContext->viewport->handle);
		::UnregisterClass(GContext->viewport->wc.lpszClassName, GContext->viewport->wc.hInstance);
	}

	void mvRenderFrame()
	{
		mvPrerender();
		Render();
		mvPostrender();
	}

}
