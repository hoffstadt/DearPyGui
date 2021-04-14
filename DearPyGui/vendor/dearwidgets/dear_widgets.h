#include <imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <algorithm>
#include <cmath>

//////////////////////////////////////////////////////////////////////////
// Style TODO:
//	* Expose Style
//	* Line Thickness for Slider2D
//	* Line Color for Slider2D
//
// Known issue:
//	* Slider2DInt must take into account the half pixel
//	* Move 2D must store state "IsDrag"
//
// Optim TODO:
//	* ChromaticPlot draw only the internal MultiColorQuad where its needed "inside" or at least leave the option for style "transparency etc"
//	* ChromaticPlot: Bake some as much as possible values: provide different version: ChromaticPlotDynamic {From enum and compute info at each frame}, ChromaticPlotFromData {From Baked data}
//
// Write use case for:
//	* HueToHue:
//		- Color Remap
//	* LumToSat:
//		- Color Remap
//	* ColorRing:
//		- HDR Color Management {Shadow, MidTone, Highlight}
//	* Grid2D_AoS_Float:
//		- Color Remap:
//////////////////////////////////////////////////////////////////////////

namespace ImWidgets {

#define ImWidgets_Kibi (1024ull)
#define ImWidgets_Mibi (ImWidgets_Kibi*1024ull)
#define ImWidgets_Gibi (ImWidgets_Mibi*1024ull)
#define ImWidgets_Tebi (ImWidgets_Gibi*1024ull)
#define ImWidgets_Pebi (ImWidgets_Tebi*1024ull)

	typedef int ImWidgetsLengthUnit;
	typedef int ImWidgetsChromaticPlot;
	typedef int ImWidgetsObserver;
	typedef int ImWidgetsIlluminance;
	typedef int ImWidgetsColorSpace;
	typedef int ImWidgetsPointer;

	enum ImWidgetsLengthUnit_
	{
		ImWidgetsLengthUnit_Metric = 0,
		ImWidgetsLengthUnit_Imperial,
		ImWidgetsLengthUnit_COUNT
	};

	enum ImWidgetsObserver_
	{
		// Standard
		ImWidgetsObserverChromaticPlot_1931_2deg = 0,
		ImWidgetsObserverChromaticPlot_1964_10deg,
		ImWidgetsObserverChromaticPlot_COUNT
	};

	enum ImWidgetsIlluminance_
	{
		// White Points
		ImWidgetsWhitePointChromaticPlot_A = 0,
		ImWidgetsWhitePointChromaticPlot_B,
		ImWidgetsWhitePointChromaticPlot_C,
		ImWidgetsWhitePointChromaticPlot_D50,
		ImWidgetsWhitePointChromaticPlot_D55,
		ImWidgetsWhitePointChromaticPlot_D65,
		ImWidgetsWhitePointChromaticPlot_D75,
		ImWidgetsWhitePointChromaticPlot_D93,
		ImWidgetsWhitePointChromaticPlot_E,
		ImWidgetsWhitePointChromaticPlot_F1,
		ImWidgetsWhitePointChromaticPlot_F2,
		ImWidgetsWhitePointChromaticPlot_F3,
		ImWidgetsWhitePointChromaticPlot_F4,
		ImWidgetsWhitePointChromaticPlot_F5,
		ImWidgetsWhitePointChromaticPlot_F6,
		ImWidgetsWhitePointChromaticPlot_F7,
		ImWidgetsWhitePointChromaticPlot_F8,
		ImWidgetsWhitePointChromaticPlot_F9,
		ImWidgetsWhitePointChromaticPlot_F10,
		ImWidgetsWhitePointChromaticPlot_F11,
		ImWidgetsWhitePointChromaticPlot_F12,
		ImWidgetsWhitePointChromaticPlot_COUNT
	};

	enum ImWidgetsColorSpace_
	{
		// Color Spaces
		ImWidgetsColorSpace_AdobeRGB = 0,	// D65
		ImWidgetsColorSpace_AppleRGB,		// D65
		ImWidgetsColorSpace_Best,			// D50
		ImWidgetsColorSpace_Beta,			// D50
		ImWidgetsColorSpace_Bruce,			// D65
		ImWidgetsColorSpace_CIERGB,			// E
		ImWidgetsColorSpace_ColorMatch,		// D50
		ImWidgetsColorSpace_Don_RGB_4,		// D50
		ImWidgetsColorSpace_ECI,			// D50
		ImWidgetsColorSpace_Ekta_Space_PS5,	// D50
		ImWidgetsColorSpace_NTSC,			// C
		ImWidgetsColorSpace_PAL_SECAM,		// D65
		ImWidgetsColorSpace_ProPhoto,		// D50
		ImWidgetsColorSpace_SMPTE_C,		// D65
		ImWidgetsColorSpace_sRGB,			// D65
		ImWidgetsColorSpace_WideGamutRGB,	// D50
		ImWidgetsColorSpace_Rec2020,		// D65
		ImWidgetsColorSpace_COUNT
	};

