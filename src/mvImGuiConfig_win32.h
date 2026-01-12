//---- Use FreeType to build and rasterize the font atlas (instead of stb_truetype which is embedded by default in Dear ImGui)
#define IMGUI_ENABLE_FREETYPE

//---- Use 32-bit vertex indices (default is 16-bit) is one way to allow large meshes with more than 64K vertices.
// Your renderer backend will need to support it (most example renderer backends support both 16/32-bit indices).
// Another way to allow large meshes while keeping 16-bit indices is to handle ImDrawCmd::VtxOffset in your renderer.
// Read about ImGuiBackendFlags_RendererHasVtxOffset for details.
#define ImDrawIdx unsigned int

#define IMGUI_USE_WCHAR32

// Note: ImTextureID underlying type is backend-specific and is therefore different on different platforms!
// We're using void* so that we can pass ID3D11ShaderResourceView* here.  While we could
// define ImTextureID as ID3D11ShaderResourceView* right here, we'd like to avoid including
// the entire d3d11.h (and windows.h) into this header because it is in turn included into imgui.h.
#define ImTextureID void*
