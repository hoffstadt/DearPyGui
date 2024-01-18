#include "mvStackWindow.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include "imgui_internal.h"

static void
DebugItem(const char* label, const char* item)
{
ImGui::Text("%s", label);
ImGui::SameLine();
ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
}

static void
DebugItem(const char* label, float x)
{
ImGui::Text("%s", label);
ImGui::SameLine();
ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", std::to_string(x).c_str());
}

static void
DebugItem(const char* label, float x, float y)
{
ImGui::Text("%s", label);
ImGui::SameLine();
ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", (std::to_string(x) + ", " + std::to_string(y)).c_str());
}

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer() {
        MaxSize = 2000;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (!Data.empty())
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
};


mvStackWindow::mvStackWindow()
{
    m_windowflags = ImGuiWindowFlags_NoSavedSettings;
}

// TODO: We don't want to import imgui_internal.h then we should recreate the state by ourself

// [DEBUG] ID Stack Tool: update queries. Called by NewFrame()
void UpdateDebugToolStackQueries()
{
    ImGuiContext& g = *GImGui;
    ImGuiIDStackTool* tool = &g.DebugIDStackTool;

    // Clear hook when id stack tool is not visible
    g.DebugHookIdInfo = 0;
    if (g.FrameCount != tool->LastActiveFrame + 1)
        return;

    // Update queries. The steps are: -1: query Stack, >= 0: query each stack item
    // We can only perform 1 ID Info query every frame. This is designed so the GetID() tests are cheap and constant-time
    const ImGuiID query_id = g.HoveredIdPreviousFrame ? g.HoveredIdPreviousFrame : g.ActiveId;
    if (tool->QueryId != query_id)
    {
        tool->QueryId = query_id;
        tool->StackLevel = -1;
        tool->Results.resize(0);
    }
    if (query_id == 0)
        return;

    // Advance to next stack level when we got our result, or after 2 frames (in case we never get a result)
    int stack_level = tool->StackLevel;
    if (stack_level >= 0 && stack_level < tool->Results.Size)
        if (tool->Results[stack_level].QuerySuccess || tool->Results[stack_level].QueryFrameCount > 2)
            tool->StackLevel++;

    // Update hook
    stack_level = tool->StackLevel;
    if (stack_level == -1)
        g.DebugHookIdInfo = query_id;
    if (stack_level >= 0 && stack_level < tool->Results.Size)
    {
        g.DebugHookIdInfo = tool->Results[stack_level].ID;
        tool->Results[stack_level].QueryFrameCount++;
    }
}

// [DEBUG] ID Stack tool: hooks called by GetID() family functions
void ImGui::DebugHookIdInfo(ImGuiID id, ImGuiDataType data_type, const void* data_id, const void* data_id_end)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiIDStackTool* tool = &g.DebugIDStackTool;

    // Step 0: stack query
    // This assumes that the ID was computed with the current ID stack, which tends to be the case for our widget.
    if (tool->StackLevel == -1)
    {
        tool->StackLevel++;
        tool->Results.resize(window->IDStack.Size + 1, ImGuiStackLevelInfo());
        for (int n = 0; n < window->IDStack.Size + 1; n++)
            tool->Results[n].ID = (n < window->IDStack.Size) ? window->IDStack[n] : id;
        return;
    }

    // Step 1+: query for individual level
    IM_ASSERT(tool->StackLevel >= 0);
    if (tool->StackLevel != window->IDStack.Size)
        return;
    ImGuiStackLevelInfo* info = &tool->Results[tool->StackLevel];
    IM_ASSERT(info->ID == id && info->QueryFrameCount > 0);

    switch (data_type)
    {
    case ImGuiDataType_S32:
        ImFormatString(info->Desc, IM_ARRAYSIZE(info->Desc), "%d", (int)(intptr_t)data_id);
        break;
    case ImGuiDataType_String:
        ImFormatString(info->Desc, IM_ARRAYSIZE(info->Desc), "%.*s", data_id_end ? (int)((const char*)data_id_end - (const char*)data_id) : (int)strlen((const char*)data_id), (const char*)data_id);
        break;
    case ImGuiDataType_Pointer:
        ImFormatString(info->Desc, IM_ARRAYSIZE(info->Desc), "(void*)0x%p", data_id);
        break;
    case ImGuiDataType_ID:
        if (info->Desc[0] != 0) // PushOverrideID() is often used to avoid hashing twice, which would lead to 2 calls to DebugHookIdInfo(). We prioritize the first one.
            return;
        ImFormatString(info->Desc, IM_ARRAYSIZE(info->Desc), "0x%08X [override]", id);
        break;
    default:
        IM_ASSERT(0);
    }
    info->QuerySuccess = true;
    info->DataType = data_type;
}

