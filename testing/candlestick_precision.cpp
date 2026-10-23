// Headless regression for the actual private renderer. Including the translation
// unit avoids exporting a test-only API; section GC discards unrelated DPG code.
#include "../src/mvPlotting.cpp"
#include <cmath>
#include <cstdio>

static int failures = 0;

static void check_candles(const char* name, const double* dates, int count,
                          float weight, double expected_half_width, bool time_axis)
{
    const double opens[] = {22000, 22020, 22010, 22030};
    const double closes[] = {22020, 22010, 22030, 22015};
    const double lows[] = {21990, 22000, 22000, 22005};
    const double highs[] = {22030, 22030, 22040, 22040};
    const double origin = count > 0 ? dates[0] : 0.0;
    const double span = count > 1 ? dates[count - 1] - origin : 1.0;
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(1000, 600));
    ImGui::Begin("Candlestick regression", nullptr, ImGuiWindowFlags_NoSavedSettings);
    if (ImPlot::BeginPlot("Candles", ImVec2(950, 500), ImPlotFlags_NoLegend)) {
        ImPlot::SetupAxes(nullptr, nullptr);
        if (time_axis)
            ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
        ImPlot::SetupAxesLimits(origin - span, origin + 2 * span,
                                21980, 22050, ImGuiCond_Always);
        ImPlot::SetupFinish();
        ImDrawList* draw_list = ImPlot::GetPlotDrawList();
        draw_list->Flags &= ~(ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill);
        const int start = draw_list->VtxBuffer.Size;
        PlotCandlestick(name, dates, opens, closes, lows, highs, count, false,
                        weight, ImVec4(0, 1, 0, 1), ImVec4(1, 0, 0, 1), ImPlotTimeUnit_Min);
        if (count == 0 && draw_list->VtxBuffer.Size == start)
            std::puts("PASS empty: no candle vertices");
        // Non-AA wick and body each contribute four vertices. Check every body,
        // including both bullish and bearish candles, against fixed expected widths.
        if (draw_list->VtxBuffer.Size - start != count * 8) {
            std::fprintf(stderr, "FAIL %s: unexpected geometry size\n", name);
            ++failures;
        }
        else for (int i = 0; i < count; ++i) {
            const ImVec2 left = draw_list->VtxBuffer[start + i * 8 + 4].pos;
            const ImVec2 right = draw_list->VtxBuffer[start + i * 8 + 6].pos;
            const double actual_width = right.x - left.x;
            const double expected_width = 2 * expected_half_width / (3 * span) * ImPlot::GetPlotSize().x;
            const double expected_center = ImPlot::PlotToPixels(dates[i], opens[i]).x;
            const bool ok = std::abs(actual_width - expected_width) < 0.002
                && std::abs((left.x + right.x) * 0.5 - expected_center) < 0.002;
            std::printf("%s %s candle=%d width_px=%.6f expected_px=%.6f\n",
                        ok ? "PASS" : "FAIL", name, i, actual_width, expected_width);
            if (!ok) ++failures;
        }
        ImPlot::EndPlot();
    }
    else {
        std::fprintf(stderr, "FAIL %s: plot did not open\n", name);
        ++failures;
    }
    ImGui::End();
    ImGui::Render();
}

int main()
{
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.DisplaySize = ImVec2(1200, 800);
    io.DeltaTime = 1.0f / 60.0f;
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    const double minute[] = {1678089480, 1678089540, 1678089600, 1678089660};
    const double collapsed[] = {1678089540, 1678089600, 1678089660, 1678089720};
    const double five_minute[] = {1678089480, 1678089780, 1678090080, 1678090380};
    const double hourly[] = {1678089480, 1678093080, 1678096680, 1678100280};
    const double daily[] = {1678089480, 1678175880, 1678262280, 1678348680};
    const double small[] = {0, 60, 120, 180};
    const double fractional[] = {0, 0.125, 0.25, 0.375};
    const double irregular[] = {1678089480, 1678089540, 1678089720, 1678089780};
    const double single[] = {1678089480};
    const double negative[] = {-180, -120, -60, 0};
    const double duplicate[] = {0, 0, 60, 120};
    check_candles("minute", minute, 4, 0.25f, 15, true);
    check_candles("collapsed-minute", collapsed, 4, 0.25f, 15, true);
    check_candles("five-minute", five_minute, 4, 0.25f, 75, true);
    check_candles("hourly", hourly, 4, 0.25f, 900, true);
    check_candles("daily", daily, 4, 0.25f, 21600, true);
    check_candles("small", small, 4, 0.25f, 15, false);
    check_candles("fractional", fractional, 4, 0.25f, 0.03125, false);
    check_candles("irregular-first-gap", irregular, 4, 0.25f, 15, true);
    check_candles("single", single, 1, 0.25f, 0.25, false);
    check_candles("empty", nullptr, 0, 0.25f, 0.25, false);
    check_candles("custom-weight", minute, 4, 0.125f, 7.5, true);
    check_candles("negative", negative, 4, 0.25f, 15, false);
    check_candles("duplicate-first-gap", duplicate, 4, 0.25f, 0, false);
    check_candles("zero-weight", minute, 4, 0, 0, true);
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    std::printf("%d failures\n", failures);
    return failures ? 1 : 0;
}
