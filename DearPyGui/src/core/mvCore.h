#pragma once

//-----------------------------------------------------------------------------
// mvCore
//
//     - This file contains commonly used items throughout the core library
//     
//-----------------------------------------------------------------------------

#include <imgui.h>
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
	// mvVec2
	//-----------------------------------------------------------------------------
	struct mvVec2
	{
		float x, y;

		operator ImVec2()
		{
			return ImVec2{ x, y };
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
		int r=0, g=0, b=0, a=255;
		bool specified = true;

		mvColor() = default;

		mvColor(int r, int g, int b, int a, bool specified = true)
			: r(r), g(g), b(b), a(a), specified(specified)
		{
		}

		explicit mvColor(ImVec4 color)
		{
			r = (int)(color.x*255.0f);
			g = (int)(color.y*255.0f);
			b = (int)(color.z*255.0f);
			a = (int)(color.w*255.0f);
		}

		operator ImU32()
		{
			return ImGui::ColorConvertFloat4ToU32(toVec4());
		}

		ImVec4 toVec4() const
		{
			if (r < 0 || g < 0 || b < 0 || a < 0)
				return ImVec4(0, 0, 0, -1);
			return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		}

	};

	typedef std::unordered_map<int, mvColor> ThemeColors;

}

#define MV_DEFAULT_COLOR Marvel::mvColor{255, 255, 255, 255, false}
#define MV_TRANSPARENT   Marvel::mvColor{255, 255, 255,   0, true }