static int StackToolFormatLevelInfo(ImGuiIDStackTool* tool, int n, bool format_for_ui, char* buf, size_t buf_size)
{
    ImGuiStackLevelInfo* info = &tool->Results[n];
    ImGuiWindow* window = (info->Desc[0] == 0 && n == 0) ? ImGui::FindWindowByID(info->ID) : NULL;
    if (window)                                                                 // Source: window name (because the root ID don't call GetID() and so doesn't get hooked)
        return ImFormatString(buf, buf_size, format_for_ui ? "\"%s\" [window]" : "%s", window->Name);
    if (info->QuerySuccess)                                                     // Source: GetID() hooks (prioritize over ItemInfo() because we frequently use patterns like: PushID(str), Button("") where they both have same id)
        return ImFormatString(buf, buf_size, (format_for_ui && info->DataType == ImGuiDataType_String) ? "\"%s\"" : "%s", info->Desc);
    if (tool->StackLevel < tool->Results.Size)                                  // Only start using fallback below when all queries are done, so during queries we don't flickering ??? markers.
        return (*buf = 0);
#ifdef IMGUI_ENABLE_TEST_ENGINE
    if (const char* label = ImGuiTestEngine_FindItemDebugLabel(GImGui, info->ID))   // Source: ImGuiTestEngine's ItemInfo()
        return ImFormatString(buf, buf_size, format_for_ui ? "??? \"%s\"" : "%s", label);
#endif
    return ImFormatString(buf, buf_size, "???");
}

