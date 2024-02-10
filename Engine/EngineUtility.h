#pragma once

#include <stdarg.h>
#include "stdint.h"

class EngineUtilityLibrary
{
public:
	/** Enums. */

	// Convert enum to bit mask.
	// You still need to cast enum to uint8.
	static uint64_t ConvertValToBitMask(uint8_t val) { return static_cast<uint64_t>(1ui64 << val); }
	static uint64_t ConvertValListToBitMask(int val0, ...)
	{
		uint64_t bitMask = 0;
		va_list args;
		va_start(args, val0);
		int val = val0;
		while (val != -1)
		{
			bitMask |= ConvertValToBitMask((uint8_t)val);
			val = va_arg(args, int);
		}
		va_end(args);
		return bitMask;
	}

	static constexpr uint32_t StringToUint32(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (StringToUint32(str, h + 1) * 33) ^ str[h];
	}
};

// More convenient way to convert enum to bit mask.
#define ENUM2BIT(enum_val) \
			EngineUtilityLibrary::ConvertValToBitMask(static_cast<uint8_t>(enum_val))

#define ENUMLIST2BIT(enum_val0, ...) \
			EngineUtilityLibrary::ConvertValListToBitMask(static_cast<int>(enum_val0), __VA_ARGS__, -1)

/** Array. */

#define SIZEOFARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

/** Assert. */

#if _DEBUG
#define DebugEngineTrap() __debugbreak()
#else // _DEBUG
#define DebugEngineTrap() void(0)
#endif // _DEBUG

inline void engine_assert(bool condition)
{
	if (!condition) DebugEngineTrap();
}
