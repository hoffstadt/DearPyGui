#include "mvGraphics.h"

#include "mvAppleSpecifics.h"

#include "mvViewport.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"

static void
BuildNearestFilterMetalState(mvGraphics_Metal* graphicsData, MTLPixelFormat colorPixelFormat)
{
    MTLSamplerDescriptor* samplerDesc = [MTLSamplerDescriptor new];
    samplerDesc.minFilter    = MTLSamplerMinMagFilterNearest;
    samplerDesc.magFilter    = MTLSamplerMinMagFilterNearest;
    samplerDesc.mipFilter    = MTLSamplerMipFilterNearest;
    samplerDesc.sAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDesc.tAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDesc.rAddressMode = MTLSamplerAddressModeClampToEdge;
    graphicsData->nearestSampler = [graphicsData->device newSamplerStateWithDescriptor:samplerDesc];

    // Mirrors imgui_impl_metal.mm's shader with a bindable sampler.
    NSString* shaderSource = @""
    "#include <metal_stdlib>\n"
    "using namespace metal;\n"
    "\n"
    "struct Uniforms { float4x4 projectionMatrix; };\n"
    "\n"
    "struct VertexIn {\n"
    "    float2 position  [[attribute(0)]];\n"
    "    float2 texCoords [[attribute(1)]];\n"
    "    uchar4 color     [[attribute(2)]];\n"
    "};\n"
    "\n"
    "struct VertexOut {\n"
    "    float4 position [[position]];\n"
    "    float2 texCoords;\n"
    "    float4 color;\n"
    "};\n"
    "\n"
    "vertex VertexOut dpg_vertex_main(VertexIn in                 [[stage_in]],\n"
    "                                 constant Uniforms &uniforms [[buffer(1)]]) {\n"
    "    VertexOut out;\n"
    "    out.position = uniforms.projectionMatrix * float4(in.position, 0, 1);\n"
    "    out.texCoords = in.texCoords;\n"
    "    out.color = float4(in.color) / float4(255.0);\n"
    "    return out;\n"
    "}\n"
    "\n"
    "fragment half4 dpg_fragment_main(VertexOut in [[stage_in]],\n"
    "                                 texture2d<half, access::sample> texture [[texture(0)]],\n"
    "                                 sampler textureSampler [[sampler(0)]]) {\n"
    "    half4 texColor = texture.sample(textureSampler, in.texCoords);\n"
    "    return half4(in.color) * texColor;\n"
    "}\n";

    NSError* error = nil;
    id<MTLLibrary> library = [graphicsData->device newLibraryWithSource:shaderSource options:nil error:&error];
    if (library == nil)
    {
        NSLog(@"DPG: failed to compile nearest-filter Metal library: %@", error);
        return;
    }

    id<MTLFunction> vertexFunction   = [library newFunctionWithName:@"dpg_vertex_main"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"dpg_fragment_main"];
    if (!vertexFunction || !fragmentFunction)
    {
        NSLog(@"DPG: failed to resolve nearest-filter Metal functions");
        return;
    }

    MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];
    vertexDescriptor.attributes[0].offset      = offsetof(ImDrawVert, pos);
    vertexDescriptor.attributes[0].format      = MTLVertexFormatFloat2;
    vertexDescriptor.attributes[0].bufferIndex = 0;
    vertexDescriptor.attributes[1].offset      = offsetof(ImDrawVert, uv);
    vertexDescriptor.attributes[1].format      = MTLVertexFormatFloat2;
    vertexDescriptor.attributes[1].bufferIndex = 0;
    vertexDescriptor.attributes[2].offset      = offsetof(ImDrawVert, col);
    vertexDescriptor.attributes[2].format      = MTLVertexFormatUChar4;
    vertexDescriptor.attributes[2].bufferIndex = 0;
    vertexDescriptor.layouts[0].stepRate      = 1;
    vertexDescriptor.layouts[0].stepFunction  = MTLVertexStepFunctionPerVertex;
    vertexDescriptor.layouts[0].stride        = sizeof(ImDrawVert);

    MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.vertexFunction   = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.vertexDescriptor = vertexDescriptor;
    pipelineDescriptor.rasterSampleCount                         = 1;
    pipelineDescriptor.colorAttachments[0].pixelFormat           = colorPixelFormat;
    pipelineDescriptor.colorAttachments[0].blendingEnabled       = YES;
    pipelineDescriptor.colorAttachments[0].rgbBlendOperation     = MTLBlendOperationAdd;
    pipelineDescriptor.colorAttachments[0].sourceRGBBlendFactor  = MTLBlendFactorSourceAlpha;
    pipelineDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipelineDescriptor.colorAttachments[0].alphaBlendOperation   = MTLBlendOperationAdd;
    pipelineDescriptor.colorAttachments[0].sourceAlphaBlendFactor= MTLBlendFactorOne;
    pipelineDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipelineDescriptor.depthAttachmentPixelFormat   = MTLPixelFormatInvalid;
    pipelineDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatInvalid;

    graphicsData->nearestPipelineState = [graphicsData->device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
    if (error != nil)
        NSLog(@"DPG: failed to create nearest-filter pipeline state: %@", error);

    static_assert(sizeof(ImDrawVert) == 20, "ImDrawVert layout changed; update DPG nearest-filter pipeline");
}

mvGraphics
setup_graphics(mvViewport &viewport)
{
    mvGraphics graphics{};
    graphics.backendSpecifics = new mvGraphics_Metal();

    auto viewportData = (mvViewportData*)viewport.platformSpecifics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    graphicsData->device = MTLCreateSystemDefaultDevice();
    graphicsData->commandQueue = [graphicsData->device  newCommandQueue];

    ImGui_ImplMetal_Init(graphicsData->device);

    NSWindow *nswin = glfwGetCocoaWindow(viewportData->handle);
    viewportData->layer = [CAMetalLayer layer];
    viewportData->layer.device = graphicsData->device;
    viewportData->layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    nswin.contentView.layer = viewportData->layer;
    nswin.contentView.wantsLayer = YES;

    graphicsData->renderPassDescriptor = [MTLRenderPassDescriptor new];

    BuildNearestFilterMetalState(graphicsData, viewportData->layer.pixelFormat);

    return graphics;
}

void
resize_swapchain(mvGraphics &graphics, int width, int height)
{

}

void
cleanup_graphics(mvGraphics &graphics)
{

}

void
present(mvGraphics &graphics, mvColor &clearColor, bool vsync)
{

}