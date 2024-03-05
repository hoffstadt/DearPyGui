#include "mvGraphics.h"
#include "mvLinuxSpecifics.h"
#include "mvViewport.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "mvProfiler.h"
mvGraphics
setup_graphics(mvViewport& viewport)
{
    mvGraphics graphics{};
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
    return graphics;
}

void
resize_swapchain(mvGraphics& graphics, int width, int height)
{

}

void
cleanup_graphics(mvGraphics& graphics)
{

}

void
present(mvGraphics& graphics, mvColor& clearColor, bool vsync)
{
	MV_PROFILE_SCOPE("Presentation")

    mvViewport* viewport = GContext->viewport;
    auto viewportData = (mvViewportData*)viewport->platformSpecifics;

    glfwGetWindowPos(viewportData->handle, &viewport->xpos, &viewport->ypos);

    glfwSwapInterval(viewport->vsync ? 1 : 0); // Enable vsync

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(viewportData->handle, &display_w, &display_h);

    glViewport(0, 0, display_w, display_h);
    glClearColor(viewport->clearColor.r, viewport->clearColor.g, viewport->clearColor.b, viewport->clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);  // 
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(viewportData->handle);
}