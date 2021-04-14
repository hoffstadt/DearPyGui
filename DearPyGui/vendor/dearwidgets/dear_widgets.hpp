
//////////////////////////////////////////////////////////////////////////
// Helpers
template < typename Type >
inline
Type	Normalize01(Type const x, Type const _min, Type const _max)
{
	return (x - _min) / (_max - _min);
}
template < typename Type >
inline
Type	ScaleFromNormalized(Type const x, Type const newMin, Type const newMax)
{
	return x * (newMax - newMin) + newMin;
}
template < typename Type >
inline
Type	Rescale(Type const x, Type const _min, Type const _max, Type const newMin, Type const newMax)
{
	return ScaleFromNormalized(Normalize01(x, _min, _max), newMin, newMax);
}

inline
float ImDot(ImVec2 const a, ImVec2 const b)
{
	return a.x * b.x + a.y * b.y;
}

inline
float ImDot(ImVec4 const a, ImVec4 const b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline
float ImFract(float x)
{
	float iPart;
	return std::modf(x, &iPart);
}

inline
float ImSmoothStep(float edge0, float edge1, float x)
{
	// Scale, bias and saturate x to 0..1 range
	x = ImClamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial
	return x * x * (3.0f - 2.0f * x);
}

inline
float ImLength(ImVec2 v)
{
	return ImSqrt(ImLengthSqr(v));
}

inline
float ImLength(ImVec4 v)
{
	return ImSqrt(ImLengthSqr(v));
}

inline
float	LinearSample(float t, float* buffer, int count)
{
#if 0
	float const width = (float)(count - 1);

	float tx = ScaleFromNormalized(t, 0.0f, width);

	float i0 = ImFloor(tx);
	float i1 = ImCeil(tx);

	float t0 = i0 / width;
	float t1 = i1 / width;

	float ti;
	if (t0 != t1)
		ti = Normalize01(t, t0, t1);
	else
		ti = 1.0f;

	return ImLerp(buffer[(int)i0], buffer[(int)i1], ti);
#else
	float const width = (float)(count - 1);

	float const i0 = ImFloor(t * width);
	float const i1 = ImCeil(t * width);

	float ti;
	if (i0 != i1)
		ti = (t * width - i0) / (i1 - i0);
	else
		ti = 1.0f;

	return ImLerp(buffer[(int)i0], buffer[(int)i1], ti);
#endif
}

// Arbitrary way to measure roughness, to resample function for AnalyticalPlot
inline
float Roughness(float x0, float x1, float x2, float x3, float x4)
{
	float const m = 0.2f * x0 + 0.2f * x1 + 0.2f * x2 + 0.2f * x3 + 0.2f * x4;

	float const dx0 = m - x0;
	float const dx1 = m - x1;
	float const dx2 = m - x2;
	float const dx3 = m - x3;
	float const dx4 = m - x4;

	float const rms = (dx0 * dx0 + dx1 * dx1 + dx2 * dx2 + dx3 * dx3 + dx4 * dx4);

	float const ddx0 = ImAbs(x0 - x1);
	float const ddx1 = ImAbs(x1 - x2);
	float const ddx2 = ImAbs(x2 - x3);
	float const ddx3 = ImAbs(x3 - x4);

	//return 0*rms + ddx0 * ddx0 + ddx1 * ddx1 + ddx2 * ddx2 + ddx3 * ddx3;
	return ddx1 * ddx1 + ddx2 * ddx2;
}

// func: ImU32(*func)(float const x, float const y)
template < bool IsBilinear, typename FuncType >
inline
void DrawColorDensityPlotEx(ImDrawList* pDrawList, FuncType func, float minX, float maxX, float minY, float maxY, ImVec2 position, ImVec2 size, int resolutionX, int resolutionY)
{
	ImVec2 const uv = ImGui::GetFontTexUvWhitePixel();

	float const sx = size.x / ((float)resolutionX);
	float const sy = size.y / ((float)resolutionY);

	float const dy = 1.0f / ((float)resolutionY);
	float const dx = 1.0f / ((float)resolutionX);
	float const hdx = 0.5f / ((float)resolutionX);
	float const hdy = 0.5f / ((float)resolutionY);

	for (int i = 0; i < resolutionX; ++i)
	{
		float x0;
		float x1;
		if constexpr (IsBilinear)
		{
			x0 = ScaleFromNormalized(((float)i + 0) * dx, minX, maxX);
			x1 = ScaleFromNormalized(((float)i + 1) * dx, minX, maxX);
		}
		else
		{
			x0 = ScaleFromNormalized(((float)i + 0) * dx + hdx, minX, maxX);
		}

		for (int j = 0; j < resolutionY; ++j)
		{
			float y0;
			float y1;
			if constexpr (IsBilinear)
			{
				y0 = ScaleFromNormalized(((float)(j + 0) * dy), maxY, minY);
				y1 = ScaleFromNormalized(((float)(j + 1) * dy), maxY, minY);
			}
			else
			{
				y0 = ScaleFromNormalized(((float)(j + 0) * dy + hdy), maxY, minY);
			}

			ImU32 const col00 = func(x0, y0);
			if constexpr (IsBilinear)
			{
				ImU32 const col01 = func(x0, y1);
				ImU32 const col10 = func(x1, y0);
				ImU32 const col11 = func(x1, y1);
				pDrawList->AddRectFilledMultiColor(position + ImVec2(sx * (i + 0), sy * (j + 0)),
					position + ImVec2(sx * (i + 1), sy * (j + 1)),
					col00, col10, col11, col01);
			}
			else
			{
				pDrawList->AddRectFilledMultiColor(position + ImVec2(sx * (i + 0), sy * (j + 0)),
					position + ImVec2(sx * (i + 1), sy * (j + 1)),
					col00, col00, col00, col00);
			}
		}
	}
}

// func: ImU32(*func)(float const t)
template <bool IsBilinear, typename FuncType>
inline
void	DrawColorBandEx(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, FuncType func, int division, float gamma)
{
	float const width = size.x;
	float const height = size.y;

	float const fSlice = static_cast<float>(division);

	ImVec2 dA(vpos);
	ImVec2 dB(vpos.x + width / fSlice, vpos.y + height);

	ImVec2 const dD(ImVec2(width / fSlice, 0));

	auto curColor = [gamma, &func](float const x, float const)
	{
		return func(ImPow(x, gamma));
	};

	DrawColorDensityPlotEx< IsBilinear >(pDrawList, curColor, 0.0f, 1.0f, 0.0f, 0.0f, vpos, size, division, 1);
}

template <bool IsBilinear, typename FuncType>
inline
void	DrawColorRingEx(ImDrawList* pDrawList, ImVec2 const curPos, ImVec2 const size, float thickness_, FuncType func, int division, float colorOffset)
{
	float const radius = ImMin(size.x, size.y) * 0.5f;

	float const dAngle = 2.0f * IM_PI / ((float)division);
	float angle = 2.0f * IM_PI / 3.0f;

	ImVec2 offset = curPos + ImVec2(radius, radius);
	if (size.x < size.y)
	{
		offset.y += 0.5f * (size.y - size.x);
	}
	else if (size.x > size.y)
	{
		offset.x += 0.5f * (size.x - size.y);
	}

	float const thickness = ImSaturate(thickness_) * radius;

	ImVec2 const uv = ImGui::GetFontTexUvWhitePixel();
	pDrawList->PrimReserve(division * 6, division * 4);
	for (int i = 0; i < division; ++i)
	{
		float x0 = radius * ImCos(angle);
		float y0 = radius * ImSin(angle);

		float x1 = radius * ImCos(angle + dAngle);
		float y1 = radius * ImSin(angle + dAngle);

		float x2 = (radius - thickness) * ImCos(angle + dAngle);
		float y2 = (radius - thickness) * ImSin(angle + dAngle);

		float x3 = (radius - thickness) * ImCos(angle);
		float y3 = (radius - thickness) * ImSin(angle);

		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 1));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 2));

		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 2));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 3));

		float const t0 = std::fmodf(colorOffset + ((float)i) / ((float)division), 1.0f);
		ImU32 const uCol0 = func(t0);

		if constexpr (IsBilinear)
		{
			float const t1 = std::fmodf(colorOffset + ((float)(i + 1)) / ((float)division), 1.0f);
			ImU32 const uCol1 = func(t1);
			pDrawList->PrimWriteVtx(offset + ImVec2(x0, y0), uv, uCol0);
			pDrawList->PrimWriteVtx(offset + ImVec2(x1, y1), uv, uCol1);
			pDrawList->PrimWriteVtx(offset + ImVec2(x2, y2), uv, uCol1);
			pDrawList->PrimWriteVtx(offset + ImVec2(x3, y3), uv, uCol0);
		}
		else
		{
			pDrawList->PrimWriteVtx(offset + ImVec2(x0, y0), uv, uCol0);
			pDrawList->PrimWriteVtx(offset + ImVec2(x1, y1), uv, uCol0);
			pDrawList->PrimWriteVtx(offset + ImVec2(x2, y2), uv, uCol0);
			pDrawList->PrimWriteVtx(offset + ImVec2(x3, y3), uv, uCol0);
		}
		angle += dAngle;
	}
}

