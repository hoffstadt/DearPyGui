#include "mvGraphics.h"
#include "mvWindowsSpecifics.h"

static std::vector <IDXGIAdapter*>
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

mvGraphics
setup_graphics(mvViewport& viewport)
{
	mvGraphics graphics{};
	graphics.backendSpecifics = new mvGraphics_D3D11();

	mvViewportData* viewportData = (mvViewportData*)viewport.platformSpecifics;
	mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)graphics.backendSpecifics;

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
	sd.OutputWindow = viewportData->handle;
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
			createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &graphicsData->swapChain,
			&graphicsData->device, &featureLevel, &graphicsData->deviceContext) != S_OK)
			return graphics;
	}
	else if (GContext->IO.info_device == -1)
	{
		if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &graphicsData->swapChain,
			&graphicsData->device, &featureLevel, &graphicsData->deviceContext) != S_OK)
			return graphics;
	}
	else
	{

		if (D3D11CreateDeviceAndSwapChain(adapters[GContext->IO.info_device], D3D_DRIVER_TYPE_UNKNOWN, nullptr,
			createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &graphicsData->swapChain,
			&graphicsData->device, &featureLevel, &graphicsData->deviceContext) != S_OK)
			return graphics;
	}

	// create render target
	graphicsData->swapChain->GetBuffer(0, IID_PPV_ARGS(&graphicsData->backBuffer));
	graphicsData->device->CreateRenderTargetView(graphicsData->backBuffer, nullptr, &graphicsData->target);

	graphics.ok = true;

	ImGui_ImplDX11_Init(graphicsData->device, graphicsData->deviceContext);

	return graphics;
}

void
cleanup_graphics(mvGraphics& graphics)
{
	mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)graphics.backendSpecifics;

	ImGui_ImplDX11_Shutdown();

	if (graphicsData->target)
	{
		graphicsData->target->Release();
		graphicsData->target = nullptr;
	}

	if (graphicsData->backBuffer)
	{
		graphicsData->backBuffer->Release();
		graphicsData->backBuffer = nullptr;
	}

	if (graphicsData->swapChain)
	{
		graphicsData->swapChain->Release();
		graphicsData->swapChain = nullptr;
	}

	if (graphicsData->deviceContext)
	{
		graphicsData->deviceContext->Release();
		graphicsData->deviceContext = nullptr;
	}

	if (graphicsData->device)
	{
		graphicsData->device->Release();
		graphicsData->device = nullptr;
	}

	delete graphicsData;
	graphicsData = nullptr;

}

void
resize_swapchain(mvGraphics& graphics, int width, int height)
{
	mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)graphics.backendSpecifics;


	if (graphicsData->target)
	{
		graphicsData->target->Release();
		graphicsData->target = nullptr;
	}

	if (graphicsData->backBuffer)
	{
		graphicsData->backBuffer->Release();
		graphicsData->backBuffer = nullptr;
	}

	graphicsData->swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);

	// recreate render target
	graphicsData->swapChain->GetBuffer(0, IID_PPV_ARGS(&graphicsData->backBuffer));
	graphicsData->device->CreateRenderTargetView(graphicsData->backBuffer, nullptr, &graphicsData->target);
}

void
present(mvGraphics& graphics, mvColor& clearColor, bool vsync)
{
	MV_PROFILE_SCOPE("Presentation")

	mvGraphics_D3D11* graphicsData = (mvGraphics_D3D11*)graphics.backendSpecifics;

	// Rendering
	ImGui::Render();
	graphicsData->deviceContext->OMSetRenderTargets(1, &graphicsData->target, nullptr);
	graphicsData->deviceContext->ClearRenderTargetView(graphicsData->target, clearColor);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	static UINT presentFlags = 0;
	if (graphicsData->swapChain->Present(vsync ? 1 : 0, presentFlags) == DXGI_STATUS_OCCLUDED)
	{
		presentFlags = DXGI_PRESENT_TEST;
		Sleep(20);
	}
	else
		presentFlags = 0;
}