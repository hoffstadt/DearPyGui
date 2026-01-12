//---- Use 32-bit vertex indices (default is 16-bit) is one way to allow large meshes with more than 64K vertices.
// Your renderer backend will need to support it (most example renderer backends support both 16/32-bit indices).
// Another way to allow large meshes while keeping 16-bit indices is to handle ImDrawCmd::VtxOffset in your renderer.
// Read about ImGuiBackendFlags_RendererHasVtxOffset for details.
#define ImDrawIdx unsigned int

#define IMGUI_USE_WCHAR32

// Note: ImTextureID underlying type is backend-specific and is therefore different on different platforms!
// Leaving it defined as void* until we figure out how to declare it properly (like, id<MTLTexture>?).
// At least void* has been working fine for years.
#define ImTextureID void*