template < bool IsBilinear >
inline
void DrawChromaticPlotEx(ImDrawList* pDrawList,
	ImVec2 const vPos,
	float width, float heigth,
	int const chromeLineSamplesCount,
	ImWidgetsColorSpace const colorspace,
	ImWidgetsObserver const observer,
	ImWidgetsIlluminance const illum,
	int resX, int resY,
	ImU32 maskColor,
	float wavelengthMin, float wavelengthMax,
	float minX, float maxX,
	float minY, float maxY)
{
	if constexpr (IsBilinear)
		DrawChromaticPlotBilinear(
			pDrawList,
			vPos, width, heigth,
			chromeLineSamplesCount,
			colorspace,
			observer,
			illum,
			resX, resY,
			maskColor,
			wavelengthMin, wavelengthMax,
			minX, maxX,
			minY, maxY);
	else
		DrawChromaticPlotNearest(
			pDrawList,
			vPos, width, heigth,
			chromeLineSamplesCount,
			colorspace,
			observer,
			illum,
			resX, resY,
			maskColor,
			wavelengthMin, wavelengthMax,
			minX, maxX,
			minY, maxY);
}

// Plots
// func: float(*func)(float const x)
template <typename FuncType>
inline
void	AnalyticalPlotEx(char const* label, FuncType func, float const minX, float const maxX, int const minSamples)
{
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);

	ImVec2 curPos = ImGui::GetCursorScreenPos();
	float const width = ImGui::GetContentRegionAvail().x;
	float const height = width;

	ImGui::InvisibleButton("##Zone", ImVec2(width, height), 0);
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();

	float const dx = (maxX - minX) / ((float)minSamples);
	//float const ddx = dx * 0.02f;
	float const ddx = dx / 50.0f;

	ImVector<float> roughness;
	roughness.reserve(minSamples);

	float y;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	for (int i = 0; i < minSamples; ++i)
	{
		float const x = ScaleFromNormalized((float)i / ((float)(minSamples - 1)), minX, maxX);

		float y1 = func(x - 2.0f * ddx);
		float y2 = func(x - 1.0f * ddx);
		float y = func(x);
		float y4 = func(x + 1.0f * ddx);
		float y5 = func(x + 2.0f * ddx);

		float const rough = Roughness(y1, y2, y, y4, y5);

		roughness.push_back(rough);

		if (y < minY)
			minY = y;
		if (y > maxY)
			maxY = y;
	}
	float const minRough = *std::min_element(roughness.begin(), roughness.end());
	float const maxRough = *std::max_element(roughness.begin(), roughness.end());
	std::transform(roughness.begin(), roughness.end(), roughness.begin(), [minRough, maxRough](float const xx) { return ImPow(ImSaturate(1.0f - (xx - minRough) / (maxRough - minRough)), 1.0f); });

	ImVector<ImVec2> pts;
	float x = minX;
	while (x <= maxX)
	{
		float const coef = Normalize01(x, minX, maxX);
		float const curRoughInv = LinearSample(coef, &roughness[0], roughness.size());
		x += ddx * ImMax(curRoughInv, 1e-4f);

		y = func(x);

		float const winX = Rescale(x, minX, maxX, 0.0f, width);
		float const winY = Rescale(y, minY, maxY, 0.0f, height);

		pts.push_back(curPos + ImVec2(winX, winY));
	};

	if (pts.size() > ((1 << (sizeof(ImDrawIdx) * 4)) - 1))
		pts.resize(((1 << (sizeof(ImDrawIdx) * 4)) - 1));

	pDrawList->AddPolyline(&pts[0], pts.size(), IM_COL32(255, 255, 255, 255), false, 1.0f);

	ImGui::PopID();
}

