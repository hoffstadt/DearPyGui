// Sample app built with Dear ImGui and ImPlot
// This app uses implot and imgui, but does not output to any backend! It only serves as a proof that an app can be built, linked, and run.

#include "imgui.h"
#include "implot.h"
#include "stdio.h"

int main(int, char**)
{    
    printf("sample_implot: start\n");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Additional imgui initialization needed when no backend is present
    ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
    ImGui::GetIO().Fonts->Build();

    // Render 500 frames
    for(int counter = 0; counter < 500; ++counter)
    {
        ImGui::NewFrame();

        if (ImGui::Begin("Hello, world!"))
        {
            ImGui::Text("Hello again");

            if (ImPlot::BeginPlot("My Plot"))
            {
                static double values[] = {1., 3., 5.};
                ImPlot::PlotLine("Values", values, 3);
                ImPlot::EndPlot();
            }

            #ifdef IMPLOT_INSTANTIATE_ALL_NUMERIC_TYPES
            if (ImPlot::BeginPlot("My Plot (long double)"))
            {
                static long double values[] = {1., 3., 5.};
                ImPlot::PlotLine("Values", values, 3);
                ImPlot::EndPlot();
            }
            #endif

            ImGui::End();
        }

        ImGui::Render();
    }

    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    printf("sample_implot: end\n");
    return 0;
}
