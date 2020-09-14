#include "mvWindowsWindow.h"
#include "implot.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


mvWindowsWindow::mvWindowsWindow()
{
	m_wc = { sizeof(WNDCLASSEX), CS_CLASSDC, HandleMsgSetup, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	RegisterClassEx(&m_wc);

	m_hwnd = CreateWindow(m_wc.lpszClassName, _T("Dear ImGui DirectX11 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, m_wc.hInstance, NULL);


	// Initialize Direct3D
	if (!CreateDeviceD3D(m_hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
	}

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;

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
	if (::PeekMessage(&m_msg, NULL, 0U, 0U, PM_REMOVE))
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

void mvWindowsWindow::postrender()
{

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Rendering
	ImGui::Render();
	s_pd3dDeviceContext->OMSetRenderTargets(1, &s_mainRenderTargetView, NULL);
	s_pd3dDeviceContext->ClearRenderTargetView(s_mainRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	s_pSwapChain->Present(1, 0); // Present with vsync
	//g_pSwapChain->Present(0, 0); // Present without vsync
}

void mvWindowsWindow::cleanup()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
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
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
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
		s_pSwapChain = NULL;
	}

	if (s_pd3dDeviceContext)
	{
		s_pd3dDeviceContext->Release();
		s_pd3dDeviceContext = NULL;
	}

	if (s_pd3dDevice)
	{
		s_pd3dDevice->Release();
		s_pd3dDevice = NULL;
	}
}

void mvWindowsWindow::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	s_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	s_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &s_mainRenderTargetView);
	pBackBuffer->Release();
}

void mvWindowsWindow::CleanupRenderTarget()
{
	if (s_mainRenderTargetView)
	{
		s_mainRenderTargetView->Release();
		s_mainRenderTargetView = NULL;
	}
}


LRESULT CALLBACK mvWindowsWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		mvWindowsWindow* const pWnd = static_cast<mvWindowsWindow*>(pCreate->lpCreateParams);
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
	mvWindowsWindow* const pWnd = reinterpret_cast<mvWindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
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
		if (s_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
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
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


ID3D11Device* mvWindowsWindow::s_pd3dDevice = NULL;
ID3D11DeviceContext* mvWindowsWindow::s_pd3dDeviceContext = NULL;
IDXGISwapChain* mvWindowsWindow::s_pSwapChain = NULL;
ID3D11RenderTargetView* mvWindowsWindow::s_mainRenderTargetView = NULL;