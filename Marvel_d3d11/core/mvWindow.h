#pragma once
#include "mvMarvelWin.h"
#include <optional>
#include <vector>


namespace Marvel {

	class mvWindow
	{

	public:

		static std::optional<int> ProcessMessages();

	public:

		mvWindow(const char* name, int width, int height);
		~mvWindow();

		HWND getHandle() const { return m_hWnd; }

	private:

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:

		static constexpr const char* s_wndClassName = "Marvel Window";

		int       m_width;
		int       m_height;
		HWND      m_hWnd;
		HINSTANCE m_hInst;
		
	};

}