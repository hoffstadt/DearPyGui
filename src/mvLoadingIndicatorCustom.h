#pragma once

#include <imgui.h>

// Posted by @alexsr here: https://github.com/ocornut/imgui/issues/1901
// Sligthly modified to provide default behaviour with default args
void LoadingIndicatorCircle(const char* label, float indicatorRadiusFactor = 1.f,
    const ImVec4* pOptionalMainColor = NULL, const ImVec4* pOptionalBackdropColor = NULL,
    int circle_count = 8, const float speed = 1.f);

// Posted by @zfedoran here: https://github.com/ocornut/imgui/issues/1901
// Sligthly modified to provide default behaviour with default args
void LoadingIndicatorCircle2(const char* label, float indicatorRadiusFactor = 1.f, float indicatorRadiusThicknessFactor = 1.f, const ImVec4* pOptionalColor = NULL);

void LoadingIndicatorRing(const char* label, float size, float thickness, float speed = 1.f, const ImVec4* pOptionalColor = nullptr);
