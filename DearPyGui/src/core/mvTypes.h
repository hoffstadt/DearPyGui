#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

typedef float  real32;
typedef double real64;

typedef bool b8;
typedef std::int32_t b32;

typedef std::int8_t   i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::int8_t   s8;
typedef std::int16_t s16;
typedef std::int32_t s32;
typedef std::int64_t s64;

typedef std::uint8_t   u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef real32 f32;
typedef real64 f64;

typedef std::unordered_map<long, float> mvThemeStyles;
typedef unsigned long long              mvUUID;

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

#if !defined(mv_internal)
#define mv_internal static
#endif
#define mv_local_persist static
#define mv_global static
#define mv_python_function PyObject*
#ifndef mv_impl
#define mv_impl
#endif
