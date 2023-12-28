#pragma once

inline void engine_assert(bool condition)
{
	if (!condition) __debugbreak();
}