// ID Stack Tool: Display UI
void ImGui::ShowIDStackToolWindow(bool* p_open)
{
    ImGuiContext* g = ImGui::GetCurrentContext();
    if (!(g->NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize))
        SetNextWindowSize(ImVec2(0.0f, GetFontSize() * 8.0f), ImGuiCond_FirstUseEver);
    if (!Begin("Dear ImGui ID Stack Tool", p_open) || GetCurrentWindow()->BeginCount > 1)
    {
        End();
        return;
    }

    // Display hovered/active status
    ImGuiIDStackTool* tool = &g->DebugIDStackTool;
    const ImGuiID hovered_id = g->HoveredIdPreviousFrame;
    const ImGuiID active_id = g->ActiveId;
#ifdef IMGUI_ENABLE_TEST_ENGINE
    Text("HoveredId: 0x%08X (\"%s\"), ActiveId:  0x%08X (\"%s\")", hovered_id, hovered_id ? ImGuiTestEngine_FindItemDebugLabel(&g, hovered_id) : "", active_id, active_id ? ImGuiTestEngine_FindItemDebugLabel(&g, active_id) : "");
#else
    Text("HoveredId: 0x%08X, ActiveId:  0x%08X", hovered_id, active_id);
#endif
    // SameLine();
    // MetricsHelpMarker("Hover an item with the mouse to display elements of the ID Stack leading to the item's final ID.\nEach level of the stack correspond to a PushID() call.\nAll levels of the stack are hashed together to make the final ID of a widget (ID displayed at the bottom level of the stack).\nRead FAQ entry about the ID stack for details.");

    // CTRL+C to copy path
    const float time_since_copy = (float)g->Time - tool->CopyToClipboardLastTime;
    ImGui::Checkbox("Ctrl+C: copy path to clipboard", &tool->CopyToClipboardOnCtrlC);
    SameLine();
    TextColored((time_since_copy >= 0.0f && time_since_copy < 0.75f && ImFmod(time_since_copy, 0.25f) < 0.25f * 0.5f) ? ImVec4(1.f, 1.f, 0.3f, 1.f) : ImVec4(), "*COPIED*");
    if (tool->CopyToClipboardOnCtrlC && IsKeyDown(ImGuiMod_Ctrl) && IsKeyPressed(ImGuiKey_C))
    {
        tool->CopyToClipboardLastTime = (float)g->Time;
        char* p = g->TempBuffer.Data;
        char* p_end = p + g->TempBuffer.Size;
        for (int stack_n = 0; stack_n < tool->Results.Size && p + 3 < p_end; stack_n++)
        {
            *p++ = '/';
            char level_desc[256];
            StackToolFormatLevelInfo(tool, stack_n, false, level_desc, IM_ARRAYSIZE(level_desc));
            for (int n = 0; level_desc[n] && p + 2 < p_end; n++)
            {
                if (level_desc[n] == '/')
                    *p++ = '\\';
                *p++ = level_desc[n];
            }
        }
        *p = '\0';
        SetClipboardText(g->TempBuffer.Data);
    }

    // Display decorated stack
    tool->LastActiveFrame = g->FrameCount;
    if (tool->Results.Size > 0 && BeginTable("##table", 3, ImGuiTableFlags_Borders))
    {
        const float id_width = CalcTextSize("0xDDDDDDDD").x;
        TableSetupColumn("Seed", ImGuiTableColumnFlags_WidthFixed, id_width);
        TableSetupColumn("PushID", ImGuiTableColumnFlags_WidthStretch);
        TableSetupColumn("Result", ImGuiTableColumnFlags_WidthFixed, id_width);
        TableHeadersRow();
        for (int n = 0; n < tool->Results.Size; n++)
        {
            ImGuiStackLevelInfo* info = &tool->Results[n];
            TableNextColumn();
            Text("0x%08X", (n > 0) ? tool->Results[n - 1].ID : 0);
            TableNextColumn();
            StackToolFormatLevelInfo(tool, n, true, g->TempBuffer.Data, g->TempBuffer.Size);
            TextUnformatted(g->TempBuffer.Data);
            TableNextColumn();
            Text("0x%08X", info->ID);
            if (n == tool->Results.Size - 1)
                TableSetBgColor(ImGuiTableBgTarget_CellBg, GetColorU32(ImGuiCol_Header));
        }
        EndTable();
    }
    End();
}
void mvStackWindow::drawWidgets()
{

    if (ImGui::BeginTabBar("Main Tabbar"))
    {
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::BeginTabItem("General##metricswindow"))
        {
                
            DebugItem("ImGui Version: ", IMGUI_VERSION);
            ImGui::Text("STILL TO BE IMPLEMENTED");

            static std::map<std::string, ScrollingBuffer> buffers;
            static float t = 0;
            t += ImGui::GetIO().DeltaTime;

            const auto& results = mvInstrumentor::Get().getResults();

            for (const auto& item : results) {
                buffers[item.first].AddPoint(t, (float)item.second.count());
            }

            static float history = 10.0f;
            ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

            float max_value = 0.0f;

            for (int i = 0; i < buffers["Frame"].Data.Size; i++)
            {
                if (buffers["Frame"].Data[i].y > max_value)
                    max_value = buffers["Frame"].Data[i].y;
            }

            if (ImGui::GetIO().Framerate < 29)
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
            else if (ImGui::GetIO().Framerate < 59)
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(1.0f, 1.0f, 0.0f, 0.3f));
            else
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            ImPlot::PushStyleColor(ImPlotCol_PlotBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            ImPlot::SetNextAxisToFit(ImAxis_X1);
            static ImPlotAxisFlags rt_axis = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_LockMin;
            if (ImPlot::BeginPlot("##Scrolling1", ImVec2(-1, 200), ImPlotFlags_None))
            {
                ImPlot::SetupAxis(ImAxis_X1, (const char *)nullptr, rt_axis);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                static float fps_h[2] = { 0.0f, 0.0f };
                static float fps_x[2] = { 0.0f, 10.0f };
                fps_x[0] = t - history;
                fps_x[1] = t;
                static float fps_60[2] = { 16000.0f, 16000.0f };
                static float fps_30[2] = { 32000.0f, 32000.0f };

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(0.0f, 1.0f, 0.0f, 0.1f));
                ImPlot::PlotShaded("60+ FPS", fps_x, fps_h, fps_60, 2);
                ImPlot::PopStyleColor();

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1.0f, 1.0f, 0.0f, 0.1f));
                ImPlot::PlotShaded("30+ FPS", fps_x, fps_60, fps_30, 2);
                ImPlot::PopStyleColor();

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1.0f, 0.0f, 0.0f, 0.1f));
                ImPlot::PlotShaded("Low FPS", fps_x, fps_30, 2, INFINITY);
                ImPlot::PopStyleColor();

                if (!buffers["Frame"].Data.empty())
                    ImPlot::PlotLine("Frame", &buffers["Frame"].Data[0].x, &buffers["Frame"].Data[0].y, buffers["Frame"].Data.size(), ImPlotLineFlags_None, buffers["Frame"].Offset, 2 * sizeof(float));
                if (!buffers["Presentation"].Data.empty())
                    ImPlot::PlotLine("Presentation", &buffers["Presentation"].Data[0].x, &buffers["Presentation"].Data[0].y, buffers["Presentation"].Data.size(), ImPlotLineFlags_None, buffers["Presentation"].Offset, 2 * sizeof(float));
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor(3);

            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            
            ImPlot::SetNextAxisToFit(ImAxis_X1);
            ImPlot::SetNextAxisToFit(ImAxis_Y1); // Is it okay?
            if (ImPlot::BeginPlot("##Scrolling2", ImVec2(-1, -1), ImPlotFlags_None))
            {
                ImPlot::SetupAxis(ImAxis_X1, (const char *)nullptr, rt_axis);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                for (const auto& item : results)
                {
                    if (item.first == "Frame" || item.first == "Presentation")
                        continue;
                    ImPlot::PlotLine(item.first.c_str(), &buffers[item.first].Data[0].x, &buffers[item.first].Data[0].y, buffers[item.first].Data.size(), ImPlotLineFlags_None, buffers[item.first].Offset, 2 * sizeof(float));
                }
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Input##metricswindow"))
        {

            ImGui::PushItemWidth(200);
            ImGui::BeginGroup();

            //DebugItem("Active Window: ", GContext->itemRegistry->getActiveWindow().c_str());
            DebugItem("Local Mouse Position:", (float)GContext->input.mousePos.x, (float)GContext->input.mousePos.y);
            DebugItem("Global Mouse Position:", (float)io.MousePos.x, (float)io.MousePos.y);
            DebugItem("Plot Mouse Position:", (float)GContext->input.mousePlotPos.x, (float)GContext->input.mousePlotPos.y);
            DebugItem("Mouse Drag Delta:", (float)GContext->input.mouseDragDelta.x, (float)GContext->input.mouseDragDelta.y);
            DebugItem("Mouse Drag Threshold:", (float)GContext->input.mouseDragThreshold);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("ImGui State Inputs");

            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

            ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysData); i++) if (io.KeysData[i].DownDuration >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysData[i].DownDuration); }
            ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysData); i++) if (ImGui::IsKeyPressed((ImGuiKey)i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysData); i++) if (ImGui::IsKeyReleased((ImGuiKey)i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:");    for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); }

            ImGui::EndGroup();
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}