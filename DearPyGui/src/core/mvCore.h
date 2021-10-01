#pragma once

//-----------------------------------------------------------------------------
// mvCore
//
//     - This file contains commonly used items throughout the core library
//     
//-----------------------------------------------------------------------------

#include <imgui.h>
#include <implot.h>
#include <utility>
#include <memory>
#include <unordered_map>

namespace Marvel {

	template<typename T>
	using mvOwnedPtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr mvOwnedPtr<T> CreateOwnedPtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using mvRef = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr mvRef<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

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
	// mvPlotPoint
	//-----------------------------------------------------------------------------
	struct mvPlotPoint
	{
		double x, y;

		operator ImPlotPoint()
		{
			return ImPlotPoint{ x, y };
		}

		mvPlotPoint operator+(const ImPlotPoint& other)
		{
			return mvPlotPoint{ x + other.x, y + other.y };
		}
	};

	//-----------------------------------------------------------------------------
	// mvVec2
	//-----------------------------------------------------------------------------
	struct mvVec2
	{
		float x, y;

		operator ImVec2()
		{
			return ImVec2{ x, y };
		}

		operator ImPlotPoint()
		{
			return ImPlotPoint{ (double)x, (double)y };
		}

		mvVec2 operator+(const ImVec2& other)
		{
			return mvVec2{ x + other.x, y + other.y };
		}
	};

	//-----------------------------------------------------------------------------
	// mvVec4
	//-----------------------------------------------------------------------------
	struct mvVec4
	{
		float x, y, z, w;

		operator ImVec4()
		{
			if (x < 0 || y < 0 || z < 0 || w < 0)
				return ImVec4(0, 0, 0, -1);
			return ImVec4{ x, y , z, w};
		}

		mvVec4 operator+(const ImVec4& other)
		{
			return mvVec4{ x + other.x, y + other.y, z + other.z, w + other.w };
		}
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

		operator ImU32()
		{
			return ImGui::ColorConvertFloat4ToU32(toVec4());
		}

		operator float* ()
		{
			return &r;
		}

		operator ImVec4()
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

	//-----------------------------------------------------------------------------
	// Constant Expression for loop
	//-----------------------------------------------------------------------------
	template <auto Start, auto End, auto Inc, class F>
	constexpr void constexpr_for(F&& f)
	{
		if constexpr (Start < End)
		{
			f(std::integral_constant<decltype(Start), Start>());
			constexpr_for<Start + Inc, End, Inc>(f);
		}
	}

	typedef std::unordered_map<long, float> mvThemeStyles;
	typedef unsigned long long mvUUID;
}

#if !defined(mv_internal)
#define mv_internal static
#endif
#define mv_local_persist static
#define mv_global static
#define mv_python_function PyObject*
#ifndef mv_impl
#define mv_impl
#endif

#define MV_DEFAULT_COLOR Marvel::mvColor(1.0f, 1.0f, 1.0f, 1.0f)

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