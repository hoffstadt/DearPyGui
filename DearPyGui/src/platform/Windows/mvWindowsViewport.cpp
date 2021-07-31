#include "mvWindowsViewport.h"
#include "mvFontManager.h"
#include <implot.h>
#include <imnodes.h>
#include <cstdlib>
#include <ctime>
#include "mvToolManager.h"
#include "mvItemRegistry.h"
#include "mvProfiler.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Marvel {

	mvViewport* mvViewport::CreateViewport(unsigned width, unsigned height)
	{
		return new mvWindowsViewport(width, height);
	}

	mvWindowsViewport::mvWindowsViewport(unsigned width, unsigned height)
		: mvViewport(width, height)
	{
	}

	mvWindowsViewport::~mvWindowsViewport()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		imnodes::DestroyContext();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		::DestroyWindow(_hwnd);
		::UnregisterClass(_wc.lpszClassName, _wc.hInstance);
	}

	void mvWindowsViewport::maximize()
	{
		ShowWindow(_hwnd, SW_MAXIMIZE);
	}

	void mvWindowsViewport::minimize()
	{
		ShowWindow(_hwnd, SW_MINIMIZE);
	}

	void mvWindowsViewport::handleModes()
	{
		_modes = WS_OVERLAPPED;

		if (_resizable && _decorated) _modes |= WS_THICKFRAME;
		if (_decorated) _modes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	}

	void mvWindowsViewport::show(bool minimized, bool maximized)
	{

		_wc = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			HandleMsgSetup,
			0L,
			0L,
			GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
			_T(_title.c_str()), nullptr };
		RegisterClassEx(&_wc);

		handleModes();
		_hwnd = CreateWindow(_wc.lpszClassName, _T(_title.c_str()),
			_modes,
			_xpos, _ypos, _actualWidth, _actualHeight, nullptr, nullptr, _wc.hInstance, this);

		if (!_small_icon.empty())
		{
			HANDLE hIcon = LoadImage(0, _T(_small_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
			if (hIcon) 
			{
				SendMessage(_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				SendMessage(GetWindow(_hwnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			}
		}

		if (!_large_icon.empty())
		{
			HANDLE hIcon = LoadImage(0, _T(_large_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
			if (hIcon) 
			{
				SendMessage(_hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				SendMessage(GetWindow(_hwnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			}
		}

		// Initialize Direct3D
		if (!CreateDeviceD3D(_hwnd))
		{
			CleanupDeviceD3D();
			::UnregisterClass(_wc.lpszClassName, _wc.hInstance);
		}
		previous_ime_char = 0;
		HKL hkl = GetKeyboardLayout(0);
		lang_id = LOWORD(hkl);

		// Show the window

		int cmdShow;

		if (minimized)
			cmdShow = SW_MINIMIZE;
		else if (maximized)
			cmdShow = SW_MAXIMIZE;
		else
			cmdShow = SW_SHOWDEFAULT;

		::ShowWindow(_hwnd, cmdShow);
		::UpdateWindow(_hwnd);

		if (_alwaysOnTop)
			SetWindowPos(_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		imnodes::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		if (mvApp::GetApp()->_loadIniFile)
		{
			ImGui::LoadIniSettingsFromDisk(mvApp::GetApp()->_iniFile.c_str());
			io.IniFilename = nullptr;
		}
		else
		{
			if (mvApp::GetApp()->_iniFile.empty())
				io.IniFilename = nullptr;
			else
				io.IniFilename = mvApp::GetApp()->_iniFile.c_str();
		}

		if(mvApp::GetApp()->_docking)
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		mvApp::SetDefaultTheme();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(_hwnd);
		ImGui_ImplDX11_Init(s_pd3dDevice, s_pd3dDeviceContext);
	}

	void mvWindowsViewport::setup()
	{
		ZeroMemory(&_msg, sizeof(_msg));
	}

	void mvWindowsViewport::prerender()
	{
		MV_PROFILE_SCOPE("Viewport prerender")

		if (_msg.message == WM_QUIT)
			_running = false;

		if (_posDirty)
		{
			SetWindowPos(_hwnd, _alwaysOnTop ? HWND_TOPMOST : HWND_TOP, _xpos, _ypos, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
			_posDirty = false;
		}

		if (_sizeDirty)
		{
			SetWindowPos(_hwnd, _alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, _actualWidth, _actualHeight, SWP_SHOWWINDOW | SWP_NOMOVE);
			_sizeDirty = false;
		}

		if (_modesDirty)
		{
			handleModes();
			SetWindowLongPtr(_hwnd, GWL_STYLE, _modes);
			SetWindowPos(_hwnd, _alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			_modesDirty = false;
		}

		if (_titleDirty)
		{
			SetWindowTextA(_hwnd, _title.c_str());
			_titleDirty = false;
		}

		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&_msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&_msg);
			::DispatchMessage(&_msg);
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

	void mvWindowsViewport::renderFrame()
	{
		prerender();
		_app->render();
		postrender();
	}

	void mvWindowsViewport::postrender()
	{

		MV_PROFILE_SCOPE("Presentation")

		// Rendering
		ImGui::Render();
		s_pd3dDeviceContext->OMSetRenderTargets(1, &s_mainRenderTargetView, nullptr);
		s_pd3dDeviceContext->ClearRenderTargetView(s_mainRenderTargetView, _clearColor);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		static UINT presentFlags = 0;
		if (s_pSwapChain->Present(_vsync ? 1 : 0, presentFlags) == DXGI_STATUS_OCCLUDED) 
		{
			presentFlags = DXGI_PRESENT_TEST;
			Sleep(20);
		}
		else
			presentFlags = 0;
	}

	bool mvWindowsViewport::CreateDeviceD3D(HWND hWnd)
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
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &s_pSwapChain,
			&s_pd3dDevice, &featureLevel, &s_pd3dDeviceContext) != S_OK)
			return false;

		CreateRenderTarget();

		//UINT support1;
		//s_pd3dDevice->CheckFormatSupport(DXGI_FORMAT_R8G8B8A8_SINT, &support1);
		//if (support1 & D3D11_FORMAT_SUPPORT_TEXTURE2D)
		//{
		//	int a = 5;
		//}


		return true;
	}

	void mvWindowsViewport::CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (s_pSwapChain)
		{
			s_pSwapChain->Release();
			s_pSwapChain = nullptr;
		}

		if (s_pd3dDeviceContext)
		{
			s_pd3dDeviceContext->Release();
			s_pd3dDeviceContext = nullptr;
		}

		if (s_pd3dDevice)
		{
			s_pd3dDevice->Release();
			s_pd3dDevice = nullptr;
		}
	}

	void mvWindowsViewport::CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		s_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		s_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &s_mainRenderTargetView);
		pBackBuffer->Release();
	}

	void mvWindowsViewport::CleanupRenderTarget()
	{
		if (s_mainRenderTargetView)
		{
			s_mainRenderTargetView->Release();
			s_mainRenderTargetView = nullptr;
		}
	}

	LRESULT CALLBACK mvWindowsViewport::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			auto const pWnd = static_cast<mvWindowsViewport*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window instance
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&mvWindowsViewport::HandleMsgThunk));
			// forward message to window instance handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK mvWindowsViewport::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// retrieve ptr to window instance
		auto const pWnd = reinterpret_cast<mvWindowsViewport*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT mvWindowsViewport::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		switch (msg)
		{
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = _minwidth;
			lpMMI->ptMinTrackSize.y = _minheight;
			lpMMI->ptMaxTrackSize.x = _maxwidth;
			lpMMI->ptMaxTrackSize.y = _maxheight;
			break;
		}

		case WM_MOVE:
			_xpos = (int)(short)LOWORD(lParam);   // horizontal position 
			_ypos = (int)(short)HIWORD(lParam);   // vertical position 
			break;

		case WM_SIZE:
			if (s_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
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

				_actualWidth = awidth;
				_actualHeight = aheight;
				_clientWidth = cwidth;
				_clientHeight = cheight;

				onResizeEvent();

				// I believe this are only used for the error logger
				_width = (UINT)LOWORD(lParam);
				_height = (UINT)HIWORD(lParam);

				CleanupRenderTarget();
				if(_decorated)
					s_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				else
					s_pSwapChain->ResizeBuffers(0, (UINT)awidth, (UINT)aheight, DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			mvApp::StopApp();
			::PostQuitMessage(0);
			return 0;
		case WM_INPUTLANGCHANGE:
			lang_id=LOWORD(lParam);			
			break;
        case WM_IME_CHAR:
			if (lang_id==0x0412)
				break;
			auto& io = ImGui::GetIO();
			DWORD wChar = (DWORD)wParam;
			if (wChar <= 127)
			{
				io.AddInputCharacter(wChar);
				return 0;
			}
			else if(wChar<0xFF){
			//GBK also supports Traditional Chinese, Japanese, English, Russian and (partially) Greek.
			//When input Japanese with MS Japanese IME under CP936, dual bytes of one Japanese char are send within one or two wParam in succession.
				if (previous_ime_char==0){ 
					previous_ime_char=(BYTE)(wChar & 0x00FF);
					return 0;
				}
				else{
					BYTE b2=(BYTE)(wChar & 0x00FF);
					wChar = MAKEWORD( previous_ime_char,b2);
					previous_ime_char=0;
				}
			}
			else
			{
				// swap lower and upper part.
				BYTE low = (BYTE)(wChar & 0x00FF);
				BYTE high = (BYTE)((wChar & 0xFF00) >> 8);
				if (previous_ime_char==0)
					wChar = MAKEWORD(high, low);
				else
				{
					//wChar = MAKEWORD(previous_ime_char, high, low);
					wChar = MAKEWORD(previous_ime_char, high);
					previous_ime_char=low;
				}
			}
			wchar_t ch[6];
			MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR)&wChar, 4, ch, 3);
			io.AddInputCharacter(ch[0]);			
			return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	ID3D11Device* mvWindowsViewport::s_pd3dDevice = nullptr;
	ID3D11DeviceContext* mvWindowsViewport::s_pd3dDeviceContext = nullptr;
	IDXGISwapChain* mvWindowsViewport::s_pSwapChain = nullptr;
	ID3D11RenderTargetView* mvWindowsViewport::s_mainRenderTargetView = nullptr;

}
