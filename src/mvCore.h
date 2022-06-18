#pragma once

//-----------------------------------------------------------------------------
// mvCore
//
//     - This file contains commonly used items throughout the core library
//     
//-----------------------------------------------------------------------------


#include <utility>
#include <memory>
#include <unordered_map>
#include "mvTypes.h"
#include "mvMath.h"

//-----------------------------------------------------------------------------
// mvFunctionWrapper
//     - Reguired because packaged_task reguires movable function objects
//-----------------------------------------------------------------------------
class mvFunctionWrapper
{
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() = default;
	};

	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		explicit impl_type(F&& f) : f(std::move(f)) {}
		void call() override { f(); }
	};

public:

	mvFunctionWrapper() = default;

	template<typename F>
	mvFunctionWrapper(F&& f) : m_impl(new impl_type<F>(std::move(f))) {}

	mvFunctionWrapper(mvFunctionWrapper&& other) noexcept
		: m_impl(std::move(other.m_impl))
	{

	}

	mvFunctionWrapper& operator=(mvFunctionWrapper&& other)
	{
		m_impl = std::move(other.m_impl);
		return *this;
	}

	// delete copy constructor and assignment operator
	mvFunctionWrapper(const mvFunctionWrapper&) = delete;
	mvFunctionWrapper(mvFunctionWrapper&) = delete;
	mvFunctionWrapper& operator=(const mvFunctionWrapper&) = delete;

	void operator()()
	{
		m_impl->call();
	}

private:

	std::unique_ptr<impl_base> m_impl;

};

//-----------------------------------------------------------------------------
// mvColor
//-----------------------------------------------------------------------------
struct mvColor
{
	static unsigned int ConvertToUnsignedInt(const mvColor& color)
	{
		return ImGui::ColorConvertFloat4ToU32(color.toVec4());
	}

	float r = -1.0f, g = -1.0f, b = -1.0f, a = -1.0f;

	mvColor() = default;

	mvColor(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{
	}

	mvColor(int r, int g, int b, int a)
		: r(r/255.0f), g(g/255.0f), b(b/255.0f), a(a/255.0f)
	{
	}

	explicit mvColor(ImVec4 color)
	{
		r = color.x;
		g = color.y;
		b = color.z;
		a = color.w;
	}

	operator ImU32() const
	{
		return ImGui::ColorConvertFloat4ToU32(toVec4());
	}

	operator float* ()
	{
		return &r;
	}

	operator ImVec4() const
	{
		return { r, g, b, a };
	}

	operator ImVec4*()
	{
		return (ImVec4*)&r;
	}

	const ImVec4 toVec4() const
	{
		return { r, g, b, a };
	}

};

#define MV_DEFAULT_COLOR mvColor(1.0f, 1.0f, 1.0f, 1.0f)

// UUIDs (+ reserved uuids)
#define MV_INVALID_UUID 0
#define MV_APP_UUID 1
#define MV_ATLAS_UUID 2
#define MV_TOOL_ABOUT_UUID 3
#define MV_TOOL_DEBUG_UUID 4
#define MV_TOOL_DOC_UUID 5
#define MV_TOOL_ITEM_REGISTRY_UUID 6
#define MV_TOOL_METRICS_UUID 7
#define MV_TOOL_STYLE_UUID 8
#define MV_TOOL_FONT_UUID 9
#define MV_RESERVED_UUIDs 10
#define MV_RESERVED_UUID_start MV_TOOL_FONT_UUID+1
#define MV_START_UUID MV_TOOL_FONT_UUID+MV_RESERVED_UUIDs+1