// Density Plots
// func: float(*sample)(float x, float y)
template < bool IsBilinear, typename FuncType >
inline
bool DensityPlotEx(const char* label, FuncType func, int resX, int resY, float minX, float maxX, float minY, float maxY)
{
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);

	float* pMin = ImGui::GetStateStorage()->GetFloatRef(iID + 0, FLT_MAX);
	float* pMax = ImGui::GetStateStorage()->GetFloatRef(iID + 1, -FLT_MAX);

	ImVec2 curPos = ImGui::GetCursorScreenPos();
	float const width = ImGui::GetContentRegionAvail().x;
	float const height = width;

	ImGui::InvisibleButton("##Zone", ImVec2(width, height), 0);

	ImVec2 const uv = ImGui::GetFontTexUvWhitePixel();
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();

	auto GetColor = [pMin, pMax, &func](float x, float y) {
		float value = func(x, y);
		if (value < *pMin)
			*pMin = value;
		if (value > *pMax)
			*pMax = value;

		float showValue = Normalize01(value, *pMin, *pMax);
		ImU32 const uVal = static_cast<ImU32>(showValue * 255.0f);

		return IM_COL32(uVal, uVal, uVal, 255);
	};

	DrawColorDensityPlotEx< IsBilinear >(pDrawList, GetColor, minX, maxX, minX, maxY, curPos, ImVec2(width, height), resX, resY);

	ImGui::PopID();

	return false;
}