	enum ImWidgetsChromaticPlot_
	{
		// Style
		ImWidgetsChromaticPlot_ShowWavelength,
		ImWidgetsChromaticPlot_ShowGrid,
		ImWidgetsChromaticPlot_ShowPrimaries,
		ImWidgetsChromaticPlot_ShowWhitePoint,

		ImWidgetsChromaticPlot_COUNT
	};

	enum ImWidgetsPointer_
	{
		// Style
		ImWidgetsPointer_Up,
		ImWidgetsPointer_Down,
		ImWidgetsPointer_Right,
		ImWidgetsPointer_Left,

		ImWidgetsPointer_COUNT
	};

	//static int triangleDrawCur = 0;
	extern int triangleDrawCur;

	// Layout
	IMGUI_API void CenterNextItem(ImVec2 nextItemSize);

	// Numbers, Scalar
	IMGUI_API bool DragFloatLog(const char* label, float* data, float v_speed, float log_basis = 10.0f, const void* p_min = NULL, const void* p_max = NULL, ImGuiSliderFlags flags = 0);

	IMGUI_API bool DragLengthScalar(const char* label, ImGuiDataType data_type, void* p_data, ImWidgetsLengthUnit* p_defaultUnit, float v_speed, const void* p_min = NULL, const void* p_max = NULL, ImGuiSliderFlags flags = 0);

	IMGUI_API bool Slider2DScalar(char const* pLabel, ImGuiDataType data_type, void* pValueX, void* pValueY, void* p_minX, void* p_maxX, void* p_minY, void* p_maxY, float const fScale = 1.0f);

	IMGUI_API bool Slider2DInt(char const* pLabel, int* pValueX, int* pValueY, int* p_minX, int* p_maxX, int* p_minY, int* p_maxY, float const fScale = 1.0f);
	IMGUI_API bool Slider2DFloat(char const* pLabel, float* pValueX, float* pValueY, float minX, float maxX, float minY, float maxY, float const fScale = 1.0f);
	IMGUI_API bool Slider2DDouble(char const* pLabel, double* pValueX, double* pValueY, double minX, double maxX, double minY, double maxY, float const fScale = 1.0f);

	IMGUI_API bool SliderScalar3D(char const* pLabel, float* pValueX, float* pValueY, float* pValueZ, float const fMinX, float const fMaxX, float const fMinY, float const fMaxY, float const fMinZ, float const fMaxZ, float const fScale = 1.0f);

	IMGUI_API bool InputVec2(const char* label, ImVec2* value, ImVec2 vMinValue, ImVec2 vMaxValue, float const fScale = 1.0f);
	IMGUI_API bool InputVec3(const char* label, ImVec4* value, ImVec4 const vMinValue, ImVec4 const vMaxValue, float const fScale = 1.0f);

	IMGUI_API bool RangeSelect2D(char const* pLabel, float* pCurMinX, float* pCurMinY, float* pCurMaxX, float* pCurMaxY, float const fBoundMinX, float const fBoundMinY, float const fBoundMaxX, float const fBoundMaxY, float const fScale /*= 1.0f*/);
	IMGUI_API bool RangeSelectVec2(const char* pLabel, ImVec2* pCurMin, ImVec2* pCurMax, ImVec2 const vBoundMin, ImVec2 const vBoundMax, float const fScale /*= 1.0f*/);

	IMGUI_API bool LineSlider(const char* label, ImVec2 start, ImVec2 end, ImU32 lineColor, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, ImWidgetsPointer pointer);

	// Grid
	// Default behavior: AoS & RowMajor
	IMGUI_API bool Grid2D_AoS_Float(const char* label, float* buffer, int rows, int columns, float minX, float maxX, float minY, float maxY);

	// Core
	IMGUI_API bool PlaneMovePoint2D(const char* label, float* buffer_aot, int float2_count, float minX, float maxX, float minY, float maxY);
	IMGUI_API bool MoveLine2D(const char* label, float* buffer_aot, int float2_count, float minX, float maxX, float minY, float maxY, bool closeLoop);

	// Widgets
	// Density Plot
	template < bool IsBilinear, typename FuncType >
	IMGUI_API bool DensityPlotEx(const char* label, FuncType func, int resX, int resY, float minX, float maxX, float minY, float maxY);
	IMGUI_API bool DensityPlotBilinear(const char* label, float(*sample)(float x, float y), int resX, int resY, float minX, float maxX, float minY, float maxY);
	IMGUI_API bool DensityPlotNearest(const char* label, float(*sample)(float x, float y), int resX, int resY, float minX, float maxX, float minY, float maxY);

	IMGUI_API bool DensityIsolinePlotBilinear(const char* label, float(*sample)(float x, float y), bool showSurface, float* isoValue, int isoLinesCount, ImU32* isoLinesColors, int isolinesColorsCount, int resX, int resY, float minX, float maxX, float minY, float maxY);

	// Plots
	template <typename FuncType>
	IMGUI_API void AnalyticalPlotEx(char const* label, FuncType func, float const minX, float const maxX, int const minSamples);
	IMGUI_API void AnalyticalPlot(char const* label, float(*func)(float const x), float const minX, float const maxX, int const minSamples = 64);

