#pragma once
#include <wrl.h>

namespace Marvel {

	template <typename T>
	using mvComPtr = Microsoft::WRL::ComPtr<T>;

}