#pragma once

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
#include <dwmapi.h>

struct mvViewportData
{
	HWND       handle = nullptr;
	WNDCLASSEX wc;
	MSG        msg;
	DWORD      modes;
};

struct mvGraphics_D3D11
{
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* target = nullptr;
};