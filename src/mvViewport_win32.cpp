#include "mvWindowsSpecifics.h"


static BYTE gprevious_ime_char;
static WORD glang_id;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int
get_horizontal_shift(const HWND window_handle)
{
	RECT window_rectangle, frame_rectangle;
	GetWindowRect(window_handle, &window_rectangle);
	if (DwmGetWindowAttribute(window_handle,
		DWMWA_EXTENDED_FRAME_BOUNDS, &frame_rectangle, sizeof(RECT)) != S_OK)
	{
		return 0;
	}

	return frame_rectangle.left - window_rectangle.left;
}

static void
mvHandleModes(mvViewport& viewport)
{
	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;
	viewportData->modes = WS_OVERLAPPED;

	if (viewport.resizable && viewport.decorated) viewportData->modes |= WS_THICKFRAME;
	if (viewport.decorated) {
		viewportData->modes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}
	else {
		viewportData->modes |= WS_POPUP;
	}

}

static void
mvPrerender(mvViewport& viewport)
{
	MV_PROFILE_SCOPE("Viewport prerender")

		mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;

	if (viewportData->msg.message == WM_QUIT)
		viewport.running = false;

	{
		// TODO: we probably need a separate mutex for this
		std::lock_guard<std::recursive_mutex> lk(GContext->mutex);

		if (viewport.posDirty)
		{
			int horizontal_shift = get_horizontal_shift(viewportData->handle);
			SetWindowPos(viewportData->handle, viewport.alwaysOnTop ? HWND_TOPMOST : HWND_TOP, viewport.xpos - horizontal_shift, viewport.ypos, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
			viewport.posDirty = false;
		}

		if (viewport.sizeDirty)
		{
			SetWindowPos(viewportData->handle, viewport.alwaysOnTop ? HWND_TOPMOST : HWND_TOP, 0, 0, viewport.actualWidth, viewport.actualHeight, SWP_SHOWWINDOW | SWP_NOMOVE);
			viewport.sizeDirty = false;
		}

		if (viewport.modesDirty)
		{
			viewportData->modes = WS_OVERLAPPED;

			if (viewport.resizable && viewport.decorated) viewportData->modes |= WS_THICKFRAME;
			if (viewport.decorated) {
				viewportData->modes |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
			}
			else {
				viewportData->modes |= WS_POPUP;
			}

			SetWindowLongPtr(viewportData->handle, GWL_STYLE, viewportData->modes);
			SetWindowPos(viewportData->handle, viewport.alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			viewport.modesDirty = false;
		}

		if (viewport.titleDirty)
		{
			SetWindowTextA(viewportData->handle, viewport.title.c_str());
			viewport.titleDirty = false;
		}
	}

	// Poll and handle messages (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

	if (GContext->IO.waitForInput)
		::WaitMessage();

	if (::PeekMessage(&viewportData->msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&viewportData->msg);
		::DispatchMessage(&viewportData->msg);
		//continue;
	}

	{
		// Font manager is thread-unsafe, so we'd better sync it
		std::lock_guard<std::recursive_mutex> lk(GContext->mutex);

		if (mvToolManager::GetFontManager().isInvalid())
		{
			mvToolManager::GetFontManager().rebuildAtlas();
			ImGui_ImplDX11_InvalidateDeviceObjects();
			mvToolManager::GetFontManager().updateAtlas();
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

static LRESULT
mvHandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	static UINT_PTR puIDEvent = 0;

	mvViewport* viewport = GContext->viewport;
	mvGraphics& graphics = GContext->graphics;
	mvViewportData* viewportData = (mvViewportData*)viewport->platformSpecifics;
	mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)graphics.backendSpecifics;

	switch (msg)
	{

	case WM_PAINT:
	{
		if (GContext->frame > 0)
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

			{
				std::lock_guard<std::recursive_mutex> lk(GContext->mutex);

				viewport->actualWidth = awidth;
				viewport->actualHeight = aheight;


				GContext->viewport->clientHeight = cheight;
				GContext->viewport->clientWidth = cwidth;

				//GContext->viewport->resized = true;
				mvOnResize();
				GContext->viewport->resized = false;

				if (mvToolManager::GetFontManager().isInvalid())
				{
					mvToolManager::GetFontManager().rebuildAtlas();
					ImGui_ImplDX11_InvalidateDeviceObjects();
					mvToolManager::GetFontManager().updateAtlas();
				}
			}
			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			Render();
			present(graphics, GContext->viewport->clearColor, GContext->viewport->vsync);
		}
		// must be called for the OS to do its thing
		PAINTSTRUCT tPaint;
		HDC tDeviceContext = BeginPaint(hWnd, &tPaint);
		EndPaint(hWnd, &tPaint);
		break;
	}

	case WM_GETMINMAXINFO:
	{
		// TODO: lock the mutex?

		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = viewport->minwidth;
		lpMMI->ptMinTrackSize.y = viewport->minheight;
		lpMMI->ptMaxTrackSize.x = viewport->maxwidth;
		lpMMI->ptMaxTrackSize.y = viewport->maxheight;
		break;
	}

	case WM_MOVING:
	{
		std::lock_guard<std::recursive_mutex> lk(GContext->mutex);

		int horizontal_shift = get_horizontal_shift(viewportData->handle);
		RECT rect = *(RECT*)(lParam);
		viewport->xpos = rect.left + horizontal_shift;
		viewport->ypos = rect.top;
		break;
	}

	case WM_SIZE:
	case WM_SIZING:

		if (graphicsData != nullptr && wParam != SIZE_MINIMIZED)
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

			std::lock_guard<std::recursive_mutex> lk(GContext->mutex);

			viewport->actualWidth = awidth;
			viewport->actualHeight = aheight;


			if (viewport->decorated)
			{
				GContext->viewport->clientHeight = cheight;
				GContext->viewport->clientWidth = cwidth;
			}
			else
			{
				GContext->viewport->clientHeight = cheight;
				GContext->viewport->clientWidth = cwidth;
			}

			GContext->viewport->resized = true;
			//mvOnResize();

			// I believe this are only used for the error logger
			viewport->width = (UINT)LOWORD(lParam);
			viewport->height = (UINT)HIWORD(lParam);

			if (viewport->decorated)
				resize_swapchain(graphics, (int)(UINT)LOWORD(lParam), (int)(UINT)HIWORD(lParam));
			else
				resize_swapchain(graphics, awidth, aheight);
		}
		return 0;

	case WM_ENTERSIZEMOVE:
	{
		// DefWindowProc below will block until mouse is released or moved.
		// Timer events can still be caught so here we add a timer so we
		// can continue rendering when catching the WM_TIMER event.
		// Timer is killed in the WM_EXITSIZEMOVE case below.
		puIDEvent = SetTimer(NULL, puIDEvent, USER_TIMER_MINIMUM, NULL);
		SetTimer(hWnd, puIDEvent, USER_TIMER_MINIMUM, NULL);
		break;
	}

	case WM_EXITSIZEMOVE:
	{
		KillTimer(hWnd, puIDEvent);
		break;
	}

	case WM_TIMER:
	{
		if (wParam == puIDEvent)
			mvOnResize();
		break;
	}
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_CLOSE:
		if (GContext->viewport->disableClose) {
			mvSubmitCallback([=]() {
				mvRunCallback(GContext->callbackRegistry->onCloseCallback, 0, nullptr, GContext->callbackRegistry->onCloseCallbackUserData);
				});
			return 0;
		}
		GContext->started = false;
		DestroyWindow(hWnd);
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

mvViewport*
mvCreateViewport(unsigned width, unsigned height)
{
	mvViewport* viewport = new mvViewport();
	viewport->width = width;
	viewport->height = height;
	viewport->platformSpecifics = new mvViewportData();
	return viewport;
}

void
mvShowViewport(mvViewport& viewport, bool minimized, bool maximized)
{
	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;
	viewportData->wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		mvHandleMsg,
		0L,
		0L,
		GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
		_T(viewport.title.c_str()), nullptr
	};
	RegisterClassEx(&viewportData->wc);

	mvHandleModes(viewport);
	viewportData->handle = CreateWindow(viewportData->wc.lpszClassName, _T(viewport.title.c_str()),
		viewportData->modes,
		viewport.xpos, viewport.ypos,
		viewport.actualWidth, viewport.actualHeight,
		nullptr, nullptr, viewportData->wc.hInstance, nullptr);

	viewport.clientHeight = viewport.actualHeight;
	viewport.clientWidth = viewport.actualWidth;

	if (!viewport.small_icon.empty())
	{
		HANDLE hIcon = LoadImage(0, _T(viewport.small_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (hIcon)
		{
			SendMessage(viewportData->handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			SendMessage(GetWindow(viewportData->handle, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		}
	}

	if (!viewport.large_icon.empty())
	{
		HANDLE hIcon = LoadImage(0, _T(viewport.large_icon.c_str()), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (hIcon)
		{
			SendMessage(viewportData->handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			SendMessage(GetWindow(viewportData->handle, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
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

	::ShowWindow(viewportData->handle, cmdShow);
	::UpdateWindow(viewportData->handle);

	if (viewport.alwaysOnTop)
		SetWindowPos(viewportData->handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	if (GContext->IO.loadIniFile)
	{
		ImGui::LoadIniSettingsFromDisk(GContext->IO.iniFile.c_str());
		io.IniFilename = nullptr;
		if (GContext->IO.autoSaveIniFile)
			io.IniFilename = GContext->IO.iniFile.c_str();
	}
	else
	{
		if (GContext->IO.iniFile.empty())
			io.IniFilename = nullptr;
		else
			io.IniFilename = GContext->IO.iniFile.c_str();
	}

	if(GContext->IO.kbdNavigation)
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	SetDefaultTheme();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(viewportData->handle);

}

void
mvMaximizeViewport(mvViewport& viewport)
{
	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;
	ShowWindow(viewportData->handle, SW_MAXIMIZE);
}

void
mvMinimizeViewport(mvViewport& viewport)
{
	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;
	ShowWindow(viewportData->handle, SW_MINIMIZE);
}

void
mvCleanupViewport(mvViewport& viewport)
{
	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;
	ImGui_ImplWin32_Shutdown();
	::DestroyWindow(viewportData->handle);
	::UnregisterClass(viewportData->wc.lpszClassName, viewportData->wc.hInstance);
}

void
mvRenderFrame()
{
	mvPrerender(*GContext->viewport);
	Render();
	present(GContext->graphics, GContext->viewport->clearColor, GContext->viewport->vsync);
}

void
mvToggleFullScreen(mvViewport& viewport)
{
	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;

	static size_t storedWidth = 0;
	static size_t storedHeight = 0;
	static int    storedXPos = 0;
	static int    storedYPos = 0;

	size_t width = GetSystemMetrics(SM_CXSCREEN);
	size_t height = GetSystemMetrics(SM_CYSCREEN);

	if (viewport.fullScreen)
	{
		RECT rect;
		rect.left = storedXPos;
		rect.top = storedYPos;
		rect.right = storedXPos + storedWidth;
		rect.bottom = storedYPos + storedHeight;
		SetWindowLongPtr(viewportData->handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow(viewportData->handle, storedXPos, storedYPos, storedWidth, storedHeight, TRUE);
		GContext->viewport->fullScreen = false;
	}
	else
	{
		storedWidth = GContext->viewport->actualWidth;
		storedHeight = GContext->viewport->actualHeight;
		storedXPos = GContext->viewport->xpos;
		storedYPos = GContext->viewport->ypos;

		SetWindowLongPtr(viewportData->handle, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
		MoveWindow(viewportData->handle, 0, 0, width, height, TRUE);
		GContext->viewport->fullScreen = true;
	}
}