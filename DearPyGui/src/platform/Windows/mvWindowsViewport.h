#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "mvViewport.h"

namespace Marvel {

	class mvWindowsViewport : public mvViewport
	{

	public:

		mvWindowsViewport(unsigned width, unsigned height, bool error = false);
		~mvWindowsViewport() override;

		void show       (bool minimized, bool maximized) override;
		void setup      () override;
		void prerender  () override;
		void postrender () override;
		void renderFrame() override;
		void cleanup    () override;
		void maximize   () override;
		void minimize   () override;
		void handleModes() override;
		void setWindowText(const std::string& name) override;

	private:

		bool CreateDeviceD3D(HWND hWnd);

		void CleanupDeviceD3D();

		static void CreateRenderTarget();

		static void CleanupRenderTarget();

	public:

		static ID3D11Device* getDevice() { return s_pd3dDevice; }

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:

		HWND m_hwnd;
		WNDCLASSEX m_wc;
		MSG m_msg;
		DWORD m_modes;
		ImVec4 m_clearColor;

	private:

		// Data
		static ID3D11Device* s_pd3dDevice;
		static ID3D11DeviceContext* s_pd3dDeviceContext;
		static IDXGISwapChain* s_pSwapChain;
		static ID3D11RenderTargetView* s_mainRenderTargetView;
		BYTE previous_ime_char;
		WORD lang_id;
	};

}
