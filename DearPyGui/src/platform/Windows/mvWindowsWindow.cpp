#include "Platform/Windows/mvWindowsWindow.h"
#include "mvApp.h"
#include "implot.h"
#include "imnodes.h"
#include "mvDataStorage.h"
#include <cstdlib>
#include <ctime>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Marvel {

	mvWindow* mvWindow::CreatemvWindow(unsigned width, unsigned height, bool error)
	{
		return new mvWindowsWindow(width, height, error);
	}

	mvWindowsWindow::mvWindowsWindow(unsigned width, unsigned height, bool error)
		: mvWindow(width, height, error)
	{
		m_clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

		m_wc = { 
			sizeof(WNDCLASSEX), 
			CS_CLASSDC, 
			HandleMsgSetup, 
			0L, 
			0L, 
			GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, 
			_T(mvApp::GetApp()->m_title.c_str()), nullptr };
		RegisterClassEx(&m_wc);

		if(mvApp::GetApp()->getResizable())
			m_hwnd = CreateWindow(m_wc.lpszClassName, _T(mvApp::GetApp()->m_title.c_str()), WS_OVERLAPPEDWINDOW, mvApp::GetApp()->m_mainXPos,
				mvApp::GetApp()->m_mainYPos, width, height, nullptr, nullptr, m_wc.hInstance, this);
		else
			m_hwnd = CreateWindow(m_wc.lpszClassName, _T(mvApp::GetApp()->m_title.c_str()),
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 
				mvApp::GetApp()->m_mainXPos, mvApp::GetApp()->m_mainYPos, width, height, nullptr, nullptr, m_wc.hInstance, this);

		// Initialize Direct3D
		if (!CreateDeviceD3D(m_hwnd))
		{
			CleanupDeviceD3D();
			::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
		}
		previous_ime_char=0;
		HKL hkl=GetKeyboardLayout(0);		
		lang_id=LOWORD(hkl);

	}

	mvWindowsWindow::~mvWindowsWindow()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		imnodes::Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		::DestroyWindow(m_hwnd);
		::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
	}

	void mvWindowsWindow::setWindowText(const std::string& name)
	{
		SetWindowTextA(m_hwnd, name.c_str());
	}

	void mvWindowsWindow::show()
	{

		// Show the window
		::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
		::UpdateWindow(m_hwnd);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		imnodes::Initialize();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.IniFilename = nullptr;

		if(mvApp::GetApp()->m_docking)
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		
		if (mvApp::GetApp()->m_dockingShiftOnly)
			io.ConfigDockingWithShift = true;

		setupFonts();

		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("C:/Users/Jonathan Hoffstadt/Desktop/calibrili.ttf", 13.0f);
		//io.Fonts->Build();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(m_hwnd);
		ImGui_ImplDX11_Init(s_pd3dDevice, s_pd3dDeviceContext);
	}

	void mvWindowsWindow::setup()
	{
		ZeroMemory(&m_msg, sizeof(m_msg));
	}

	void mvWindowsWindow::prerender()
	{
		if (m_msg.message == WM_QUIT)
			m_running = false;

		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&m_msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&m_msg);
			::DispatchMessage(&m_msg);
			//continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void mvWindowsWindow::renderFrame()
	{
		prerender();

		if (m_error)
		{
			mvAppLog::setSize(m_width, m_height);
			mvAppLog::render();
		}

		else
			m_app->render();

		postrender();
	}

	void mvWindowsWindow::run()
	{

		setup();
		while (m_running)
			renderFrame();

	}

	void mvWindowsWindow::postrender()
	{

		ImVec4 clear_color = m_clearColor;

		// Rendering
		ImGui::Render();
		s_pd3dDeviceContext->OMSetRenderTargets(1, &s_mainRenderTargetView, nullptr);
		s_pd3dDeviceContext->ClearRenderTargetView(s_mainRenderTargetView, (float*)&clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//s_pSwapChain->Present(1, 0); // Present with vsync
		//s_pSwapChain->Present(0, 0); // Present without vsync

		static UINT presentFlags = 0;
		if (s_pSwapChain->Present(mvApp::GetApp()->getVSync() ? 1 : 0, presentFlags) == DXGI_STATUS_OCCLUDED) {
			presentFlags = DXGI_PRESENT_TEST;
			Sleep(20);
		}
		else {
			presentFlags = 0;
		}
	}

	void mvWindowsWindow::cleanup()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		imnodes::Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		::DestroyWindow(m_hwnd);
		::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
	}

	bool mvWindowsWindow::CreateDeviceD3D(HWND hWnd)
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
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &s_pSwapChain,
			&s_pd3dDevice, &featureLevel, &s_pd3dDeviceContext) != S_OK)
			return false;

		CreateRenderTarget();
		return true;
	}

	void mvWindowsWindow::CleanupDeviceD3D()
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

	void mvWindowsWindow::CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		s_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		s_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &s_mainRenderTargetView);
		pBackBuffer->Release();
	}

	void mvWindowsWindow::CleanupRenderTarget()
	{
		if (s_mainRenderTargetView)
		{
			s_mainRenderTargetView->Release();
			s_mainRenderTargetView = nullptr;
		}
	}

	LRESULT CALLBACK mvWindowsWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			auto const pWnd = static_cast<mvWindowsWindow*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window instance
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&mvWindowsWindow::HandleMsgThunk));
			// forward message to window instance handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK mvWindowsWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// retrieve ptr to window instance
		auto const pWnd = reinterpret_cast<mvWindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT mvWindowsWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		switch (msg)
		{
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

				mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
					CreateEventArgument("actual_width", awidth),
					CreateEventArgument("actual_height", aheight),
					CreateEventArgument("client_width", cwidth),
					CreateEventArgument("client_height", cheight)
					});

				// I believe this are only used for the error logger
				m_width = (UINT)LOWORD(lParam);
				m_height = (UINT)HIWORD(lParam);

				CleanupRenderTarget();
				s_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
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
			DWORD wChar = wParam;
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
				else{
					wChar = MAKEWORD(previous_ime_char, high, low);
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

	ID3D11Device* mvWindowsWindow::s_pd3dDevice = nullptr;
	ID3D11DeviceContext* mvWindowsWindow::s_pd3dDeviceContext = nullptr;
	IDXGISwapChain* mvWindowsWindow::s_pSwapChain = nullptr;
	ID3D11RenderTargetView* mvWindowsWindow::s_mainRenderTargetView = nullptr;

}