	// Draws
	IMGUI_API void DrawTrianglePointer(ImDrawList* pDrawList, ImVec2 targetPoint, float size, ImU32 col, ImWidgetsPointer pointDirection);
	IMGUI_API void DrawTrianglePointerFilled(ImDrawList* pDrawList, ImVec2 targetPoint, float size, ImU32 col, ImWidgetsPointer pointDirection);

	// Mask
	IMGUI_API void DrawConvexMaskMesh(ImDrawList* pDrawList, ImVec2 curPos, ImVec2 size, ImU32 maskColor, float* buffer_aot, int float2_count, float minX, float maxX, float minY, float maxY, bool flipY = false);

	// func: ImU32(*func)(float const x, float const y)
	template < bool IsBilinear, typename FuncType >
	IMGUI_API void DrawDensityPlotEx(ImDrawList* pDrawList, FuncType func, float minX, float maxX, float minY, float maxY, ImVec2 position, ImVec2 size, int resolutionX, int resolutionY);

	// xyzToRGB: a rowMajorMatrix
	template < bool IsBilinear >
	IMGUI_API void DrawChromaticPlotEx(ImDrawList* pDrawList,
		ImVec2 const vPos,
		float width, float heigth,
		int const chromeLineSamplesCount,
		ImWidgetsColorSpace const colorspace,
		ImWidgetsObserver const observer,
		ImWidgetsIlluminance const illum,
		int resX, int resY,
		ImU32 maskColor,
		float wavelengthMin = 400.0f, float wavelengthMax = 700.0f,
		float minX = 0.0f, float maxX = 0.8f,
		float minY = 0.0f, float maxY = 0.9f);
	IMGUI_API void DrawChromaticPlotNearest(ImDrawList* pDrawList,
		ImVec2 const vPos,
		float width, float heigth,
		int const chromeLineSamplesCount,
		ImWidgetsColorSpace const colorspace,
		ImWidgetsObserver const observer,
		ImWidgetsIlluminance const illum,
		int resX, int resY,
		ImU32 maskColor,
		float wavelengthMin = 400.0f, float wavelengthMax = 700.0f,
		float minX = 0.0f, float maxX = 0.8f,
		float minY = 0.0f, float maxY = 0.9f);
	IMGUI_API void DrawChromaticPlotBilinear(ImDrawList* pDrawList,
		ImVec2 const vPos,
		float width, float heigth,
		int const chromeLineSamplesCount,
		ImWidgetsColorSpace const colorspace,
		ImWidgetsObserver const observer,
		ImWidgetsIlluminance const illum,
		int resX, int resY,
		ImU32 maskColor,
		float wavelengthMin = 400.0f, float wavelengthMax = 700.0f,
		float minX = 0.0f, float maxX = 0.8f,
		float minY = 0.0f, float maxY = 0.9f);
	IMGUI_API void DrawChromaticPoint(ImDrawList* pDrawList, ImVec2 const vpos, ImU32 col);
	IMGUI_API void DrawChromaticLine(ImDrawList* pDrawList, ImVec2 const* vpos, int const pts_counts, ImU32 col, bool closed, float thickness);

	// func: ImU32(*func)(float const t): t in [0; 1]
	template <bool IsBilinear, typename FuncType>
	IMGUI_API void DrawColorBandEx(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, FuncType func, int division, float gamma);
	// func ImU32(*func)(float const t): t in [0; 1]
	template <bool IsBilinear, typename FuncType>
	IMGUI_API void DrawColorRingEx(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, float thickness, FuncType func, int division, float colorOffset);

	// Color Processing
	IMGUI_API void DrawHueBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, float alpha, float gamma, float offset = 0.0f);
	IMGUI_API void DrawHueBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, float colorStartRGB[3], float alpha, float gamma);
	IMGUI_API void DrawLumianceBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, ImVec4 const& color, float gamma);
	IMGUI_API void DrawSaturationBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, ImVec4 const& color, float gamma);
	IMGUI_API bool ColorRing(const char* label, float thickness, int split);

	// Color Selector
	IMGUI_API void HueSelector(char const* label, ImVec2 const size, float* hueCenter, float* hueWidth, float* featherLeft, float* featherRight, int division = 32, float alpha = 1.0f, float hideHueAlpha = 0.75f, float offset = 0.0f);

	//////////////////////////////////////////////////////////////////////////
	// External
	//////////////////////////////////////////////////////////////////////////
	// By @thedmd: https://github.com/ocornut/imgui/issues/1496#issuecomment-655048353
	void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(-1.0f, -1.0f));
	void EndGroupPanel();

#if 0
	// By @r-lyeh: https://github.com/ocornut/imgui/issues/786#issuecomment-479539045
	void ShowBezierDemo();

	// By @nem0: https://github.com/ocornut/imgui/issues/786#issuecomment-358106252
	enum class CurveEditorFlags
	{
		NO_TANGENTS = 1 << 0,
		SHOW_GRID = 1 << 1
	};
	int CurveEditor(const char* label, float* values, int points_count, const ImVec2& editor_size, ImU32 flags, int* new_count);
#endif

#include <dear_widgets.hpp>
}